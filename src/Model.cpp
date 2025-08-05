#include "VelixFlow/Model.hpp"

ELIX_NAMESPACE_BEGIN

Model::Model(const std::string &name, const std::vector<std::shared_ptr<mesh::IMesh>> &meshes, std::unique_ptr<Skeleton> skeleton): m_name(name), m_meshes(meshes)
{
    if (skeleton)
    {
        m_skeleton = std::move(skeleton);
        m_skeleton->calculateBindPoseTransforms();
    }
}

const std::vector<std::shared_ptr<mesh::IMesh>>& Model::getMeshes() const
{
    return m_meshes;
}

void Model::addAnimation(animation::Animation* animation)
{
    m_animations.push_back(animation);
}

animation::Animation* Model::getAnimation(const int index) const
{
    return m_animations[index];
}

animation::Animation* Model::getAnimation(const std::string &name) const
{
    for (auto& animation : m_animations)
        if (name == animation->name)
            return animation;

    return nullptr;
}

const std::vector<animation::Animation*> & Model::getAnimations() const
{
    return m_animations;
}

Skeleton* Model::getSkeleton() const
{
    return m_skeleton.get();
}

std::string Model::getName() const
{
    return m_name;
}

size_t Model::getNumMeshes() const
{
    return m_meshes.size();
}

std::shared_ptr<mesh::IMesh> Model::getMesh(const int meshIndex)
{
    if(meshIndex < 0 || meshIndex > m_meshes.size())
        return nullptr;
    
    return m_meshes[meshIndex];
}

bool Model::hasSkeleton() const
{
    return m_skeleton != nullptr;
}

ELIX_NAMESPACE_END