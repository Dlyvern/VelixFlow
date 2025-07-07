#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <assimp/matrix4x4.h>
#include <glm/fwd.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Texture.hpp"
#include <fstream>
#include <string>
#include <unordered_set>
#include <iostream>
// #include <foundation/PxTransform.h>

namespace utilities
{
    inline std::string generateUniqueName(const std::string& baseName, const std::unordered_set<std::string>& existingNames)
    {
        std::string candidate = baseName;
        int counter = 1;

        while (existingNames.contains(candidate))
            candidate = baseName + "_" + std::to_string(counter++);

        return candidate;
    }

    inline size_t getRamUsage()
    {
        std::ifstream status_file("/proc/self/status");
        std::string line;

        while (std::getline(status_file, line))
        {
            // std::cout << line << std::endl;

            if (line.starts_with("VmRSS"))
            {
                line = line.substr(line.find("VmRSS") + 9);

                // std::cout << line << std::endl;

                line = line.substr(0, line.find("kB") - 1);

                size_t ramUsage = std::stoi(line);

                // std::cout << ramUsage;

                return ramUsage / 1024;
            }

            // if (line.rfind("VmRSS:", 0) == 0)
            // {
            //
            // }

            // if (line.rfind("VmRSS:", 0) == 0)
            // {
            //     std::istringstream iss(line);
            //     std::string key, value, unit;
            //     iss >> key >> value >> unit;
            //     return std::stoul(value) / 1024;
            // }
        }

        return 0;
    }

    inline std::string fromTypeToString(const elix::Texture::TextureType& type)
    {
        switch (type)
        {
            case elix::Texture::TextureType::Diffuse:
                return "Diffuse";
            case elix::Texture::TextureType::Specular:
                return "Specular";
            case elix::Texture::TextureType::Normal:
                return "Normal";
            case elix::Texture::TextureType::Metallic:
                return "Metallic";
            case elix::Texture::TextureType::Roughness:
                return "Roughness";
            case elix::Texture::TextureType::AO:
                return "AO";
            case elix::Texture::TextureType::Emissive:
                return "Emissive";
            case elix::Texture::TextureType::Height:
                return "Height";
            case elix::Texture::TextureType::Glossiness:
                return "Glossiness";
            case elix::Texture::TextureType::Opacity:
                return "Opacity";
            default:
                return "Undefined";
        }
    }

    inline elix::Texture::TextureType fromStringToTextureType(const std::string& type)
    {
        if (type == "Diffuse")
            return elix::Texture::TextureType::Diffuse;
        if (type == "Specular")
            return elix::Texture::TextureType::Specular;
        if (type == "Normal")
            return elix::Texture::TextureType::Normal;
        if (type == "Metallic")
            return elix::Texture::TextureType::Metallic;
        if (type == "Roughness")
            return elix::Texture::TextureType::Roughness;
        if (type == "AO")
            return elix::Texture::TextureType::AO;
        if (type == "Emissive")
            return elix::Texture::TextureType::Emissive;
        if (type == "Height")
            return elix::Texture::TextureType::Height;
        if (type == "Glossiness")
            return elix::Texture::TextureType::Glossiness;
        if (type == "Opacity")
            return elix::Texture::TextureType::Opacity;
        return elix::Texture::TextureType::Undefined;
    }

    inline glm::vec3 interpolate(const glm::vec3& start, const glm::vec3& end, float t) {
        return start + t * (end - start);
    }

    inline glm::quat interpolate(const glm::quat& start, const glm::quat& end, float t) {
        return glm::slerp(start, end, t);
    }

    inline float interpolate(float start, float end, float t) {
        return start + t * (end - start);
    }

    inline std::pair<const common::SQT*, const common::SQT*> findKeyframes(const std::vector<common::SQT>& keyFrames, const float currentTime)
    {
        if (keyFrames.empty())
            return {nullptr, nullptr};

        if (currentTime <= keyFrames.front().timeStamp)
            return {&keyFrames.front(), &keyFrames.front()};

        if (currentTime >= keyFrames.back().timeStamp)
            return {&keyFrames.back(), &keyFrames.back()};

        for (size_t i = 1; i < keyFrames.size(); ++i)
            if (currentTime < keyFrames[i].timeStamp)
                return {&keyFrames[i - 1], &keyFrames[i]};

        return {nullptr, nullptr};  // Should never reach here
    }

    inline glm::mat4 convertMatrixToGLMFormat(const aiMatrix4x4& from)
    {
        glm::mat4 to;
        //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
        to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
        to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
        to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
        to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
        return to;
    }

    // inline physx::PxTransform glmMat4ToPxTransform(const glm::mat4& mat)
    // {
    //     glm::vec3 pos = glm::vec3(mat[3]);
    //     glm::quat rot = glm::quat_cast(mat);
    //     return physx::PxTransform(physx::PxVec3(pos.x, pos.y, pos.z),
    //                               physx::PxQuat(rot.x, rot.y, rot.z, rot.w));
    // }
    //
    // inline glm::mat4 pxTransformToGlmMat4(const physx::PxTransform& transform) {
    //     glm::quat q(transform.q.w, transform.q.x, transform.q.y, transform.q.z);
    //     glm::mat4 rotation = glm::toMat4(q);
    //     rotation[3] = glm::vec4(transform.p.x, transform.p.y, transform.p.z, 1.0f);
    //     return rotation;
    // }
} //namespace utilities

#endif //UTILITIES_HPP