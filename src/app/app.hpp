#ifndef APP_HPP
#define APP_HPP

#include <memory>

#include "window/window.hpp"
#include "render/renderer/master_renderer.hpp"
#include "controllers/camera/flying_camera_controller.hpp"
#include "world_manager/world_manager.hpp"

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
    rend::MasterRenderer m_renderer;
    std::shared_ptr<control::FlyingCameraController> m_cameraController;
};

#endif // APP_HPP
