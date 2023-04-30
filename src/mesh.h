//
// Created by sayori on 2023/4/26.
//

#ifndef MYMESHLAB_MESH_H
#define MYMESHLAB_MESH_H
#include <CGAL/boost/graph/named_params_helper.h>
#include <CGAL/boost/graph/properties.h>
#include <CGAL/property_map.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#include <CGAL/Polygon_mesh_processing/compute_normal.h>
#include <CGAL/Polygon_mesh_processing/measure.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <CGAL/Weights/voronoi_region_weights.h>
#include <CGAL/Weights/cotangent_weights.h>
#include <CGAL/boost/graph/iterator.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_3.h>

#include "io.h"

namespace mesh{
    std::vector<double> vertices;
    std::vector<unsigned int> indices;

    void import_mesh(TCHAR* file_name){
        io::read_mesh(file_name, vertices, indices);
    }



}
#endif //MYMESHLAB_MESH_H
