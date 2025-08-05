#include "VelixVK/VKSceneRender.hpp"

ELIX_NAMESPACE_BEGIN

namespace render
{
    window::ClearFlag VKSceneRender::getClearFlag()
    {
        return window::ClearFlag::NONE;
    }

    bool VKSceneRender::shouldExecute() const
    {
        return true;
    }

    std::string VKSceneRender::getName() const
    {
        return "VKSceneRender";
    }

    void VKSceneRender::render(const FrameData& frameData, Scene* scene)
    {

    }

    int VKSceneRender::getPriority() const
    {
        return 1;
    }
    
} //namespace render

ELIX_NAMESPACE_END