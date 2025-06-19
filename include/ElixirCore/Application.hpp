#ifndef APPLICATION_HPP
#define APPLICATION_HPP


namespace elix
{
    class Application
    {
    public:
        Application() = default;

        static void initializeCore();

        static void shutdownCore();

        static Application* createApplication();

        static Application& instance();

        void init();

        void shutdown();
    };
}

#endif //APPLICATION_HPP
