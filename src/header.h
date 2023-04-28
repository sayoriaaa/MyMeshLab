//
// Created by sayori on 2023/4/28.
//

#ifndef MYMESHLAB_HEADER_H
#define MYMESHLAB_HEADER_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#undef GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#endif //MYMESHLAB_HEADER_H
