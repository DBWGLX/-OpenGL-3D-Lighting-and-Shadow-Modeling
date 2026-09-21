#pragma once
#include <string>

constexpr int WINDOW_WIDTH  = 1000;
constexpr int WINDOW_HEIGHT = 800;

inline std::string vertexShaderPath() {
#ifdef __APPLE__
    return "shaders/vshader_mac.glsl";
#else
    return "shaders/vshader_win.glsl";
#endif
}

inline std::string fragmentShaderPath() {
#ifdef __APPLE__
    return "shaders/fshader_mac.glsl";
#else
    return "shaders/fshader_win.glsl";
#endif
}