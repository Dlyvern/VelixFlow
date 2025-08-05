#version 450

#define MAX_LIGHTS 4

layout (location = 0) in vec3 FragPos;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoords;
layout (location = 3) in vec4 FragPosLightSpace[MAX_LIGHTS];

layout(location = 0) out vec4 FragColor;

const int LIGHT_TYPE_DIRECTIONAL = 0;
const int LIGHT_TYPE_POINT       = 1;
const int LIGHT_TYPE_SPOT        = 2;

struct Light {
    vec3 position;
    float strength;
    vec3 color;
    float radius;
    vec3 direction;
    float cutoff;
    float outerCutoff;
    int type;
    int castShadows;
    float farPlane;
};


layout(set = 0, binding = 0) uniform UBO
{
    vec3 viewPos;
    Light lights[MAX_LIGHTS];
} ubo;

layout(set = 1, binding = 0) uniform sampler2D shadowMaps[MAX_LIGHTS];
layout(set = 1, binding = 4) uniform samplerCube pointShadowMaps[MAX_LIGHTS];


layout(set = 1, binding = 8) uniform sampler2D u_Diffuse;
layout(set = 1, binding = 9) uniform sampler2D u_Normal;
layout(set = 1, binding = 10) uniform sampler2D u_Metallic;
layout(set = 1, binding = 11) uniform sampler2D u_Roughness;
layout(set = 1, binding = 12) uniform sampler2D u_AO;


layout(set = 1, binding = 13) uniform UseMaps
{
    bool use_Diffuse;
    bool use_Normal;
    bool use_Metallic;
    bool use_Roughness;
    bool use_AO;
} useMaps;

layout(set = 0, binding = 1) uniform BaseColor
{
    vec3 baseColor;
}  baseColor;


float PointLightShadow(vec3 fragPos, Light light, samplerCube depthMap)
{
    vec3 fragToLight = fragPos - light.position;
    float currentDepth = length(fragToLight);
    float shadow = 0.0;
    float bias = 0.05;
    int samples = 20;
    float offset = 0.15;

    for(int i = 0; i < samples; ++i)
    {
        vec3 sampleOffsetDir = normalize(fragToLight) + offset * normalize(vec3(
            fract(sin(float(i) * 12.9898) * 43758.5453),
            fract(cos(float(i) * 78.233) * 12345.6789),
            fract(sin(float(i) * 91.5321) * 4321.8765)
        ));
        float closestDepth = texture(depthMap, sampleOffsetDir).r * light.farPlane;
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }

    shadow /= float(samples);
    return shadow;
    }

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, sampler2D shadowMapToUse)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0) return 0.0;

    float closestDepth = texture(shadowMapToUse, projCoords.xy).r;
    float currentDepth = projCoords.z;

    vec3 normal = normalize(Normal);

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMapToUse, 0);

    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMapToUse, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }

    shadow /= 9.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

vec3 getAlbedo()
{
    return useMaps.use_Diffuse ? texture(u_Diffuse, TexCoords).rgb : baseColor.baseColor;
}

float getMetallic()
{
    return useMaps.use_Metallic ? texture(u_Metallic, TexCoords).r : 0.0;
}

float getRoughness()
{
    return useMaps.use_Roughness ? texture(u_Roughness, TexCoords).r : 1.0;
}

float getAO()
{
    return useMaps.use_AO ? texture(u_AO, TexCoords).r : 1.0;
}

vec3 getNormal()
{
    return normalize(Normal);
}

vec3 getViewDir(vec3 fragPos)
{
    return normalize(ubo.viewPos - fragPos);
}

float getSpecular(vec3 normal, vec3 lightDir, vec3 viewDir, float roughness)
{
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float shininess = mix(8.0, 128.0, 1.0 - roughness);
    return pow(max(dot(normal, halfwayDir), 0.0), shininess);
}

vec3 calculateDirectionalLight(Light light, vec3 albedo, float roughness, float metallic, float ao, int index)
{
    vec3 normal = getNormal();
    vec3 lightDir = normalize(-light.direction);
    vec3 viewDir = getViewDir(FragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = getSpecular(normal, lightDir, viewDir, roughness);

    vec3 ambient = 0.03 * albedo * ao;
    vec3 diffuse = diff * albedo * light.color * light.strength;
    vec3 specular = spec * mix(vec3(0.04), albedo, metallic) * light.strength;

    float shadow = ShadowCalculation(FragPosLightSpace[index], lightDir, shadowMaps[index]);
    diffuse *= (1.0 - shadow);
    specular *= (1.0 - shadow);

    return ambient + diffuse + specular;
}

vec3 calculateSpotLight(Light light, vec3 albedo, float roughness, float metallic, float ao, int index)
{
    vec3 normal = getNormal();
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 viewDir = getViewDir(FragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = getSpecular(normal, lightDir, viewDir, roughness);

    vec3 ambient = 0.03 * albedo * ao;
    vec3 diffuse = diff * albedo * light.color * light.strength;
    vec3 specular = spec * mix(vec3(0.04), albedo, metallic) * light.strength;

    float theta = dot(normalize(FragPos - light.position), normalize(light.direction));

    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    diffuse *= intensity;
    specular *= intensity;

    float shadow = ShadowCalculation(FragPosLightSpace[index], lightDir, shadowMaps[index]);
    diffuse *= (1.0 - shadow);
    specular *= (1.0 - shadow);

    return ambient + diffuse + specular;
}

vec3 calculatePointLight(Light light, vec3 albedo, float roughness, float metallic, float ao, int index)
{
    vec3 normal = getNormal();
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 viewDir = getViewDir(FragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = getSpecular(normal, lightDir, viewDir, roughness);

    vec3 ambient = 0.03 * albedo * ao;
    vec3 diffuse = diff * albedo * light.color * light.strength;
    vec3 specular = spec * mix(vec3(0.04), albedo, metallic) * light.strength;

    float distance = length(light.position - FragPos);
    float attenuation = clamp(1.0 - (distance / light.radius), 0.0, 1.0);

    diffuse *= attenuation;
    specular *= attenuation;

    float shadow = PointLightShadow(FragPos, light, pointShadowMaps[index]);
    // diffuse *= (1.0 - shadow);
    // specular *= (1.0 - shadow);

    return ambient + diffuse + specular;
}

void main()
{
    vec3 albedo     = getAlbedo();
    float metallic  = getMetallic();
    float roughness = getRoughness();
    float ao        = getAO();

    vec3 result = vec3(0.0);

    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        Light lightV = ubo.lights[i];

        if (lightV.type == LIGHT_TYPE_DIRECTIONAL)
            result += calculateDirectionalLight(lightV, albedo, roughness, metallic, ao, i);
        else if (lightV.type == LIGHT_TYPE_SPOT)
            result += calculateSpotLight(lightV, albedo, roughness, metallic, ao, i);
        else if (lightV.type == LIGHT_TYPE_POINT)
            result += calculatePointLight(lightV, albedo, roughness, metallic, ao, i);
    }

    FragColor = vec4(result, 1.0);
}
