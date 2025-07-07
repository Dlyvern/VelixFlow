#include "Model.hpp"
#include "ShaderManager.hpp"

elix::Model::Model(const std::string &name, const std::vector<elix::Mesh> &meshes, std::unique_ptr<Skeleton> skeleton): m_name(name), m_meshes(meshes)
{
    if (skeleton)
    {
        m_skeleton = std::move(skeleton);
        m_skeleton->calculateBindPoseTransforms();
    }
}

const std::vector<elix::Mesh>& elix::Model::getMeshes() const
{
    return m_meshes;
}

void elix::Model::addAnimation(common::Animation *animation)
{
    m_animations.push_back(animation);
}

common::Animation* elix::Model::getAnimation(const int index) const
{
    return m_animations[index];
}

common::Animation * elix::Model::getAnimation(const std::string &name) const
{
    for (auto& animation : m_animations)
        if (name == animation->name)
            return animation;

    return nullptr;
}

const std::vector<common::Animation *> & elix::Model::getAnimations() const
{
    return m_animations;
}

Skeleton * elix::Model::getSkeleton() const
{
    return m_skeleton.get();
}

void elix::Model::draw() const
{
    for (auto& mesh : m_meshes)
        mesh.draw();
}

std::string elix::Model::getName() const
{
    return m_name;
}

size_t elix::Model::getNumMeshes() const
{
    return m_meshes.size();
}

elix::Mesh * elix::Model::getMesh(const int meshIndex)
{
    return &m_meshes[meshIndex];
}

bool elix::Model::hasSkeleton() const
{
    return m_skeleton != nullptr;
}
