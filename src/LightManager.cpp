#include "LightManager.hpp"

LightManager& LightManager::instance()
{
    static LightManager instance;
    return instance;
}

void LightManager::addLight(lighting::Light* light)
{
    light->id = m_lights.size();
    m_lights.push_back(light);
}

void LightManager::removeLight(lighting::Light *light)
{
    auto it = std::find(m_lights.begin(), m_lights.end(), light);

    if (it != m_lights.end())
        m_lights.erase(it);
}

std::vector<lighting::Light*> LightManager::getLights() const
{
    return m_lights;
}

lighting::Light* LightManager::getDirectionalLight() const
{
    auto it = std::find_if(m_lights.begin(), m_lights.end(), [](const lighting::Light* light) {return light->type == lighting::LightType::DIRECTIONAL; });

    if(it != m_lights.end())  return *it;
	
    return nullptr;
}

std::vector<lighting::Light*> LightManager::getSpotLights() const
{
    std::vector<lighting::Light*> spotLights;

    for (const auto& light : m_lights)
        if (light->type == lighting::LightType::SPOT)
            spotLights.push_back(light);

    return spotLights;
}

std::vector<lighting::Light*> LightManager::getPointLights() const
{
    std::vector<lighting::Light*> pointLights;

    for (const auto& light : m_lights)
        if (light->type == lighting::LightType::POINT)
            pointLights.push_back(light);

    return pointLights;
}

void LightManager::sendLightsIntoShader(const elix::Shader &shader) const
{
    for (size_t i = 0; i < m_lights.size(); ++i)
    {
        const lighting::Light* light = m_lights[i];
        shader.setInt("lights[" + std::to_string(i) + "].type", static_cast<int>(light->type));
        shader.setVec3("lights[" + std::to_string(i) + "].position", light->position);
        shader.setVec3("lights[" + std::to_string(i) + "].color", light->color);
        shader.setFloat("lights[" + std::to_string(i) + "].strength", light->strength);
        shader.setFloat("lights[" + std::to_string(i) + "].radius", light->radius);
        shader.setVec3("lights[" + std::to_string(i) + "].direction", light->direction);
        shader.setFloat("lights[" + std::to_string(i) + "].cutoff", light->cutoff);
        shader.setFloat("lights[" + std::to_string(i) + "].outerCutoff", light->outerCutoff);
    }
}

