#ifndef DEFAULT_RENDER_HPP
#define DEFAULT_RENDER_HPP

#include "Render.hpp"
#include "Window.hpp"

namespace elix
{
    class DefaultRender : public elix::IRender
    {
    public:
        window::ClearFlag getClearFlag() override;
        void setSelectedGameObject(GameObject* gameObject) {m_selectedGameObject = gameObject;}
        void setWindow(window::Window* window) {m_window = window;}
        bool shouldExecute() const override;
        std::string getName() const override;
        void render(const elix::FrameData& frameData, Scene* scene = nullptr) override;
        int getPriority() const override;
        ~DefaultRender() override = default;

    private:
        GameObject* m_selectedGameObject{nullptr};
        window::Window* m_window{nullptr};
    };
} //namespace elix

#endif //DEFAULT_RENDER_HPP
