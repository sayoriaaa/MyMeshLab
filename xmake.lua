add_requires("glfw", "glad")
add_requires("imgui", {configs = {glfw_opengl3 = true}})
--add_requires("libigl")
add_requires("cgal")
target("MyLab")
    set_kind("binary")
    add_files("src/*.cpp") --例如"src/*.cpp|ui_utils.cpp|ui_utils.h" |后面表示ignore
    --add_requires("eigen", {system = true})
    --add_requires("cgal", {system = true})
    --add_requires("matplotplusplus", {system = true})
    add_packages("glfw", "glad")
    add_packages("imgui")
    add_packages("cgal", "eigen")
    --add_packages("libigl")
    add_links("User32", "Comdlg32") --为了实现调用win32的打开文件
