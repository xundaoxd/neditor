include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

set(GLFW_BUILD_WAYLAND OFF)
FetchContent_Declare(glfw URL https://github.com/glfw/glfw/archive/refs/tags/3.4.tar.gz)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(imgui-proj
    GIT_REPOSITORY      https://github.com/ocornut/imgui.git
    GIT_TAG             docking
)
FetchContent_Populate(imgui-proj)

add_library(imgui
    ${imgui-proj_SOURCE_DIR}/imgui.cpp
    ${imgui-proj_SOURCE_DIR}/imgui_demo.cpp
    ${imgui-proj_SOURCE_DIR}/imgui_draw.cpp
    ${imgui-proj_SOURCE_DIR}/imgui_tables.cpp
    ${imgui-proj_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui-proj_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui-proj_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC ${imgui-proj_SOURCE_DIR} ${imgui-proj_SOURCE_DIR}/backends)
target_link_libraries(imgui PUBLIC glfw dl GL)
