#include "ShadowSystem.hpp"
#include <glad/glad.h>
#include "Light.hpp"
#include "Window.hpp"
#include "Logger.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>


void elix::ShadowSystem::bindShadowPass(lighting::Light* light, unsigned int textureSlot)
{
    if(!light || !m_shadowData.contains(light->id))
    {   
                ELIX_LOG_ERROR("Could not bind invalid light");
                return;
    }

    auto& shadowData = m_shadowData[light->id];

    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_2D, shadowData.shadowMap);
}

elix::ShadowSystem::Shadow elix::ShadowSystem::createShadowFramebuffer()
{
	unsigned int fbo;
	unsigned int textureMap;

	glGenFramebuffers(1, &fbo);
	glGenTextures(1, &textureMap);
	glBindTexture(GL_TEXTURE_2D, textureMap);

    int resolution = getResolutionBasedOnQuality(m_quality);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureMap, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        ELIX_LOG_ERROR("Shadow buffer is not complete");
        glDeleteFramebuffers(1, &fbo);
        glDeleteTextures(1, &textureMap);
        return {};
    }

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Shadow shadow;

    shadow.fbo = fbo;
    shadow.shadowMap = textureMap;

    return shadow;
}

void elix::ShadowSystem::cleanup()
{
    for (auto& [id, shadow] : m_shadowData)
    {
        glDeleteFramebuffers(1, &shadow.fbo);
        glDeleteTextures(1, &shadow.shadowMap);
    }
    
    m_shadowData.clear();
}

glm::mat4 elix::ShadowSystem::calculateSpotLightMatrix(lighting::Light* spotLight)
{
    float aspectRatio{1.0f};
    float angleRadians = acos(spotLight->cutoff);
    float fov = angleRadians * 2.0f;
    glm::mat4 lightProjection = glm::perspective(fov, aspectRatio, 0.1f, 100.0f);

    glm::vec3 up = glm::abs(glm::dot(glm::normalize(spotLight->direction), glm::vec3(0, 1, 0))) > 0.099f ? glm::vec3(0, 0, 1) : glm::vec3(0, 1, 0);

    glm::mat4 lightView = glm::lookAt(spotLight->position, spotLight->position + spotLight->direction, up);

    glm::mat4 lightMatrix = lightProjection * lightView;

    return lightMatrix;
}


glm::mat4 elix::ShadowSystem::calculateDirectionalLightMatrix(lighting::Light* light)
{
//  glm::mat4 invView = glm::inverse(viewMatrix);

//    glm::vec3 frustumCorners[8] = {
//    glm::vec3(-1.0f, 1.0f, -1.0f),
//    glm::vec3(1.0f, 1.0f, -1.0f),
//    glm::vec3(1.0f, -1.0f, -1.0f),
//    glm::vec3(-1.0f, -1.0f, -1.0f),
//    glm::vec3(-1.0f, 1.0f, 1.0f),
//    glm::vec3(1.0f, 1.0f, 1.0f),
//    glm::vec3(1.0f, -1.0f, 1.0f),
//    glm::vec3(-1.0f, -1.0f, 1.0f),
//    };
  
   // for(auto& corner : frustumCorners)
   // {
       // glm::vec4 worldCorner = invView * glm::vec4(corner, 1.0f);
       // corner = glm::vec3(worldCorner) / worldCorner.w;
   // }

    glm::vec3 lightDirection = glm::normalize(light->direction);

    glm::vec3 lightTarget{0.0f};

    glm::vec3 lightPosition = lightTarget - lightDirection * 20.0f;

    glm::mat4 lightView = glm::lookAt(lightPosition, lightTarget, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 1.0f, 40.0f);

    glm::mat4 lightMatrix = lightProjection * lightView;

    return lightMatrix;
}

const elix::ShadowSystem::Shadow& elix::ShadowSystem::getShadowData(lighting::Light* light) const
{
    return m_shadowData.at(light->id);
}

void elix::ShadowSystem::beginShadowPass(lighting::Light* light)
{
    if(!light || !m_shadowData.contains(light->id))
    {
        ELIX_LOG_ERROR("Failed to begin shadow pass with invalid light");
        return;
    }

    auto& shadowData = m_shadowData[light->id];

    glBindFramebuffer(GL_FRAMEBUFFER, shadowData.fbo);

    int resolution = getResolutionBasedOnQuality(m_quality);

    glViewport(0, 0, resolution, resolution);

    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT);

    // window::MainWindow::setViewport(0, 0, resolution, resolution);

    window::Window::clear(window::ClearFlag::DEPTH_BUFFER_BIT);

    // window::MainWindow::setCullMode(window::CullMode::FRONT);
}

void elix::ShadowSystem::endShadowPass()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_CULL_FACE);

    // window::MainWindow::setCullMode(window::CullMode::BACK);

    window::Window::clear(window::ClearFlag::COLOR_BUFFER_BIT | window::ClearFlag::DEPTH_BUFFER_BIT);
}

elix::ShadowSystem::Shadow elix::ShadowSystem::createShadowCubeFramebuffer()
{
    unsigned int fbo;
    unsigned int shadowMap;


    glGenFramebuffers(1, &fbo);
    glGenTextures(1, &shadowMap);

    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);

    int resolution = getResolutionBasedOnQuality(m_quality);

    for(unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
   
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMap, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        ELIX_LOG_ERROR("Shadow buffer is not complete");
        glDeleteFramebuffers(1, &fbo);
        glDeleteTextures(1, &shadowMap);
        return {};
    }

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Shadow shadow;
    shadow.fbo = fbo;
    shadow.shadowMap = shadowMap;

    return shadow;
}

int elix::ShadowSystem::getResolutionBasedOnQuality(elix::ShadowSystem::ShadowQuality quality)
{
    switch(quality)
    {
        case elix::ShadowSystem::ShadowQuality::LOW: return 512;
        case elix::ShadowSystem::ShadowQuality::MEDIUM: return 1024;
        case elix::ShadowSystem::ShadowQuality::HIGH: return 2048;
        case elix::ShadowSystem::ShadowQuality::ULTRA: return 4096;
    }

    return 0;
}

glm::mat4 elix::ShadowSystem::getLightMatrix(lighting::Light* light) const
{
    if(!light || !m_shadowData.contains(light->id))
        return {};

    return m_shadowData.at(light->id).lightMatrix;
}

void elix::ShadowSystem::updateLightMatrix(lighting::Light* light)
{
    if(!light)
        return;

    if(!light || !m_shadowData.contains(light->id))
    {
        ELIX_LOG_ERROR("Failed to update light matrix with invalid light");
        return;
    }

    elix::ShadowSystem::Shadow& shadowData = m_shadowData[light->id];

    if(light->type == lighting::LightType::DIRECTIONAL)
        shadowData.lightMatrix = calculateDirectionalLightMatrix(light);
    else if(light->type == lighting::LightType::SPOT)
        shadowData.lightMatrix = calculateSpotLightMatrix(light);
}

void elix::ShadowSystem::init(const std::vector<lighting::Light*>& lights, ShadowQuality quality)
{
    m_quality = quality;

    for(const auto& light : lights)
    {
        Shadow newShadow;

        if(light->type == lighting::LightType::POINT)
            newShadow = createShadowCubeFramebuffer();
        else
            newShadow = createShadowFramebuffer();

        m_shadowData[light->id] = newShadow;
        updateLightMatrix(light);
        ELIX_LOG_INFO("Inittialized shadow with light id: ", light->id);
    }

    ELIX_LOG_INFO("Intialized shadows with ", lights.size(), " lights");
 }
