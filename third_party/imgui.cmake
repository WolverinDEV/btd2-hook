add_library(imgui__core
        third_party/imgui/imgui.cpp
        third_party/imgui/imgui_draw.cpp
        third_party/imgui/imgui_tables.cpp
        third_party/imgui/imgui_widgets.cpp

        third_party/imgui/misc/cpp/imgui_stdlib.cpp
)

add_library(imgui__win32 third_party/imgui/backends/imgui_impl_win32.cpp)
target_link_libraries(imgui__win32 imgui__core)

add_library(imgui__opengl3 third_party/imgui/backends/imgui_impl_opengl3.cpp)
target_link_libraries(imgui__opengl3 imgui__core)

target_include_directories(imgui__core PUBLIC third_party/imgui/ third_party/imgui/backends/ third_party/imgui/misc/cpp/)