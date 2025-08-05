#ifndef ASSETS_CACHE_HPP
#define ASSETS_CACHE_HPP

#include <filesystem>
#include <memory>
#include <unordered_map>
#include <string>

#include "VelixFlow/Logger.hpp"
#include "VelixFlow/Assets.hpp"
#include "VelixFlow/DefaultMacros.hpp"

ELIX_NAMESPACE_BEGIN

class AssetsCache
{
public:
    template<typename T>
    std::vector<T*> getAllAssets()
    {
        std::vector<T*> result;

        for (const auto& [_, asset] : m_assets)
            if (auto dynamicAsset = dynamic_cast<T*>(asset.get()))
                result.push_back(dynamicAsset);

        return result;
    }

    template<typename T>
    T* addAsset(const std::string& path, std::unique_ptr<T> asset)
    {
        static_assert(std::is_base_of<Asset, T>::value, "T must derive from Asset");

        T* raw = asset.get();
        m_assets[path] = std::move(asset);
        return raw;
    }

    template<typename T>
    T* getAsset(const std::string& path)
    {
        if (const auto it = m_assets.find(path); it != m_assets.end())
            return dynamic_cast<T*>(it->second.get());

        for (const auto& [pathToAsset, asset] : m_assets)
            if (std::string justAName = std::filesystem::path(pathToAsset).filename().string(); justAName == path)
                return dynamic_cast<T*>(asset.get());

        return nullptr;
    }

private:
    std::unordered_map<std::string, std::unique_ptr<Asset>> m_assets;
};

ELIX_NAMESPACE_END

#endif //ASSETS_CACHE_HPP
