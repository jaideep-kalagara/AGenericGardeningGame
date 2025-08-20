IMGUI = imgui/*.cpp imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp

all:
	g++ -std=c++26 -Iinclude -Iimgui -Iimgui/backends -Llib -o out src/*.cpp src/gl.c $(IMGUI) -DGLFW_INCLUDE_NONE -DIMGUI_IMPL_OPENGL_LOADER_GLAD -lglfw3 -lopengl32 -lgdi32 -lstdc++exp