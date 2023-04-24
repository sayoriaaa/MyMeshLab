//
// Created by sayori on 2023/4/24.
//

#ifndef MYMESHLAB_IO_H
#define MYMESHLAB_IO_H

#endif //MYMESHLAB_IO_H
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <iomanip>
#include "ui_utils.h"

int read_obj(
        TCHAR* file_name,
        std::vector<double>& verts,
        std::vector<size_t>& mesh,
        std::vector<size_t>& mesh_ptr);

int read_mesh(
        TCHAR* in_mesh,
        std::vector<double>& V,
        std::vector<size_t>& M);