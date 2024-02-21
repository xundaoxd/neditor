find_package(glfw3 REQUIRED)

include(FetchContent)

FetchContent_Declare(imgui-proj
    GIT_REPOSITORY      https://github.com/ocornut/imgui.git
    GIT_TAG             master
)
FetchContent_Populate(imgui-proj)

add_library(imgui
    ${imgui-proj_SOURCE_DIR}/imgui_draw.cpp
    ${imgui-proj_SOURCE_DIR}/imgui_tables.cpp
    ${imgui-proj_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui-proj_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui-proj_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC ${imgui-proj_SOURCE_DIR} ${imgui-proj_SOURCE_DIR}/backends)
target_link_libraries(imgui PUBLIC glfw dl)

