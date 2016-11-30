#include "kai.h"
#include "conv.h"

kai_t* kai_new()
{
  kai_t* kai = malloc(sizeof(kai_t));
  kai->default_shader_ctx = shader_3d_default_context_new();
  kai->actor = 0;
  return kai;
}

void kai_set_shader(kai_t* kai, shader_t* shader)
{
  kai->default_shader_ctx->shader = shader;
  shader_3d_default_context_config_shader_mesh(kai->default_shader_ctx, kai->actor->mesh);
}

void kai_render(kai_t* kai)
{
  if(kai->actor->mesh->skeleton) model_skin_join_send_gl_data(kai->actor->mesh->skeleton, kai->actor->mesh, kai->default_shader_ctx->shader);

  GLint uniModel = glGetUniformLocation(kai->default_shader_ctx->shader->id, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, kai->actor->render_model.m);

  glBindVertexArray(kai->actor->mesh->gl_data->vao);
  glDrawArrays(GL_TRIANGLES, 0, kai->actor->mesh->vertices_count);
	glBindVertexArray(0);
}

void kai_free(kai_t* k)
{
  shader_3d_default_context_free(k->default_shader_ctx);
  free(k);
}
