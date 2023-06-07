#ifndef APP_HPP
#define APP_HPP

#include <memory>

#include "window/window.hpp"
#include "render/renderer/cube_renderer.hpp"
#include "controllers/camera/flying_camera_controller.hpp"

class App {
public:
    App(const char *title, uint32_t width, uint32_t height);
    ~App();

    void start();
    void init();
    void terminate();
private:
    //! Init render internal components of the system like renderer
    //! The purpose, easy to misplace them in the init function
    void initRenderIternal();

    WindowWrapper m_window;
    rend::CubeRenderer m_renderer;
    std::unique_ptr<control::FlyingCameraController> m_cameraController;
};

#endif // APP_HPP
