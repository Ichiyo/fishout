#include "texture.h"
#include "m_map.h"
#include "m_array.h"
#include "m_string.h"

static m_map* texture_cache = 0;
static m_map* texture_active_ids = 0;
static m_map* texture_binding_ids = 0;
static m_array* texture_keys = 0;

texture_t* texture_new_from_file(char* file)
{
  texture_t* tex = 0;
  if(!texture_cache)
  {
    texture_cache = m_map_new(sizeof(texture_t*));
  }
  if(!texture_keys)
  {
    texture_keys = m_array_new(sizeof(m_string*));
  }
  tex = m_map_get(texture_cache, texture_t*, qckey(file));
  if(tex) goto finish;

  image_t* image = image_new_from_file(file);
  tex = texture_new_from_image(image);
  image_free(image);

  /* keep strong ref to tex */
  m_map_set(texture_cache, qckey(file), &tex);
  tex->ref++;
  m_string* key = m_string_new_from_char_array(file);
  m_array_push(texture_keys, &key);

finish:
  return tex;
}

texture_t* texture_new_from_image(image_t* image)
{
  texture_t* tex = malloc(sizeof(texture_t));
  glGenTextures(1, &tex->id);
  glBindTexture(GL_TEXTURE_2D, tex->id);
  glTexImage2D(GL_TEXTURE_2D, 0, image_get_type(image), image_get_width(image), image_get_height(image), 0, image_get_type(image), GL_UNSIGNED_BYTE, image_get_pixels(image));
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  tex->width = image_get_width(image);
  tex->height = image_get_height(image);
  tex->ref = 0;
  return tex;
}

void texture_bind(texture_t* tex, int id)
{
  /* create map from id to GL_TEXTUREi */
  if(!texture_active_ids)
  {
    int i;
    texture_active_ids = m_map_new(sizeof(GLuint));
    for(i = 0; i < 30; i++)
    {
      GLuint tid = GL_TEXTURE0 + i;
      m_map_set(texture_active_ids, qpkey(i), &tid);
    }
  }
  /* create map to remember current texture binded to GL_TEXTUREi */
  if(!texture_binding_ids)
  {
    texture_binding_ids = m_map_new(sizeof(GLuint));
  }

  /* get GL_TEXTUREi from id*/
  GLuint tid = m_map_get(texture_active_ids, GLuint, qpkey(id));

  if(m_map_contain_key(texture_binding_ids, qpkey(tid)))
  {
    /* if tex is currently binding to GL_TEXTUREi then goto finish */
    GLuint texid = m_map_get(texture_binding_ids, GLuint, qpkey(tid));
    if(texid != tex->id) goto map;
    else goto finish;
  }
  // else goto map
map:
  glActiveTexture(tid);
  glBindTexture(GL_TEXTURE_2D, tex->id);
  m_map_set(texture_binding_ids, qpkey(tid), &tex->id);
finish:
  ;
}

void texture_free(texture_t* tex)
{
  tex->ref--;
  if(tex->ref <= 0)
  {
    glDeleteTextures(1, &tex->id);
    free(tex);
  }
}

void texture_remove_unused()
{
  int i;
  for(i = 0; i < texture_keys->len; i++)
  {
    /* get key and texture */
    m_string* key = m_array_get(texture_keys, m_string*, i);
    texture_t* tex = m_map_get(texture_cache, texture_t*, qskey(key));
    if(tex->ref == 1)
    {
      /* remove unused texture */
      m_array_remove_index(texture_keys, i);
      m_map_remove_key(texture_cache, qskey(key));
      m_string_free(key);
      texture_free(tex);
      i--;
    }
  }
}

void texture_free_cache()
{
  if(texture_active_ids)
  {
    m_map_free(texture_active_ids);
  }
  if(texture_binding_ids)
  {
    m_map_free(texture_binding_ids);
  }
  if(texture_keys)
  {
    while(texture_keys->len)
    {
      /* get key and texture */
      m_string* key = m_array_get(texture_keys, m_string*, 0);
      texture_t* tex = m_map_get(texture_cache, texture_t*, qskey(key));
      /* force removing other texture references */
      tex->ref = 1;
      /* remove key and texture */
      m_array_remove_index(texture_keys, 0);
      m_map_remove_key(texture_cache, qskey(key));
      m_string_free(key);
      texture_free(tex);
    }
    m_array_free(texture_keys);
    m_map_free(texture_cache);
  }
}
