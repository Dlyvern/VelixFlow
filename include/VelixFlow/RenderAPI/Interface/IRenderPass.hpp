#ifndef IRENDER_PASS_HPP
#define IRENDER_PASS_HPP

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <string>
#include <memory>

// #include "VelixFlow/FrameBuffer.hpp"
#include "VelixFlow/Window.hpp"
#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/Scene.hpp"

ELIX_NAMESPACE_BEGIN

namespace render
{
    struct FrameData
    {
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        glm::vec3 cameraPosition;
        glm::mat4 uiProjection;
        glm::mat4 flippedUiProjection;
        glm::vec2 screenPosition;
    };
        
    class IRenderPass
    {
    public:
        virtual window::ClearFlag getClearFlag() {return window::ClearFlag::NONE; }
        virtual bool shouldExecute() const = 0;
        virtual std::string getName() const = 0;
        virtual void render(const FrameData& frameData, Scene* scene = nullptr) = 0;
        // void setRenderTarget(const std::shared_ptr<FrameBuffer>& renderTarget) {renderTarget_ = renderTarget; }
        virtual ~IRenderPass() = default;
        virtual int getPriority() const = 0;
    // protected:
    //     std::shared_ptr<FrameBuffer> renderTarget_{nullptr};
    };
}

ELIX_NAMESPACE_END

#endif //IRENDER_PASS_HPP
