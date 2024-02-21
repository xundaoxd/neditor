if(imgui-node-editor-proj_POPULATED)
    return()
endif()

find_package(imgui REQUIRED)
include(FetchContent)

FetchContent_Declare(imgui-node-editor-proj
    GIT_REPOSITORY      https://github.com/thedmd/imgui-node-editor.git
    GIT_TAG             master
)
FetchContent_Populate(imgui-node-editor-proj)

add_library(imgui-neditor
    ${imgui-node-editor-proj_SOURCE_DIR}/imgui_node_editor.cpp
)
target_include_directories(imgui-neditor PUBLIC ${imgui-node-editor-proj_SOURCE_DIR})
target_link_libraries(imgui-neditor PUBLIC imgui)

