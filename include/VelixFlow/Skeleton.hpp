#ifndef SKELETON_HPP
#define SKELETON_HPP

#include <unordered_map>
#include <assimp/mesh.h>

#include "VelixFlow/DefaultMacros.hpp"

#include <glm/mat4x4.hpp>
#include <vector>


ELIX_NAMESPACE_BEGIN

class Skeleton
{
public:
    struct BoneInfo
    {
        std::string name{"Undefined"};
        int id{-1};
        glm::mat4 offsetMatrix{1.0f};
        glm::mat4 finalTransformation{1.0f};
        glm::mat4 localBindTransform{1.0f};
        glm::mat4 globalBindTransform{1.0f};
        std::vector<int> children;
        std::vector<BoneInfo*> childrenInfo;
        int parentId{-1};

        BoneInfo() = default;

        BoneInfo(const std::string& boneName, int boneId, const glm::mat4& boneOffsetMatrix, const glm::mat4& boneFinalTransformation)
        {
            name = boneName;
            id = boneId;
            offsetMatrix = boneOffsetMatrix;
            finalTransformation = boneFinalTransformation;
        }
    };

    Skeleton();
    void addBone(aiBone* bone);
    unsigned int addBone(const BoneInfo& bone);
    int getBoneId(const std::string& boneName);

    void printBonesHierarchy();

    size_t getBonesCount() const;

    bool hasBone(const std::string& boneName) const;

    BoneInfo* getBone(const std::string& boneName);
    BoneInfo* getBone(int boneID);
    BoneInfo* getParent();

    void calculateBindPoseTransforms();

    const std::vector<glm::mat4>& getBindPoses() const;

    const std::vector<glm::mat4>& getFinalMatrices();

    glm::mat4 globalInverseTransform;
private:
    bool m_isBaked{false};
    std::vector<glm::mat4> m_bindPoseTransform;
    std::unordered_map<std::string, unsigned int> m_boneMap;
    std::vector<BoneInfo> m_bonesInfo;

    std::vector<glm::mat4> m_finalBoneMatrices;
};

ELIX_NAMESPACE_END

#endif //SKELETON_HPP
