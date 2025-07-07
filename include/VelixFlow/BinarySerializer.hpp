#ifndef BINARY_SERIALIZER_HPP
#define BINARY_SERIALIZER_HPP

#include "DefaultMacros.hpp"
#include <iostream>
#include "BinaryLoader.hpp"
#include "Model.hpp"
#include "Assets.hpp"
#include <memory>

ELIX_NAMESPACE_BEGIN

struct ElixPacketHeader
{
    char magic[8] = {'E', 'L', 'I', 'X', 'P', 'K', 'T', '\0'};
    uint32_t version = 1;
    uint32_t modelCount = 0;
    uint64_t modelsTableOffset = 0;
};

struct ModelEntry
{
    uint64_t offset;
    uint64_t size;
    char name[64];
};

class BinarySerializer
{
public:
    void serializeModel(BinaryLoader& writer, const Model& model)
    {
        char nameBuffer[64] = {0};
        std::strncpy(nameBuffer, model.getName().c_str(), sizeof(nameBuffer)-1);
        writer.write(nameBuffer);

        uint32_t meshCount = static_cast<uint32_t>(model.getMeshes().size());
        writer.write(meshCount);

        for (const auto& mesh : model.getMeshes())
        {
            uint32_t vertexCount = static_cast<uint32_t>(mesh.getVertices().size());
            writer.write(vertexCount);

            writer.write(mesh.getVertices());

            uint32_t indexCount = static_cast<uint32_t>(mesh.getIndices().size());
            writer.write(indexCount);

            writer.write(mesh.getIndices());
        }
    }

    std::unique_ptr<AssetModel> deserializeModel(BinaryLoader& reader)
    {
        char nameBuffer[64] = {};
        reader.read(nameBuffer);
        const std::string modelName = std::string(nameBuffer);

        uint32_t meshCount = 0;
        reader.read(meshCount);

        std::vector<elix::Mesh> meshes;
        meshes.reserve(meshCount);

        for (uint32_t i = 0; i < meshCount; ++i)
        {
            std::vector<common::Vertex> vertices;
            std::vector<unsigned int> indices;

            uint32_t vertexCount = 0;
            reader.read(vertexCount);

            vertices.resize(vertexCount);
            reader.read(vertices);

            uint32_t indexCount = 0;
            reader.read(indexCount);

            indices.resize(indexCount);
            reader.read(indices);

            elix::Mesh mesh(vertices, indices);

            meshes.push_back(std::move(mesh));
        }

        std::unique_ptr<elix::Model> model{nullptr};

        model = std::make_unique<elix::Model>(modelName, std::move(meshes));

        return std::make_unique<elix::AssetModel>(std::move(model));
    }

    bool writeElixPacket(const std::string& filePath, const std::vector<elix::AssetModel*>& models)
    {
        BinaryLoader writer;

        if (!writer.init(filePath, BinaryLoader::Mode::Write))
            return false;

        ElixPacketHeader header;
        header.modelCount = static_cast<uint32_t>(models.size());

        writer.write(header);

        std::vector<ModelEntry> modelEntries;
        uint64_t currentOffset = writer.getPosition();

        for (const auto& assetModel : models)
        {
            ModelEntry entry = {};
            std::strncpy(entry.name, assetModel->getModel()->getName().c_str(), sizeof(entry.name)-1);
            entry.offset = currentOffset;

            std::ostringstream oss(std::ios::binary);
            {
                serializeModel(writer, *assetModel->getModel());
            }

            uint64_t newOffset = writer.getPosition();
            entry.size = newOffset - entry.offset;
            currentOffset = newOffset;

            modelEntries.push_back(entry);
        }

        header.modelsTableOffset = currentOffset;
        writer.write(modelEntries);

        writer.setPosition(0);
        writer.write(header);

        writer.close();

        return true;
    }

    bool readElixPacket(const std::string& filePath, std::vector<std::unique_ptr<AssetModel>>& outModels)
    {
        BinaryLoader reader;
    
        if (!reader.init(filePath, BinaryLoader::Mode::Read))
            return false;

        ElixPacketHeader header;
        reader.read(header);

        if (strncmp(header.magic, "ELIXPKT", 7) != 0)
            return false;

        reader.setPosition(header.modelsTableOffset);

        std::vector<ModelEntry> modelEntries(header.modelCount);

        reader.read(modelEntries);

        for (const auto& entry : modelEntries)
        {
            reader.setPosition(entry.offset);

            outModels.push_back(std::move(deserializeModel(reader)));
        }

        reader.close();

        return true;
    }
};

ELIX_NAMESPACE_END

#endif //BINARY_SERIALIZER_HPP
