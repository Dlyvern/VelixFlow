#ifndef MODEL_HPP
#define MODEL_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/Skeleton.hpp"
#include "VelixFlow/Animation.hpp"

#include "VelixFlow/RenderAPI/Interface/IMesh.hpp"

#include <memory>

ELIX_NAMESPACE_BEGIN

class Model
{
public:
    Model(const std::string& name, const std::vector<std::shared_ptr<mesh::IMesh>>& meshes, std::unique_ptr<Skeleton> skeleton = nullptr);

    void addAnimation(animation::Animation* animation);

    [[nodiscard]] animation::Animation* getAnimation(int index) const;
    [[nodiscard]] animation::Animation* getAnimation(const std::string& name) const;
    [[nodiscard]] const std::vector<animation::Animation*>& getAnimations() const;
    [[nodiscard]] Skeleton* getSkeleton() const;
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] size_t getNumMeshes() const;
    [[nodiscard]] std::shared_ptr<mesh::IMesh> getMesh(int meshIndex);
    [[nodiscard]] const std::vector<std::shared_ptr<mesh::IMesh>>& getMeshes() const;
    [[nodiscard]] bool hasSkeleton() const;
private:
    std::string m_name;
    std::vector<std::shared_ptr<mesh::IMesh>> m_meshes;
    std::unique_ptr<Skeleton> m_skeleton{nullptr};
    std::vector<animation::Animation*> m_animations;
};

ELIX_NAMESPACE_END

#endif //MODEL_HPP