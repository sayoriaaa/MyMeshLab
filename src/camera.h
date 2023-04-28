//
// Created by sayori on 2023/4/27.
//

#ifndef MYMESHLAB_CAMERA_H
#define MYMESHLAB_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace cam{
    float world_radians = -55.;
    float world_vec[3] = {1., 0., 0.};
    float world_vec2[3] = {0., 0., 0.};

    float camera_position[3] = {0., 0., 3.};
    float camera_lookat[3] = {0., 0., 0.};
    float camera_up[3] = {0., 1., 0.};
    float cameraFront[3] = {0., 0., -1.};

    float projection_near = 0.1;
    float projection_far = 100.;
    float projection_fov = 45.f;

    int display_w;
    int display_h;

    bool set_camera_look_at_0 = false;
    bool allow_camera_look_at_free = false;

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;


    void camera_go_w(float step){
        camera_position[0] += step * cameraFront[0];
        camera_position[1] += step * cameraFront[1];
        camera_position[2] += step * cameraFront[2];
    }

    void camera_go_s(float step){
        camera_position[0] -= step * cameraFront[0];
        camera_position[1] -= step * cameraFront[1];
        camera_position[2] -= step * cameraFront[2];
    }

    void camera_go_a(float step){
        auto temp = glm::normalize(glm::cross(glm::vec3(cameraFront[0], cameraFront[1], cameraFront[2]), glm::vec3(camera_up[0], camera_up[1], camera_up[2]))) * step;
        camera_position[0] -= temp.x;
        camera_position[1] -= temp.y;
        camera_position[2] -= temp.z;
    }

    void camera_go_d(float step){
        auto temp = glm::normalize(glm::cross(glm::vec3(cameraFront[0], cameraFront[1], cameraFront[2]), glm::vec3(camera_up[0], camera_up[1], camera_up[2]))) * step;
        camera_position[0] += temp.x;
        camera_position[1] += temp.y;
        camera_position[2] += temp.z;
    }

    void exec_set_camera_look_at_0(){
        camera_lookat[0] = 0.;
        camera_lookat[1] = 0.;
        camera_lookat[2] = 0.;
    }

    void exec_fix_camera_look_dir(){
        cameraFront[0] = 0.;
        cameraFront[1] = 0.;
        cameraFront[2] = -1.;
        camera_lookat[0] = camera_position[0] + cameraFront[0];
        camera_lookat[1] = camera_position[1] + cameraFront[1];
        camera_lookat[2] = camera_position[2] + cameraFront[2];
    }

    void construct_mvp(){
        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(world_radians), glm::vec3(world_vec[0], world_vec[1], world_vec[2]));
        model = glm::translate(model, glm::vec3(world_vec2[0], world_vec2[1], world_vec2[2]));
        view = glm::lookAt(glm::vec3(camera_position[0], camera_position[1], camera_position[2]),
                           glm::vec3(camera_lookat[0], camera_lookat[1], camera_lookat[2]),
                           glm::vec3(camera_up[0], camera_up[1], camera_up[2]));

        projection = glm::perspective(glm::radians(projection_fov), (float)display_h / (float)display_h, projection_near, projection_far);
    }

    void camera_get_w_h(int w, int h){
        display_w = w;
        display_h = h;
    }

}
#endif //MYMESHLAB_CAMERA_H
