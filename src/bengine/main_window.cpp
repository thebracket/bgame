#include "main_window.hpp"
#include "gl_include.hpp"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "IconsFontAwesome.h"
#include "telemetry.hpp"
#include "filesystem.hpp"
#include "imgui_style.hpp"
#include <chrono>
#include "../global_assets/game_config.hpp"

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
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void finalize_glfw() {
        if (main_window == nullptr) throw std::runtime_error("Error creating main window.");
        glfwMakeContextCurrent(main_window);
        glewExperimental = GL_TRUE;
        glewInit();
		if (config::game_config.vsync)
			glfwSwapInterval(1);
    }

	ImFontConfig config;
	const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

    void init(bool fullscreen, int width, int height, std::string gui_font, int gui_font_size) {
        init_glfw();
        init_glfw_window_hints();

        if (fullscreen) {
            GLFWmonitor *monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode *mode = glfwGetVideoMode(monitor);
            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

            main_window = glfwCreateWindow(mode->width, mode->height, "Nox Futura", monitor, nullptr);
        } else {
            GLFWmonitor *monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode *mode = glfwGetVideoMode(monitor);
            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
			glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

            main_window = glfwCreateWindow(width, height, "Nox Futura", nullptr, nullptr);
        }
        if (!main_window) {
            glfwTerminate();
            throw std::runtime_error("Unable to initialize window");
        }

        finalize_glfw();
		printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));

        ImGuiIO& io = ImGui::GetIO();
        const std::string font_path = std::string("game_assets/") + gui_font;
        //io.Fonts->AddFontDefault();
		glog(log_target::LOADER, log_severity::info, "Loading {0}, at size {1} pixels", font_path, gui_font_size);
        io.Fonts->AddFontFromFileTTF(font_path.c_str(), static_cast<float>(gui_font_size));
        config.MergeMode = true;
        // TODO: Why does this fail?
		const std::string fontawesome_path = "game_assets/fontawesome-webfont.ttf";
		if (!exists(fontawesome_path)) throw std::runtime_error("Unable to load Font Awesome");
		io.Fonts->AddFontFromFileTTF(fontawesome_path.c_str(), static_cast<float>(gui_font_size), &config, icon_ranges);

		setup_imgui_style();

        ImGui_ImplGlfwGL3_Init(main_window, true);
    }

    void run() {
		//glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE); // Move z-buffer to 0..1 instead of +1..-1.
        double duration_ms = 0.0;
        while (!glfwWindowShouldClose(main_window)) {
            auto start_time = std::chrono::high_resolution_clock::now();
            glfwPollEvents();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            main_func(duration_ms);

            glFlush();
            glfwSwapBuffers(main_window);
            auto end_time = std::chrono::high_resolution_clock::now();
            duration_ms = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count());
			telemetry::on_tick();
        }
        stop_telemetry();
        glfwTerminate();
    }
}
