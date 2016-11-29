#include "model.h"
#include "xml.h"
#include "m_map.h"
#include "conv.h"

collada_context* collada_context_new(char* file)
{
	collada_context* ctx = malloc(sizeof(collada_context));
	ctx->ctx = parse_xml_file(file);
	return ctx;
}

/* parse xml file to array of model meshs */

typedef struct dae_geo_mesh
{
	m_string* id;
	m_array* positions;
	m_array* normals;
	m_array* uvs;
	m_array* vcount;
	m_array* p;
} dae_geo_mesh;

typedef struct dae_controller_skin
{
	m_string* id;
	dae_geo_mesh* mesh;
	m_matrix4 bind_shape_matrix;
	m_array* join_names;
	m_array* inverse_bind_poses;
	m_map* join_name_to_inv_bind_poses;
	m_array* weights;
	m_array* vcount;
	m_array* v;
	m_array* vertex_join_ids;
	m_array* vertex_weights;
	int max_vcount;
} dae_controller_skin;

typedef struct dae_visual_join
{
	m_string* id;
	m_string* name;
	m_string* sid;
	m_matrix4 bind_pose;
	m_matrix4 inverse_bind_pose;
	m_array* children;
	struct dae_visual_join* parent;
	int ref;
	int index;
} dae_visual_join;

typedef struct dae_visual_node
{
	dae_visual_join* join;
	dae_controller_skin* skin;
	dae_geo_mesh* mesh;
	m_string* id;
	m_string* name;
	m_array* children;
	struct dae_visual_node* parent;
} dae_visual_node;

static dae_geo_mesh* new_dae_geo_mesh()
{
	dae_geo_mesh* ret = malloc(sizeof(dae_geo_mesh));
	ret->id = m_string_new();
	ret->positions = m_array_new(sizeof(m_vector3));
	ret->normals = m_array_new(sizeof(m_vector3));
	ret->uvs = m_array_new(sizeof(m_vector2));
	ret->vcount = m_array_new(sizeof(int));
	ret->p = m_array_new(sizeof(int));
	return ret;
}

static void free_dae_geo_mesh(dae_geo_mesh* mesh)
{
	m_string_free(mesh->id);
	m_array_free(mesh->positions);
	m_array_free(mesh->normals);
	m_array_free(mesh->uvs);
	m_array_free(mesh->vcount);
	m_array_free(mesh->p);
	free(mesh);
}

static dae_controller_skin* new_dae_controller_skin()
{
	dae_controller_skin* ret = malloc(sizeof(dae_controller_skin));
	ret->id = m_string_new();
	ret->mesh = 0;
	ret->max_vcount = 0;
	ret->bind_shape_matrix = matrix4_identity;
	ret->join_names = m_array_new(sizeof(m_string*));
	ret->inverse_bind_poses = m_array_new(sizeof(m_matrix4));
	ret->join_name_to_inv_bind_poses = m_map_new(sizeof(m_matrix4));
	ret->weights = m_array_new(sizeof(float));
	ret->vcount = m_array_new(sizeof(int));
	ret->vertex_join_ids = m_array_new(sizeof(float));
	ret->vertex_weights = m_array_new(sizeof(float));
	ret->v = m_array_new(sizeof(int));
	return ret;
}

static void free_dae_controller_skin(dae_controller_skin* cs)
{
	int i;
	for(i = 0; i < cs->join_names->len; i++)
	{
		m_string_free(m_array_get(cs->join_names, m_string*, i));
	}
	m_string_free(cs->id);
	m_map_free(cs->join_name_to_inv_bind_poses);
	m_array_free(cs->join_names);
	m_array_free(cs->inverse_bind_poses);
	m_array_free(cs->weights);
	m_array_free(cs->vcount);
	m_array_free(cs->vertex_join_ids);
	m_array_free(cs->vertex_weights);
	m_array_free(cs->v);
	free(cs);
}

static dae_visual_join* new_dae_visual_join()
{
	dae_visual_join* ret = malloc(sizeof(dae_visual_join));
	ret->id = m_string_new();
	ret->name = m_string_new();
	ret->sid = m_string_new();
	ret->children = m_array_new(sizeof(dae_visual_join*));
	ret->parent = 0;
	ret->bind_pose = matrix4_identity;
	ret->inverse_bind_pose = matrix4_identity;
	ret->ref = 0;
	return ret;
}

static void free_dae_visual_join(dae_visual_join* join)
{
	join->ref--;
	if(join->ref <= 0)
	{
		int i;
		m_string_free(join->id);
		m_string_free(join->name);
		m_string_free(join->sid);
		for(i = 0; i < join->children->len; i++)
		{
			free_dae_visual_join(m_array_get(join->children, dae_visual_join*, i));
		}
		m_array_free(join->children);
		free(join);
	}
}

static dae_visual_node* new_dae_visual_node()
{
	dae_visual_node* ret = malloc(sizeof(dae_visual_node));
	ret->join = 0;
	ret->skin = 0;
	ret->mesh = 0;
	ret->id = m_string_new();
	ret->name = m_string_new();
	ret->children = m_array_new(sizeof(dae_visual_node*));
	ret->parent = 0;
	return ret;
}

static void free_dae_visual_node(dae_visual_node* node)
{
	int i;
	if(node->join) free_dae_visual_join(node->join);
	m_string_free(node->id);
	m_string_free(node->name);
	for(i = 0; i < node->children->len; i++)
	{
		free_dae_visual_node(m_array_get(node->children, dae_visual_node*, i));
	}
	m_array_free(node->children);
	free(node);
}

typedef struct dae_context
{
	m_array* geo_meshs;
	m_array* controller_skins;
	m_array* visual_nodes;
} dae_context;

static dae_context* new_dae_context()
{
	dae_context* ret = malloc(sizeof(dae_context));
	ret->geo_meshs = m_array_new(sizeof(dae_geo_mesh*));
	ret->controller_skins = m_array_new(sizeof(dae_controller_skin*));
	ret->visual_nodes = m_array_new(sizeof(dae_visual_node*));
	return ret;
}

static void free_dae_context(dae_context* ctx)
{
	int i;
	for(i = 0; i < ctx->geo_meshs->len; i++)
	{
		free_dae_geo_mesh(m_array_get(ctx->geo_meshs, dae_geo_mesh*, i));
	}
	m_array_free(ctx->geo_meshs);
	for(i = 0; i < ctx->controller_skins->len; i++)
	{
		free_dae_controller_skin(m_array_get(ctx->controller_skins, dae_controller_skin*, i));
	}
	m_array_free(ctx->controller_skins);
	for(i = 0;i < ctx->visual_nodes->len; i++)
	{
		free_dae_visual_node(m_array_get(ctx->visual_nodes, dae_visual_node*, i));
	}
	m_array_free(ctx->visual_nodes);
	free(ctx);
}

static void push_vector3_array(m_array* arr, m_string* text)
{
	char temp[64];
	m_vector3 p;
	for(int j = 0, temp_index = 0, p_index = 0, size = text->len; j <= size; j++)
	{
		temp[temp_index] = text->str[j];
		if(temp[temp_index] == ' ' || j == size)
		{
			/* prepare string for atof */
			temp[temp_index] = '\0';
			/* convert string to float */
			p.v[p_index] = atof(temp);
			/* move to next vector axis index */
			p_index++;
			if(p_index == 3)
			{
				/* complete vector, push it to array */
				m_array_push(arr, &p);
				/* prepare next vector */
				p_index = 0;
			}
			/* reset temp buffer index */
			temp_index = 0;
		}
		/* increase temp buffer index */
		else temp_index++;
	}
}

static void push_vector2_array(m_array* arr, m_string* text)
{
	char temp[64];
	m_vector2 p;
	for(int j = 0, temp_index = 0, p_index = 0, size = text->len; j <= size; j++)
	{
		temp[temp_index] = text->str[j];
		if(temp[temp_index] == ' ' || j == size)
		{
			/* prepare string for atof */
			temp[temp_index] = '\0';
			/* convert string to float */
			p.v[p_index] = atof(temp);
			/* move to next vector axis index */
			p_index++;
			if(p_index == 2)
			{
				/* complete vector, push it to array */
				m_array_push(arr, &p);
				/* prepare next vector */
				p_index = 0;
			}
			/* reset temp buffer index */
			temp_index = 0;
		}
		/* increase temp buffer index */
		else temp_index++;
	}
}

static void push_float_array(m_array* arr, m_string* text)
{
	char temp[64];
	float p;
	for(int j = 0, temp_index = 0, size = text->len; j <= size; j++)
	{
		temp[temp_index] = text->str[j];
		if(temp[temp_index] == ' ' || j == size)
		{
			/* string may end with space */
			if(temp_index == 0) continue;
			/* prepare string for atof */
			temp[temp_index] = '\0';
			/* convert string to float */
			p = atof(temp);
			m_array_push(arr, &p);
			/* reset temp buffer index */
			temp_index = 0;
		}
		/* increase temp buffer index */
		else temp_index++;
	}
}

static void push_int_array(m_array* arr, m_string* text)
{
	char temp[64];
	int p;
	for(int j = 0, temp_index = 0, size = text->len; j <= size; j++)
	{
		temp[temp_index] = text->str[j];
		if(temp[temp_index] == ' ' || j == size)
		{
			/* string may end with space */
			if(temp_index == 0) continue;
			/* prepare string for atoi */
			temp[temp_index] = '\0';
			/* convert string to int */
			p = atoi(temp);
			m_array_push(arr, &p);
			/* reset temp buffer index */
			temp_index = 0;
		}
		/* increase temp buffer index */
		else temp_index++;
	}
}

static m_matrix4 convert_matrix_4(m_string* text)
{
	char temp[64];
	m_matrix4 ret;
	for(int j = 0, temp_index = 0, ret_index = 0, size = text->len; j <= size; j++)
	{
		temp[temp_index] = text->str[j];
		if(temp[temp_index] == ' ' || j == size)
		{
			/* string may end with space */
			if(temp_index == 0) continue;
			/* prepare string for atoi */
			temp[temp_index] = '\0';
			/* convert string to int */
			ret.m[ret_index] = atof(temp);
			ret_index++;
			/* reset temp buffer index */
			temp_index = 0;
		}
		/* increase temp buffer index */
		else temp_index++;
	}
	return ret;
}

static void push_matrix4_array(m_array* arr, m_string* text)
{
	char temp[64];
	m_matrix4 ret;
	for(int j = 0, temp_index = 0, ret_index = 0, size = text->len; j <= size; j++)
	{
		temp[temp_index] = text->str[j];
		if(temp[temp_index] == ' ' || j == size)
		{
			/* string may end with space */
			if(temp_index == 0) continue;
			/* prepare string for atoi */
			temp[temp_index] = '\0';
			/* convert string to int */
			ret.m[ret_index] = atof(temp);
			ret_index++;
			if(ret_index == 16)
			{
				m_array_push(arr, &ret);
				ret_index = 0;
			}
			/* reset temp buffer index */
			temp_index = 0;
		}
		/* increase temp buffer index */
		else temp_index++;
	}
}

static void push_string_array(m_array* arr, m_string* text)
{
	int start = -1;
	int end = -2;
	int i;
	int size = text->len;
	for(i = 0; i <= size; i++)
	{
		if(text->str[i] == ' ' || i == size)
		{
			if(end >= start)
			{
				m_string* s = m_string_new();
				m_string_cat_char_array_with_len(s, text->str + start, end - start + 1);
				m_array_push(arr, &s);
			}
			start = -1;
			end = -2;
		}
		else
		{
			if(start == -1) start = i;
			end = i;
		}
	}
}

static dae_visual_join* build_bone(xml_element* node, dae_visual_join* parent)
{
	int i;
	for(i = 0; i < node->children->len; i++)
	{
		xml_element* child = m_array_get(node->children, xml_element*, i);
		xml_attribute* type_join = xml_context_find_attribute(child, "type");
		if(type_join && strcmp(type_join->value->str, "JOINT") == 0)
		{
			dae_visual_join* join = new_dae_visual_join();
			m_string_cat_string(join->id, xml_context_find_attribute(child, "id")->value);
			m_string_cat_string(join->name, xml_context_find_attribute(child, "name")->value);
			m_string_cat_string(join->sid, xml_context_find_attribute(child, "sid")->value);
			xml_element* matrix = xml_context_find(child, "matrix", 0);
			join->bind_pose = convert_matrix_4(matrix->value);
			if(!parent) parent = join;
			else
			{
				m_array_push(parent->children, &join);
				join->parent = parent;
			}
			join->ref = 0;
			build_bone(child, join);
		}
	}
	return parent;
}

static void apply_skin_to_bone(dae_controller_skin* skin, dae_visual_join* join)
{
	int i;
	join->inverse_bind_pose = m_map_get(skin->join_name_to_inv_bind_poses, m_matrix4, qskey(join->sid));
	for(i = 0; i < skin->join_names->len; i++)
	{
		if(strcmp(join->sid->str, m_array_get(skin->join_names, m_string*, i)->str) == 0)
		{
			join->index = i;
			break;
		}
	}
	for(i = 0; i < join->children->len; i++)
	{
		apply_skin_to_bone(skin, m_array_get(join->children, dae_visual_join*, i));
	}
}

static dae_visual_node* build_visual_node(dae_context* ctx, xml_element* node, dae_visual_node* parent, dae_visual_join* join)
{
	int i;
	int j;
	dae_visual_node* dvn = new_dae_visual_node();
	if(!parent)
	{
		parent = dvn;
	}
	else
	{
		m_array_push(parent->children, &dvn);
		dvn->parent = parent;
	}
	m_string_cat_string(dvn->id, xml_context_find_attribute(node, "id")->value);
	m_string_cat_string(dvn->name, xml_context_find_attribute(node, "name")->value);
	for(i = 0; i < node->children->len; i++)
	{
		xml_element* elm = m_array_get(node->children, xml_element*, i);
		if(strcmp(elm->name->str, "instance_controller") == 0)
		{
			xml_attribute* url = xml_context_find_attribute(elm, "url");
			for(j = 0; j < ctx->controller_skins->len; j++)
			{
				dae_controller_skin* skin = m_array_get(ctx->controller_skins, dae_controller_skin*, j);
				if(strcmp(skin->id->str, url->value->str + 1) == 0)
				{
					apply_skin_to_bone(skin, join);
					dvn->join = join;
					dvn->skin = skin;
					join->ref++;
					break;
				}
			}
		}
		else if(strcmp(elm->name->str, "instance_geometry") == 0)
		{
			xml_attribute* url = xml_context_find_attribute(elm, "url");
			for(j = 0; j < ctx->geo_meshs->len; j++)
			{
				dae_geo_mesh* mesh = m_array_get(ctx->geo_meshs, dae_geo_mesh*, j);
				if(strcmp(mesh->id->str, url->value->str + 1) == 0)
				{
					dvn->mesh = mesh;
					break;
				}
			}
		}
		else if(strcmp(elm->name->str, "node") == 0)
		{
			build_visual_node(ctx, elm, dvn, join);
		}
	}
	return dvn;
}

static void build_library_geometries(dae_context* dae_ctx, collada_context* context)
{
	int i;
	xml_element* library_geometries = xml_context_find(m_array_get(context->ctx->elements, xml_element*, 0), "library_geometries", 0);
	for(i = 0; i < library_geometries->children->len; i++)
	{
		int count;
		xml_element* geometry = m_array_get(library_geometries->children, xml_element*, i);
		dae_geo_mesh* mesh = new_dae_geo_mesh();
		m_array_push(dae_ctx->geo_meshs, &mesh);

		m_string_cat_string(mesh->id, xml_context_find_attribute(geometry, "id")->value);

		xml_element* vertices = xml_context_find(geometry, "vertices", 0);
		xml_element* vertices_input = xml_context_find_with_attribute(vertices, "input", "semantic", "POSITION");
		xml_element* polylist = xml_context_find(geometry, "polylist", 0);

		xml_element* input_vertex = xml_context_find_with_attribute(polylist, "input", "semantic", "VERTEX");
		xml_element* input_normal = xml_context_find_with_attribute(polylist, "input", "semantic", "NORMAL");
		xml_element* input_texcoord = xml_context_find_with_attribute(polylist, "input", "semantic", "TEXCOORD");
		xml_element* vcount = xml_context_find(polylist, "vcount", 0);
		xml_element* p = xml_context_find(polylist, "p", 0);

		xml_element* source_position = xml_context_find_with_attribute(geometry, "source", "id", xml_context_find_attribute(vertices_input, "source")->value->str + 1);
		xml_element* source_position_float_array = xml_context_find(source_position, "float_array", 0);
		count = atoi(xml_context_find_attribute(source_position_float_array, "count")->value->str);
		m_array_reserve(mesh->positions, count / 3);
		push_vector3_array(mesh->positions, source_position_float_array->value);

		xml_element* source_normal = xml_context_find_with_attribute(geometry, "source", "id", xml_context_find_attribute(input_normal, "source")->value->str + 1);
		xml_element* source_normal_float_array = xml_context_find(source_normal, "float_array", 0);
		count = atoi(xml_context_find_attribute(source_normal_float_array, "count")->value->str);
		m_array_reserve(mesh->normals, count / 3);
		push_vector3_array(mesh->normals, source_normal_float_array->value);

		if(input_texcoord)
		{
			xml_element* source_texcoord = xml_context_find_with_attribute(geometry, "source", "id", xml_context_find_attribute(input_texcoord, "source")->value->str + 1);
			xml_element* source_texcoord_float_array = xml_context_find(source_texcoord, "float_array", 0);
			count = atoi(xml_context_find_attribute(source_texcoord_float_array, "count")->value->str);
			m_array_reserve(mesh->uvs, count / 2);
			push_vector2_array(mesh->uvs, source_texcoord_float_array->value);
		}

		count = atoi(xml_context_find_attribute(polylist, "count")->value->str);
		m_array_reserve(mesh->vcount, count);
		push_int_array(mesh->vcount, vcount->value);

		/* need fix for source_color */
		if(!input_texcoord) count *= 6; // each triangle define by vcount has 3 vertices, each vertex has 1 position and 1 normal
		else count *= 9;// each triangle define by vcount has 3 vertices, each vertex has 1 position and 1 normal and 1 texcoord
		m_array_reserve(mesh->p, count);
		push_int_array(mesh->p, p->value);
	}
}

static void build_library_controllers(dae_context* dae_ctx, collada_context* context)
{
	int i;
	xml_element* library_controllers = xml_context_find(m_array_get(context->ctx->elements, xml_element*, 0), "library_controllers", 0);
	for(i = 0; i < library_controllers->children->len; i++)
	{
		int count = 0;
		int j = 0;
		int k = 0;
		int v_index = 0;
		xml_element* controller = m_array_get(library_controllers->children, xml_element*, i);
		dae_controller_skin* controller_skin = new_dae_controller_skin();
		m_array_push(dae_ctx->controller_skins, &controller_skin);

		m_string_cat_string(controller_skin->id, xml_context_find_attribute(controller, "id")->value);

		xml_element* skin = xml_context_find(controller, "skin", 0);
		controller_skin->bind_shape_matrix = convert_matrix_4(xml_context_find(skin, "bind_shape_matrix", 0)->value);

		char* skin_mesh = xml_context_find_attribute(skin, "source")->value->str + 1;
		for(j = 0; j < dae_ctx->geo_meshs->len; j++)
		{
			dae_geo_mesh* dgm = m_array_get(dae_ctx->geo_meshs, dae_geo_mesh*, j);
			if(strcmp(dgm->id->str, skin_mesh) == 0)
			{
				controller_skin->mesh = dgm;
				break;
			}
		}

		xml_element* joints = xml_context_find(skin, "joints", 0);
		xml_element* vertex_weights = xml_context_find(skin, "vertex_weights", 0);

		xml_element* input_join = xml_context_find_with_attribute(joints, "input", "semantic", "JOINT");
		xml_element* source_join_names = xml_context_find_with_attribute(skin, "source", "id", xml_context_find_attribute(input_join,"source")->value->str + 1);
		xml_element* source_join_names_name_array = xml_context_find(source_join_names, "Name_array", 0);
		count = atoi(xml_context_find_attribute(source_join_names_name_array, "count")->value->str);
		m_array_reserve(controller_skin->join_names, count);
		push_string_array(controller_skin->join_names, source_join_names_name_array->value);

		xml_element* input_inv_bind_matrix = xml_context_find_with_attribute(joints, "input", "semantic", "INV_BIND_MATRIX");
		xml_element* source_inv_bind_poses = xml_context_find_with_attribute(skin, "source", "id", xml_context_find_attribute(input_inv_bind_matrix, "source")->value->str + 1);
		xml_element* source_inv_bind_poses_float_array = xml_context_find(source_inv_bind_poses, "float_array", 0);
		count = atoi(xml_context_find_attribute(source_inv_bind_poses_float_array, "count")->value->str) / 16;
		m_array_reserve(controller_skin->inverse_bind_poses, count);
		push_matrix4_array(controller_skin->inverse_bind_poses, source_inv_bind_poses_float_array->value);

		for(j = 0; j < count; j++)
		{
			m_string* name = m_array_get(controller_skin->join_names, m_string*, j);
			m_matrix4 inv_bind_pose = m_array_get(controller_skin->inverse_bind_poses, m_matrix4, j);
			m_map_set(controller_skin->join_name_to_inv_bind_poses, qskey(name), &inv_bind_pose);
		}

		xml_element* input_weight = xml_context_find_with_attribute(vertex_weights, "input", "semantic", "WEIGHT");
		xml_element* source_weight = xml_context_find_with_attribute(skin, "source", "id", xml_context_find_attribute(input_weight, "source")->value->str + 1);
		xml_element* source_weight_float_array = xml_context_find(source_weight, "float_array", 0);
		count = atoi(xml_context_find_attribute(source_weight_float_array, "count")->value->str);
		m_array_reserve(controller_skin->weights, count);
		push_float_array(controller_skin->weights, source_weight_float_array->value);

		xml_element* vcount = xml_context_find(vertex_weights, "vcount", 0);
		xml_element* v = xml_context_find(vertex_weights, "v", 0);
		count = atoi(xml_context_find_attribute(vertex_weights, "count")->value->str);
		m_array_reserve(controller_skin->vcount, count);
		push_int_array(controller_skin->vcount, vcount->value);

		count = 0;
		for(j = 0; j < controller_skin->vcount->len; j++)
		{
			int v = m_array_get(controller_skin->vcount, int, j);
			count += v * 2;
			controller_skin->max_vcount = MAX(controller_skin->max_vcount, v);
		}
		m_array_reserve(controller_skin->v, count);
		push_int_array(controller_skin->v, v->value);

		m_array_reserve(controller_skin->vertex_join_ids, controller_skin->vcount->len * controller_skin->max_vcount);
		m_array_reserve(controller_skin->vertex_weights, controller_skin->vcount->len * controller_skin->max_vcount);
		v_index = 0;
		for(j = 0; j < controller_skin->vcount->len; j++)
		{
			int c = m_array_get(controller_skin->vcount, int, j);
			if(c == 0)
			{
				for(k = 0; k < controller_skin->max_vcount; k++)
				{
					float id = 0.0;
					m_array_push(controller_skin->vertex_join_ids, &id);
					float weight = k == 0 ? 1.0 : 0.0;
					m_array_push(controller_skin->vertex_weights, &weight);
				}
			}
			else
			{
				for(k = 0; k < c; k++)
				{
					if(v_index < controller_skin->v->len)
					{
						int join_index = m_array_get(controller_skin->v, int, v_index);
						v_index++;
						int weight_index = m_array_get(controller_skin->v, int, v_index);
						v_index++;
						float id = (float)join_index;
						m_array_push(controller_skin->vertex_join_ids, &id);
						float weight = m_array_get(controller_skin->weights, float, weight_index);
						m_array_push(controller_skin->vertex_weights, &weight);
					}
					else
					{
						// error
						float id = 0;
						m_array_push(controller_skin->vertex_join_ids, &id);
						float weight = k == 0 ? 1.0 : 0.0;
						m_array_push(controller_skin->vertex_weights, &weight);
					}
				}
				if(c > 0 && c < controller_skin->max_vcount)
				{
					for(k = c; k < controller_skin->max_vcount; k++)
					{
						float zero = 0.0;
						m_array_push(controller_skin->vertex_join_ids, &zero);
						m_array_push(controller_skin->vertex_weights, &zero);
					}
				}
			}
		}
	}
}

static void build_library_visual_scenes(dae_context* dae_ctx, collada_context* context)
{
	int i;
	xml_element* library_visual_scenes = xml_context_find(m_array_get(context->ctx->elements, xml_element*, 0), "library_visual_scenes", 0);
	for(i = 0; i < library_visual_scenes->children->len; i++)
	{
		int count;
		int j;
		xml_element* visual_scene = m_array_get(library_visual_scenes->children, xml_element*, i);
		xml_element* current_join_data = 0;
		for(j = 0; j < visual_scene->children->len; j++)
		{
			xml_element* node = m_array_get(visual_scene->children, xml_element*, j);
			xml_element* test_joint = xml_context_find_with_attribute(node, "node", "type", "JOINT");
			if(test_joint)
			{
				current_join_data = node;
			}
			else
			{
				// node to render
				dae_visual_join* join = 0;
				if(current_join_data)
				{
					join = build_bone(current_join_data, join);
				}
				dae_visual_node* new_visual_node = 0;
				new_visual_node = build_visual_node(dae_ctx, node, new_visual_node, join);
				m_array_push(dae_ctx->visual_nodes, &new_visual_node);

				if(join && join->ref == 0)
				{
					free_dae_visual_join(join);
				}
			}
		}
	}
}



static model_skin_join* new_model_skin_join(dae_visual_join* vj, model_skin_join* parent)
{
	int i;
	model_skin_join* ret = malloc(sizeof(model_skin_join));
	ret->sid = m_string_new_from_string(vj->sid);
	ret->uniform = m_string_new();
	m_string_reserve(ret->uniform, 8);
	int_to_chars(s, vj->index);
	m_string_cat_char_array(ret->uniform, "join[");
	m_string_cat_char_array(ret->uniform, s);
	m_string_cat_char_array(ret->uniform, "]");
	ret->children = m_array_new(sizeof(model_skin_join*));
	ret->parent = 0;
	ret->ref = 0;
	ret->world_matrix = matrix4_identity;
	ret->final_matrix = matrix4_identity;
	ret->bind_pose = vj->bind_pose;
	ret->inverse_bind_pose = vj->inverse_bind_pose;
	if(!parent)
	{
		parent = ret;
	}
	else
	{
		m_array_push(parent->children, &ret);
		ret->parent = parent;
	}
	for(i = 0; i < vj->children->len; i++)
	{
		new_model_skin_join(m_array_get(vj->children, dae_visual_join*, i), ret);
	}
	return ret;
}

static void apply_skin_to_model_mesh(model_mesh* mesh, dae_controller_skin* skin)
{
	int i;
	int j;
	dae_geo_mesh* geo_mesh = skin->mesh;
	GLfloat* gl_data = 0;
	long total_size = 0;
	int one_vertex_size = 0;
	mesh->joins_per_vertex = skin->max_vcount;

	if(geo_mesh->uvs->len) //has uvs
	{
		mesh->has_uvs = 1;
		// 3 vertex + 3 normal + 2 texcoord + max_vcount joins_id + max_vcount weights
		one_vertex_size = 3 + 3 + 2 + 2 * skin->max_vcount;
		mesh->vertices_count = geo_mesh->p->len / 3;
		total_size = geo_mesh->p->len / 3 * one_vertex_size * sizeof(GLfloat);
		gl_data = malloc(total_size);
		int data_index = 0;
		for(i = 0; i < geo_mesh->p->len; i+= 3)
		{
			int vertex_id = m_array_get(geo_mesh->p, int, i);
			int normal_id = m_array_get(geo_mesh->p, int, i + 1);
			int uv_id = m_array_get(geo_mesh->p, int, i + 2);
			m_vector3 pos = m_array_get(geo_mesh->positions, m_vector3, vertex_id);
			m_vector3 nor = m_array_get(geo_mesh->normals, m_vector3, normal_id);
			m_vector2 uv = m_array_get(geo_mesh->uvs, m_vector2, uv_id);
			gl_data[data_index++] = pos.v[0];
			gl_data[data_index++] = pos.v[1];
			gl_data[data_index++] = pos.v[2];
			gl_data[data_index++] = nor.v[0];
			gl_data[data_index++] = nor.v[1];
			gl_data[data_index++] = nor.v[2];
			gl_data[data_index++] = uv.v[0];
			gl_data[data_index++] = uv.v[1];

			for(j = 0; j < skin->max_vcount; j++)
			{
				gl_data[data_index++] = m_array_get(skin->vertex_join_ids, float, vertex_id * skin->max_vcount + j);
				gl_data[data_index++] = m_array_get(skin->vertex_weights, float, vertex_id * skin->max_vcount + j);
			}
		}
	}
	else
	{
		mesh->has_uvs = 0;
		one_vertex_size = 3 + 3 + 2 * skin->max_vcount;
		mesh->vertices_count = geo_mesh->p->len / 2;
		total_size = geo_mesh->p->len / 2 * one_vertex_size * sizeof(GLfloat);
		gl_data = malloc(total_size);
		int data_index = 0;
		for(i = 0; i < geo_mesh->p->len; i+= 2)
		{
			int vertex_id = m_array_get(geo_mesh->p, int, i);
			int normal_id = m_array_get(geo_mesh->p, int, i + 1);
			m_vector3 pos = m_array_get(geo_mesh->positions, m_vector3, vertex_id);
			m_vector3 nor = m_array_get(geo_mesh->normals, m_vector3, normal_id);
			gl_data[data_index++] = pos.v[0];
			gl_data[data_index++] = pos.v[1];
			gl_data[data_index++] = pos.v[2];
			gl_data[data_index++] = nor.v[0];
			gl_data[data_index++] = nor.v[1];
			gl_data[data_index++] = nor.v[2];
			for(j = 0; j < skin->max_vcount; j++)
			{
				gl_data[data_index++] = m_array_get(skin->vertex_join_ids, float, vertex_id * skin->max_vcount + j);
				gl_data[data_index++] = m_array_get(skin->vertex_weights, float, vertex_id * skin->max_vcount + j);
			}
		}
	}

	glGenVertexArrays(1, &mesh->vao);
	glGenBuffers(1, &mesh->vbo);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBufferData(GL_ARRAY_BUFFER, total_size, gl_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	free(gl_data);
}

static void apply_geo_mesh_to_model_mesh(model_mesh* mesh, dae_geo_mesh* geo_mesh)
{
	int i;
	int j;
	GLfloat* gl_data = 0;
	long total_size = 0;
	int one_vertex_size = 0;

	if(geo_mesh->uvs->len) //has uvs
	{
		mesh->has_uvs = 1;
		// 3 vertex + 3 normal + 2 texcoord
		one_vertex_size = 3 + 3 + 2;
		mesh->vertices_count = geo_mesh->p->len / 3;
		total_size = geo_mesh->p->len / 3 * one_vertex_size * sizeof(GLfloat);
		gl_data = malloc(total_size);
		int data_index = 0;
		for(i = 0; i < geo_mesh->p->len; i+= 3)
		{
			int vertex_id = m_array_get(geo_mesh->p, int, i);
			int normal_id = m_array_get(geo_mesh->p, int, i + 1);
			int uv_id = m_array_get(geo_mesh->p, int, i + 2);
			m_vector3 pos = m_array_get(geo_mesh->positions, m_vector3, vertex_id);
			m_vector3 nor = m_array_get(geo_mesh->normals, m_vector3, normal_id);
			m_vector2 uv = m_array_get(geo_mesh->uvs, m_vector2, uv_id);
			gl_data[data_index++] = pos.v[0];
			gl_data[data_index++] = pos.v[1];
			gl_data[data_index++] = pos.v[2];
			gl_data[data_index++] = nor.v[0];
			gl_data[data_index++] = nor.v[1];
			gl_data[data_index++] = nor.v[2];
			gl_data[data_index++] = uv.v[0];
			gl_data[data_index++] = uv.v[1];
		}
	}
	else
	{
		mesh->has_uvs = 0;
		one_vertex_size = 3 + 3;
		mesh->vertices_count = geo_mesh->p->len / 2;
		total_size = geo_mesh->p->len / 2 * one_vertex_size * sizeof(GLfloat);
		gl_data = malloc(total_size);
		int data_index = 0;
		for(i = 0; i < geo_mesh->p->len; i+= 2)
		{
			int vertex_id = m_array_get(geo_mesh->p, int, i);
			int normal_id = m_array_get(geo_mesh->p, int, i + 1);
			m_vector3 pos = m_array_get(geo_mesh->positions, m_vector3, vertex_id);
			m_vector3 nor = m_array_get(geo_mesh->normals, m_vector3, normal_id);
			gl_data[data_index++] = pos.v[0];
			gl_data[data_index++] = pos.v[1];
			gl_data[data_index++] = pos.v[2];
			gl_data[data_index++] = nor.v[0];
			gl_data[data_index++] = nor.v[1];
			gl_data[data_index++] = nor.v[2];
		}
	}

	glGenVertexArrays(1, &mesh->vao);
	glGenBuffers(1, &mesh->vbo);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBufferData(GL_ARRAY_BUFFER, total_size, gl_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	free(gl_data);
}

static model_mesh* new_model_mesh(dae_visual_node* node, model_mesh* parent, model_skin_join* skeleton)
{
	int i;
	model_mesh* ret = malloc(sizeof(model_mesh));
	ret->sub_meshs = m_array_new(sizeof(model_mesh*));
	ret->bind_shape_matrix = matrix4_identity;
	ret->vao = 0;
	ret->vbo = 0;
	ret->joins_per_vertex = 0;

	if(node->join)
	{
		if(!skeleton)
		{
			skeleton = new_model_skin_join(node->join, skeleton);
		}
		ret->skeleton = skeleton;
		skeleton->ref++;
		apply_skin_to_model_mesh(ret, node->skin);
	}
	else
	{
		ret->skeleton = 0;
		apply_geo_mesh_to_model_mesh(ret, node->mesh);
	}

	if(!parent)
	{
		parent = ret;
	}
	else
	{
		m_array_push(parent->sub_meshs, &ret);
		ret->parent = parent;
	}
	for(i = 0; i < node->children->len; i++)
	{
		new_model_mesh(m_array_get(node->children, dae_visual_node*, i), ret, skeleton);
	}
	return ret;
}

static void model_skin_join_free(model_skin_join* join)
{
	join->ref--;
	if(join->ref <= 0)
	{
		int i;
		m_string_free(join->sid);
		m_string_free(join->uniform);
		for(i = 0; i < join->children->len; i++)
		{
			model_skin_join_free(m_array_get(join->children, model_skin_join*, i));
		}
		m_array_free(join->children);
		free(join);
	}
}

void model_mesh_free(model_mesh* mesh)
{
	int i;
	if(mesh->vbo) glDeleteBuffers(1, &mesh->vbo);
  if(mesh->vao) glDeleteVertexArrays(1, &mesh->vao);
	if(mesh->skeleton) model_skin_join_free(mesh->skeleton);

	for(i = 0; i < mesh->sub_meshs->len; i++)
	{
		model_mesh_free(m_array_get(mesh->sub_meshs, model_mesh*, i));
	}
	m_array_free(mesh->sub_meshs);
	free(mesh);
}

static m_array* build_model_mesh(dae_context* dae_ctx)
{
	int i;
	m_array* ret = m_array_new(sizeof(model_mesh*));
	for(i = 0; i < dae_ctx->visual_nodes->len; i++)
	{
		model_mesh* mesh = new_model_mesh(m_array_get(dae_ctx->visual_nodes, dae_visual_node*, i), 0, 0);
		m_array_push(ret, &mesh);
	}
	return ret;
}

m_array* collada_context_parse(collada_context* context)
{
	dae_context* dae_ctx = new_dae_context();
	/* construct geo meshs */
	build_library_geometries(dae_ctx, context);

	/* construct controller skins */
	build_library_controllers(dae_ctx, context);

	/* construct visual nodes */
	build_library_visual_scenes(dae_ctx, context);

	/* build render mesh */
	m_array* ret = build_model_mesh(dae_ctx);

	/* free up */
	free_dae_context(dae_ctx);
	return ret;
}

void collada_context_free(collada_context* ctx)
{
	xml_context_free(ctx->ctx);
	free(ctx);
}
