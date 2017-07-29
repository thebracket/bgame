#include <stdexcept>
#include "main_window.hpp"
#include "gl_include.hpp"

namespace bengine {

    GLFWwindow * main_window;
    std::function<void(const double &)> main_func;

    void init_glfw() {
        // Start GL Context using the GLFW3 helper library
        if (!glfwInit()) {
            throw std::runtime_error("Could not start GLFW");
        }
    }

    void init_glfw_window_hints() {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void finalize_glfw() {
        if (main_window == nullptr) throw std::runtime_error("Error creating main window.");
        glfwMakeContextCurrent(main_window);
        glewExperimental = GL_TRUE;
        glewInit();
    }

    void init() {
        init_glfw();
        init_glfw_window_hints();

        GLFWmonitor * monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        main_window = glfwCreateWindow(mode->width, mode->height, "Nox Futura", monitor, nullptr);
        if (!main_window) {
            glfwTerminate();
            throw std::runtime_error("Unable to initialize window");
        }

        finalize_glfw();
    }

    void run() {
        double duration_ms = 0.0;
        while (!glfwWindowShouldClose(main_window)) {
            clock_t start_time = clock();
            glfwPollEvents();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            main_func(duration_ms);

            glfwSwapBuffers(main_window);
            duration_ms = ((clock() - start_time) * 1000.0) / CLOCKS_PER_SEC;
        }
        glfwTerminate();
    }
}