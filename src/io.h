//
// Created by sayori on 2023/4/24.
//

#ifndef MYMESHLAB_IO_H
#define MYMESHLAB_IO_H

#include <string>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <iomanip>

#include <Windows.h>
#include <tchar.h>
#include "commdlg.h"

namespace io {

    // a simple obj parser. only read vertices coordinates and faces
    int read_obj(
            TCHAR* file_name,
            std::vector<double>& verts,
            std::vector<unsigned int>& mesh,
            std::vector<size_t>& mesh_ptr)
    {
        std::cout << "# [ Start to read OBJ file ]" << std::endl;
        std::ifstream inf(file_name);

        if (inf.fail()) {
            std::cerr << "# [ Error: read obj ] Can't open file: "
                      << file_name << std::endl;
            return __LINE__;
        }

        std::vector<double>& V  = verts;
        std::vector<unsigned int>& M  = mesh;
        std::vector<size_t>& MP = mesh_ptr;

        std::string str_line;
        size_t cnt = 0;
        while (std::getline(inf, str_line)) {
            std::string ty;
            std::istringstream ss(str_line);
            ss >> ty;
            if (ty[0] == '#') { continue; }
            else if (ty == "v") {
                double x, y, z;
                ss >> x >> y >> z;
                V.push_back(x); V.push_back(y); V.push_back(z);
            } else if ( ty == "f") {
                std::string fid_str;
                //mesh_ptr.push_back(mesh.size());
                MP.push_back(cnt);
                while (ss >> fid_str) {
                    std::istringstream fid_ss(fid_str);
                    size_t fid;
                    fid_ss >> fid;
                    ++cnt;
                    M.push_back(fid-1);
                }
            } else if ( ty == "l") {
                std::string fid_str;
                //mesh_ptr.push_back(mesh.size());
                MP.push_back(cnt);
                while (ss >> fid_str) {
                    std::istringstream fid_ss(fid_str);
                    size_t fid;
                    fid_ss >> fid;
                    ++cnt;
                    M.push_back(fid-1);
                }
            }
        }

        MP.push_back(cnt);

        if (cnt != M.size()) {
            std::cerr << "-- [ ERROR: read obj ] mesh size is not compatible with faces type sum."
                      << std::endl;
        }

        std::cout << "-- Verts Num: " << V.size()/3 << std::endl;
        std::cout << "-- Faces Num: " << MP.size()-1 << std::endl;
        if (MP.size() > 1)
            std::cout << "-- First face type: "<< MP[1]-MP[0] << std::endl;
        std::cout << "# [ End: read obj.]" << std::endl;

        return 0;
    }

    int read_mesh(
            TCHAR* in_mesh,
            std::vector<double>& V,
            std::vector<unsigned int>& M)
    {
        std::vector<size_t> MP;
        return read_obj(in_mesh, V, M, MP);
    }
}
#endif //MYMESHLAB_IO_H
