#ifndef __M_MODEL_H__
#define __M_MODEL_H__

#include "m_array.h"
#include "graphic.h"
#include "g_math.h"
#include "xml.h"
#include "m_string.h"
#include "shader.h"

typedef struct collada_context
{
	xml_context* ctx;
} collada_context;

typedef struct model_skin_join
{
	m_string* sid;
	m_string* uniform;

	m_array* children;
	struct model_skin_join* parent;

 	/* reference count, multiple model_meshs can hold the same skeleton */
	int ref;

	m_matrix4 bind_pose;
	m_matrix4 inverse_bind_pose;
	m_matrix4 world_matrix;
	m_matrix4 final_matrix;
} model_skin_join;

typedef model_skin_join model_skin_join_t;

typedef struct model_mesh_gl_data_t
{
	GLuint vao;
	GLuint vbo;
	int ref;
} model_mesh_gl_data_t;

typedef struct model_mesh
{
	model_mesh_gl_data_t* gl_data;
	m_string* name;
	m_matrix4 bind_shape_matrix;
	model_skin_join* skeleton;
	int vertices_count;
	int has_uvs;
	int joins_per_vertex;
	int join_count;
	/* store self sub meshs*/
	m_array* sub_meshs;
	/* weak ref to parent */
	struct model_mesh* parent;
} model_mesh;

typedef model_mesh model_mesh_t;

collada_context* collada_context_new(char* file);
/* parse xml file to array of model meshs */
m_array* collada_context_parse(collada_context* context);
void collada_context_free(collada_context* context);
model_mesh* model_mesh_clone(model_mesh* mesh);
void model_skin_join_send_gl_data(model_skin_join_t* join, model_mesh_t* mesh, shader_t* shader);
void model_mesh_free(model_mesh* mesh);

#endif
