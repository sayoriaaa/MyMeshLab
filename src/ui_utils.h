//
// Created by sayori on 2023/4/24.
//

#ifndef MYMESHLAB_UI_UTILS_H
#define MYMESHLAB_UI_UTILS_H

#endif //MYMESHLAB_UI_UTILS_H

#ifdef _WIN32
#include <Windows.h>
#include <stdio.h>
TCHAR *open_file_dialog();
#endif


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#undef GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <tchar.h>
#include "commdlg.h"

//std::string TCHAR2STRING(TCHAR *STR);