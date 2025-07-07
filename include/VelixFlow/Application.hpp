#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Renderer.hpp"
#include <memory>
#include "CameraComponent.hpp"
#include "Window.hpp"
#include "Scene.hpp"

namespace elix
{
    class Application
    {
    public:
        void update();

        void render();

        void endRender();

        static void shutdownCore();

        static std::unique_ptr<Application> createApplication();
        
        void init();

        float getDeltaTime() const;

        elix::Renderer* getRenderer() const;
        elix::CameraComponent* getCamera() const;
        window::Window* getWindow() const;
        Scene* getScene() const;
    private:
        std::unique_ptr<elix::CameraComponent> m_camera{nullptr};
        std::unique_ptr<elix::Renderer> m_renderer{nullptr};
        std::unique_ptr<window::Window> m_mainWindow{nullptr};
        std::unique_ptr<Scene> m_scene{nullptr};
        float m_lastFrame{0.0f};
        float m_deltaTime{0.0f};

    };
}

#endif //APPLICATION_HPP
