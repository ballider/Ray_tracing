cmake_minimum_required(VERSION 2.8)

project(Ray_tracing)

set(Ray_tracing_src main.cpp Vec3d.cpp render.cpp File_work.cpp)
set(Ray_tracing_hdr head.hpp Vec3d.hpp Ray.hpp Material.hpp Light.hpp Figure.hpp Figure_Factory.hpp)

add_executable(${PROJECT_NAME} ${Ray_tracing_src} ${Ray_tracing_hdr})