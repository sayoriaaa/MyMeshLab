//
// Created by sayori on 2023/4/24.
//


#include "header.h"
#include "mesh.h"
#include "io.h"
#include "shader/shader.h"
#include "camera.h"
#include "front.h"
#include "demo.h"


std::string projdir = "D:/Manage-my-github/MyMeshLab/";

void run_demo_quad(){
    // Create window with graphics context
    GLFWwindow* window = front::create_glfw_window();
    front::init();


    Shader ourShader = demo::quad::compile_shader(projdir);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {

        front::quad::render_imgui();
        demo::quad::buff_shader();

        front::get_display_w_h();
        cam::camera_get_w_h(front::display_w, front::display_h);

        demo::quad::draw_background();

        // draw our stuff
        ourShader.use();

        if(cam::allow_camera_look_at_free == false){
            if(cam::set_camera_look_at_0 == true) cam::exec_set_camera_look_at_0();
            if(cam::set_camera_look_at_0 == false) cam::exec_fix_camera_look_dir();
        }

        // create transformations
        cam::construct_mvp();
        demo::quad::shade(ourShader);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
        front::quad::processInput(window);

    }
    demo::quad::release_buff();
}

void run_mesh(){
    // Create window with graphics context
    GLFWwindow* window = front::create_glfw_window();
    front::init();

    //glEnable(GL_DEPTH_TEST);// because 3D
    Shader ourShader = demo::main::compile_shader(projdir);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {

        front::mesh::render_imgui();
        demo::main::buff_shader();

        front::get_display_w_h();
        cam::camera_get_w_h(front::display_w, front::display_h);

        demo::main::draw_background();

        // draw our stuff
        ourShader.use();

        if(cam::allow_camera_look_at_free == false){
            if(cam::set_camera_look_at_0 == true) cam::exec_set_camera_look_at_0();
            if(cam::set_camera_look_at_0 == false) cam::exec_fix_camera_look_dir();
        }

        // create transformations
        cam::construct_mvp();
        if(front::mesh::selected_model) demo::main::shade(ourShader);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
        front::mesh::processInput(window);

    }
    demo::quad::release_buff();
}

int main(int argc, char** argv)
{
    if (!glfwInit())
        return 1;

    //run_demo_quad();
    run_mesh();

    front::clean_up();
    return 0;
}

