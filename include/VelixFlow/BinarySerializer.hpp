#ifndef BINARY_SERIALIZER_HPP
#define BINARY_SERIALIZER_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/BinaryLoader.hpp"
#include "VelixFlow/Model.hpp"
#include "VelixFlow/Assets.hpp"

#include <memory>
#include <iostream>

ELIX_NAMESPACE_BEGIN

class BinarySerializer
{
public:
    void serializeModel(BinaryLoader& writer, const Model& model);
    std::unique_ptr<AssetModel> deserializeModel(BinaryLoader& reader);

    // void serializeTexture(BinaryLoader& writer, const Texture& texture);

    bool writeElixPacket(const std::string& filePath, const std::vector<elix::AssetModel*>& models);

    bool readElixPacket(const std::string& filePath, std::vector<std::unique_ptr<AssetModel>>& outModels);
private:
    struct ElixPacketHeader
    {
        char magic[4] = {'E', 'L', 'X', 'P'};
        uint32_t version = 1;

        uint32_t modelCount = 0;
        uint32_t textureCount = 0;
        uint32_t materialCount = 0;

        uint32_t modelOffset = 0;
        uint32_t textureOffset = 0;
        uint32_t materialOffset = 0;

        uint64_t modelsTableOffset = 0;
    };

    struct ModelEntry
    {
        uint64_t offset;
        uint64_t size;
        char name[64];
    };
};

ELIX_NAMESPACE_END

#endif //BINARY_SERIALIZER_HPP
