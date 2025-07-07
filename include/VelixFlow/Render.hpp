#ifndef RENDER_HPP
#define RENDER_HPP

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <string>
#include "Scene.hpp"
#include "FrameBuffer.hpp"
#include <memory>
#include "Window.hpp"

namespace elix
{
    struct FrameData
    {
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        glm::vec3 cameraPosition;
    };

    class IRender
    {
    public:
        virtual window::ClearFlag getClearFlag() {return window::ClearFlag::NONE; }
        virtual bool shouldExecute() const = 0;
        virtual std::string getName() const = 0;
        virtual void render(const elix::FrameData& frameData, Scene* scene = nullptr) = 0;
        void setRenderTarget(const std::shared_ptr<elix::FrameBuffer>& renderTarget) {renderTarget_ = renderTarget; }
        virtual ~IRender() = default;
        virtual int getPriority() const = 0;
    protected:
        std::shared_ptr<elix::FrameBuffer> renderTarget_{nullptr};
    };
} //namespace elix


#endif //RENDER_HPP
