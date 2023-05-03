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
#include <CGAL/Polygon_mesh_processing/distance.h>

#include "io.h"
#include <random>
#include <cfloat>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Surface_mesh<K::Point_3> cgal_Mesh;

class SimpleMesh{
public:
    std::vector<double> vertices;
    std::vector<unsigned int> indices;

    SimpleMesh(TCHAR* file_name){
        //vertices = new std::vector<double>();
        //indices = new std::vector<unsigned int>();
        io::read_mesh(file_name, vertices, indices);
    }

    SimpleMesh(cgal_Mesh* m, bool delete_cgal_mesh= true){
        for(cgal_Mesh::Vertex_index v : m->vertices()){
            K::Point_3 vertex = m->point(v);
            vertices.push_back(double(vertex.x()));
            vertices.push_back(double(vertex.y()));
            vertices.push_back(double(vertex.z()));
        }

        for(cgal_Mesh::Face_index f : m->faces()){
            for(cgal_Mesh::Vertex_index v : m->vertices_around_face(m->halfedge(f))){
                indices.push_back(unsigned int(v.idx()));
            }
        }

        if(delete_cgal_mesh) delete m;
    }

    cgal_Mesh* to_cgal_Mesh(){
        //转换成CGAL里的数据结构
        std::cout << "# [ Start to transfer to CGAL data structure ]" << std::endl;
        cgal_Mesh *ret = new cgal_Mesh();
        for(unsigned int i=0; i<vertices.size(); i+=3){
            K::Point_3 vertex(vertices[i], vertices[i+1], vertices[i+2]);
            ret->add_vertex(vertex);
        }

        for(unsigned int i=0; i<indices.size(); i+=3) {
            cgal_Mesh::Face_index face = ret->add_face(
                    cgal_Mesh::Vertex_index(indices[i]),
                    cgal_Mesh::Vertex_index(indices[i + 1]),
                    cgal_Mesh::Vertex_index(indices[i + 2])
            );
        }
        std::cout << "# [ End transfer to CGAL data structure ]" << std::endl;
        return ret;
    }

};

namespace mesh{
    //std::vector<double> vertices;
    //std::vector<unsigned int> indices;

    std::vector<SimpleMesh*> mesh_buffer;
    int cur_mesh = -1;

    void import_mesh(TCHAR* file_name){
        std::cout << "# [ importing mesh ] ..." << std::endl;
        auto *m =  new SimpleMesh(file_name);
        mesh_buffer.push_back(m);
        cur_mesh ++ ;
    }

    void perturb_mesh(SimpleMesh *a){
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> uni_float(-0.05, 0.05);

        SimpleMesh *b = new SimpleMesh(*a);
        for(int i=0; i<b->vertices.size(); i++){
            b->vertices[i] += uni_float(mt);
        }

        mesh_buffer.push_back(b);
        cur_mesh++;

    }

    void cgal_isotropic_remesh(SimpleMesh *a){
        cgal_Mesh *b = a->to_cgal_Mesh();
        //determine target edge size
        double max_edge = -1., min_edge = DBL_MAX, edge_size, target_edge;
        for(auto e : b->edges()){
            auto v1 = b->point(b->vertex(e, 0));
            auto v2 = b->point(b->vertex(e, 1));
            edge_size = CGAL::sqrt(CGAL::squared_distance(v1, v2));
            if(max_edge<edge_size) max_edge=edge_size;
            if(min_edge>edge_size) min_edge=edge_size;
        }
        std::cout << " --max edge size: " << max_edge << std::endl << " --min edge size: " << min_edge << std::endl;
        target_edge = (min_edge+max_edge)/2;

        //call CGAL isotropic remeshing function
        std::cout << "# [ start using CGAL isotropic remeshing (target edge =  " << target_edge << ") ]" << std::endl;
        CGAL::Polygon_mesh_processing::isotropic_remeshing(b->faces(), target_edge, *b, CGAL::parameters::number_of_iterations(3));
        /*
        std::cout << " -- Hausdorff distance "
                  << CGAL::Polygon_mesh_processing::approximate_Hausdorff_distance
                          (*a, *b, CGAL::parameters::number_of_points_per_area_unit(4000))
                  << " ]"
                  << std::endl;
        */
        std::cout << "# [ done remeshing ]" << std::endl;


        SimpleMesh *c = new SimpleMesh(b);
        mesh_buffer.push_back(c);
        cur_mesh++;

    }

    //----------------unit test-----------------
    void run_test_cgal(SimpleMesh *a){
        cgal_Mesh *b = a->to_cgal_Mesh();
        SimpleMesh *c = new SimpleMesh(b);
        mesh_buffer.push_back(c);
        cur_mesh++;
        std::cout << "# testing mesh " << std::endl;
    }
    // this part works correctly



}
#endif //MYMESHLAB_MESH_H
