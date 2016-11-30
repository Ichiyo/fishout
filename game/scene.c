#include "scene.h"
#include <stdlib.h>

scene_t* scene_new()
{
  scene_t* scene = malloc(sizeof(scene_t));
  scene->material_ctx = 0;
  return scene;
}

void scene_render(scene_t* scene)
{
  
}

void scene_free(scene_t* scene)
{
  if(scene->material_ctx) scene_material_context_free(scene->material_ctx);
  free(scene);
}
