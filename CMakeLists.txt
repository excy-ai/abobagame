#cmake_minimum_required(VERSION 3.26)
#project(abobavulkan)
#
#file(GLOB SOURCES *.cpp)
#
#set(CMAKE_CXX_STANDARD 23)
#
#find_package(Vulkan REQUIRED)
#
#if (DEFINED VULKAN_SDK_PATH)
#    set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/include")
#    set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/lib")
#    set(Vulkan_FOUND "True")
#else()
#    find_package(Vulkan REQUIRED) # throws error if could not find Vulkan
#    message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
#endif()
#if (NOT Vulkan_FOUND)
#    message(FATAL_ERROR "Could not find Vulkan library!")
#else()
#    message(STATUS "Using vulkan lib at: ${Vulkan_LIBRARIES}")
#endif()
#
#set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
#set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
#set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#
#add_subdirectory(lib/glfw)
#add_subdirectory(lib/glm)
#
#add_executable(${PROJECT_NAME} main.cpp
#        lib/volk/volk.cpp)
#target_link_libraries(${PROJECT_NAME} glfw)
#target_link_libraries(${PROJECT_NAME} glm)
#target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
#
cmake_minimum_required(VERSION 3.26)
project(abobavulkan)

file(GLOB SOURCES *.cpp)

set(CMAKE_CXX_STANDARD 23)

find_package(Vulkan REQUIRED)

if (DEFINED VULKAN_SDK_PATH)
    set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/include")
    set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/lib")
    set(Vulkan_FOUND "True")
else()
    find_package(Vulkan REQUIRED) # throws error if could not find Vulkan
    message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
endif()
if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find Vulkan library!")
else()
    message(STATUS "Using vulkan lib at: ${Vulkan_LIBRARIES}")
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/glfw)
add_subdirectory(lib/glm)

# IMGUI
set(IMGUI_DIR lib/imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ..)

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} glm)
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)

add_executable(${PROJECT_NAME} ${SOURCES} lib/volk/volk.cpp ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_demo.cpp ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp)

target_compile_definitions(${PROJECT_NAME} PUBLIC -DImTextureID=ImU64)
