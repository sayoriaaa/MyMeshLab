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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

int main(int argc, char** argv)
{
    if (!glfwInit())
        return 1;

    // Create window with graphics context
    GLFWwindow* window = front::quad::create_glfw_window();

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    //编译shader
    //气死我啦，这里一定得传绝对地址，而且xmake也没能用预定义宏把projdir传进来，暂时只能在前面手动设一下
    std::string vdir = "src/shader/shader.vs";
    std::string fdir = "src/shader/shader.fs";
    vdir = projdir+vdir;
    fdir = projdir+fdir;
    Shader ourShader(vdir.c_str(), fdir.c_str());
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------


    // Main loop
    while (!glfwWindowShouldClose(window))
    {

        front::quad::render_imgui();
        demo::quad::buff_shader();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        cam::get_display_w_h(display_w, display_h);// pass it to camera namespace

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
    front::clean_up();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

