#include <iostream>
#include "lib/volk/volk.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <cstdio>
#include <cstdlib>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

//#define APP_USE_UNLIMITED_FRAME_RATE
#ifdef _DEBUG
#define APP_USE_VULKAN_DEBUG_REPORT
#endif

static void _error_callback(int code, const char *description) {
    fprintf(stderr, "GLFW error %d: %s\n", code, description);
}

int main() {
    glfwSetErrorCallback(_error_callback);
    if (glfwInit()) {
        std::cout << "GLFW initialized!" << std::endl;
    } else {
        fprintf(stderr, "%s", "error initializing GLFW\n");
        exit(1);
    }

    if (glfwVulkanSupported()) {
        std::cout << "Vulkan supported!" << std::endl;
    }

    if (!volkInitialize()) {
        std::cout << "volk initialized!" << std::endl;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Window Title", glfwGetPrimaryMonitor(), NULL);
    int x;
    std::cin >> x;
    return 0;
}
