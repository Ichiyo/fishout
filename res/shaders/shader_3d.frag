#version 100

#ifdef GL_ES

precision mediump float;
#define plf_in varying
#define plf_out
#define plf_texture texture2D
#define plf_fragColor gl_FragColor

#else

#define plf_in in
#define plf_out out
#define plf_texture texture
out vec4 fragColor;
#define plf_fragColor fragColor

#endif

#define HAS_TEXTURE _
#define NR_DIRECTION_LIGHTS __
#define NR_POINT_LIGHTS __
#define NR_SPOT_LIGHTS __
#define HAS_SHADOW _

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float     shininess;
};

struct DirLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct SpotLight {
  vec3 position;
  vec3 direction;
  float cutOff;
  float outerCutOff;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

#if HAS_TEXTURE != 0
plf_in vec2 Texcoord;
#endif
plf_in vec3 Normal;
plf_in vec3 FragPos;

#if HAS_SHADOW != 0
//plf_in vec2 v_shadow_frag[9];
plf_in vec3 v_fragPosLightSpace;

uniform sampler2D shadowMap;
#endif

#if NR_DIRECTION_LIGHTS != 0
uniform DirLight dirLights[NR_DIRECTION_LIGHTS];
#endif
#if NR_POINT_LIGHTS != 0
uniform PointLight pointLights[NR_POINT_LIGHTS];
#endif
#if NR_SPOT_LIGHTS != 0
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
#endif

uniform Material material;
uniform vec3 view_position;

// function prototype
vec3 CalcDirLight(DirLight ulight, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight ulight, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight ulight, vec3 normal, vec3 fragPos, vec3 viewDir);
float ShadowCalculation(vec3 fragPosLightSpace, vec3 lightDir);

void main()
{
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(view_position - FragPos);

  vec3 result = vec3(0.0, 0.0, 0.0);

  #if NR_DIRECTION_LIGHTS != 0
  for(int i = 0; i < NR_DIRECTION_LIGHTS; i++)
  {
      result += CalcDirLight(dirLights[i], norm, viewDir);
  }
  #endif

  #if NR_POINT_LIGHTS != 0
  for(int i = 0; i < NR_POINT_LIGHTS; i++)
  {
      result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
  }
  #endif

  #if NR_SPOT_LIGHTS != 0
  for(int i = 0; i < NR_SPOT_LIGHTS; i++)
  {
      result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
  }
  #endif

  plf_fragColor =  vec4(result, 1.0);
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight ulight, vec3 normal, vec3 viewDir)
{
#if HAS_TEXTURE != 0
   vec3 color = vec3(texture2D(material.diffuse, Texcoord));
   vec3 spec_color = vec3(texture2D(material.specular, Texcoord));
#else
   vec3 color = vec3(1.0, 1.0, 1.0);
   vec3 spec_color = vec3(0.0, 0.0, 0.0);
#endif
   vec3 lightDir = normalize(ulight.direction - FragPos);

   // Diffuse shading
   float diff = max(dot(normal, lightDir), 0.0);

   // Specular shading
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

   // shadow
#if HAS_SHADOW != 0
   float shadow = ShadowCalculation(v_fragPosLightSpace, lightDir);
   shadow = min(shadow, 0.75);
#else
   float shadow = 0.0;
#endif

   // Combine results
   vec3 ambient = ulight.ambient * color;
   vec3 diffuse = ulight.diffuse * diff * color;
   vec3 specular = ulight.specular * spec * spec_color;
   return (ambient + (1.0 - shadow) * (diffuse + specular));
}

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight ulight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(ulight.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Attenuation
    float distance = length(ulight.position - fragPos);
    float attenuation = 1.0 / (ulight.constant + ulight.linear * distance + ulight.quadratic * (distance * distance));
    // Combine results
#if HAS_TEXTURE != 0
    vec3 ambient = ulight.ambient * vec3(texture2D(material.diffuse, Texcoord));
    vec3 diffuse = ulight.diffuse * diff * vec3(texture2D(material.diffuse, Texcoord));
    vec3 specular = ulight.specular * spec * vec3(texture2D(material.specular, Texcoord));
#else
    vec3 ambient = ulight.ambient;
    vec3 diffuse = ulight.diffuse * diff;
    vec3 specular = ulight.specular * spec;
#endif
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// Calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight ulight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(ulight.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Attenuation
    float distance = length(ulight.position - fragPos);
    float attenuation = 1.0 / (ulight.constant + ulight.linear * distance + ulight.quadratic * (distance * distance));
    // Spotlight intensity
    float theta = dot(lightDir, normalize(-ulight.direction));
    float epsilon = ulight.cutOff - ulight.outerCutOff;
    float intensity = clamp((theta - ulight.outerCutOff) / epsilon, 0.0, 1.0);
    // Combine results
#if HAS_TEXTURE != 0
    vec3 ambient = ulight.ambient * vec3(texture2D(material.diffuse, Texcoord));
    vec3 diffuse = ulight.diffuse * diff * vec3(texture2D(material.diffuse, Texcoord));
    vec3 specular = ulight.specular * spec * vec3(texture2D(material.specular, Texcoord));
#else
    vec3 ambient = ulight.ambient;
    vec3 diffuse = ulight.diffuse * diff;
    vec3 specular = ulight.specular * spec;
#endif
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

#if HAS_SHADOW != 0
float ShadowCalculation(vec3 fragPosLightSpace, vec3 lightDir)
{
    vec3 normal = normalize(Normal);

    // perform perspective divide
    // vec3 projCoords = fragPosLightSpace;
    // vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    // projCoords = projCoords * 0.5 + 0.5;

    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)

    float closestDepth = texture2D(shadowMap, fragPosLightSpace.xy).r;
    // Get depth of current fragment from light's perspective
    float currentDepth = fragPosLightSpace.z;
    // Check whether current frag pos is in shadow
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

//  float shadow = 0.0;
//  vec2 texelSize = vec2(1.0 / 1024.0);
//  for(int x = -1; x <= 1; ++x)
//  {
//    for(int y = -1; y <= 1; ++y)
//    {
//      float pcfDepth = texture2D(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
//      shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
//     }
//  }
//  for(int i = 0; i < 9; i++)
//  {
//    float pcfDepth = texture2D(shadowMap, v_shadow_frag[i]).r;
//    shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
//  }
//  shadow /= 9.0;
//
// // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
// if(projCoords.z > 1.0)
//     shadow = 0.0;

    return shadow;
}
#endif
