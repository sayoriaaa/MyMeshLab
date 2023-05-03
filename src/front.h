//
// Created by sayori on 2023/4/27.
//

#ifndef MYMESHLAB_FRONT_H
#define MYMESHLAB_FRONT_H

#include "camera.h"
#include "util.h"
#include "mesh.h"
//front空间负责主窗口的创建、imgui的绘制及imgui上相关设置变量的访问
namespace front{
    GLFWwindow* window;
    int display_w, display_h;

    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }

    void init(){
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    }

    void get_display_w_h(){
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
    }

    GLFWwindow *create_glfw_window() {
        // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
        // GL ES 2.0 + GLSL 100
                const char* glsl_version = "#version 100";
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
                glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
        // GL 3.2 + GLSL 150
                const char* glsl_version = "#version 150";
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
        // GL 3.0 + GLSL 130
        const char *glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

        window = glfwCreateWindow(1280, 720, "MyMeshLab", NULL, NULL);
        if (window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // Enable vsync

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        if (window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
        }
        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }
        return window;
    }

    namespace quad {
        //imgui页面的一些设置变量

        bool *show_demo_window = new bool(true);
        bool *show_another_window = new bool(false);


        ImVec4 *clear_color = new ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        ImVec4 *vcolor1 = new ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        ImVec4 *vcolor2 = new ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        ImVec4 *vcolor3 = new ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        bool selected_model = false;//标记是否打开模型
        TCHAR *szFileName;//Mesh File

        void render_imgui() {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::ShowDemoWindow(show_demo_window);
            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Settings");

                ImGui::Text("This is some useful text.");
                ImGui::Checkbox("Demo Window", show_demo_window);
                ImGui::Checkbox("Another Window", show_another_window);

                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
                ImGui::ColorEdit3("background color", (float *) clear_color);
                ImGui::ColorEdit3("vertex color 1", (float *) vcolor1);
                ImGui::ColorEdit3("vertex color 2", (float *) vcolor2);
                ImGui::ColorEdit3("vertex color 3", (float *) vcolor3);

                if (ImGui::Button("Open File")) {
                    szFileName = open_file_dialog();

                    _tprintf("selected: %s\n", szFileName);
                    selected_model = true;
                    std::vector<double> V, P; // V: 3*vn, P: 4*pn.
                    std::vector<unsigned int> M; // M: 3*fn.
                    io::read_mesh(szFileName, V, M);

                }
                if (selected_model) {
                    ImGui::Text("Selected file: %s\n", szFileName);
                }

                ImGui::Text("Here set model, view, projection matrix");
                ImGui::Text("Here set model matrix(local space -> world space)");

                ImGui::SliderFloat3("set direction of model matrix", cam::world_vec, 0., 1.);
                ImGui::SliderFloat("set radians(0-360) of the given direction", &cam::world_radians, -360, 360);
                ImGui::SliderFloat3("set object translation", cam::world_vec2, -5, 5);
                ImGui::Text("Here set view matrix(world space -> view space)");
                ImGui::SliderFloat3("set camera position", cam::camera_position, -10., 10.);
                ImGui::SliderFloat3("set camera look at position(0,0,0 or free for fps camera)", cam::camera_lookat,
                                    -10., 10.);
                ImGui::SliderFloat3("set camera up position(0,1,0 for almost all cases)", cam::camera_up, -10., 10.);
                ImGui::Text(
                        "Here is two way to construct lookat matrix: keep gaze point (0,0,0) fixed or keep look direction (0,0,-1) fixed");
                ImGui::Text(
                        "you can see find first mode in 3d reconstruction tasks like nerf dataset, and the latter one in games");
                ImGui::Text(
                        "if you wish to control look direction freely, first activate below: ""allow set arbitrary look at direction"" and then set above camera look at matrix, or use mouse");
                if (ImGui::Button("switch camera mode:(look at 0/fps)"))
                    cam::set_camera_look_at_0 = !cam::set_camera_look_at_0;
                if (ImGui::Button("allow set arbitrary look at direction"))
                    cam::allow_camera_look_at_free = !cam::allow_camera_look_at_free;
                ImGui::Text("Here set projection matrix(view space -> clip space)");
                ImGui::SliderFloat("set near", &cam::projection_near, 0, 1);
                ImGui::SliderFloat("set far", &cam::projection_far, 0, 100);
                ImGui::SliderFloat("set fov", &cam::projection_fov, 0, 180);
                ImGui::End();
            }

            // 3. Show another simple window.
            if (show_another_window) {
                ImGui::Begin("Another Window", show_another_window);
                ImGui::Text("Hello from another window!");
                if (ImGui::Button("Close Me"))
                    *show_another_window = false;
                ImGui::End();
            }
            // Rendering
            ImGui::Render();
        }


        void processInput(GLFWwindow *window) {
            float cameraSpeed = 0.05f; // adjust accordingly
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                cam::camera_go_w(cameraSpeed);
            }
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                cam::camera_go_s(cameraSpeed);
            }
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                cam::camera_go_a(cameraSpeed);
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                cam::camera_go_d(cameraSpeed);
            }
        }
    }

    namespace mesh {
        ImVec4 *clear_color = new ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        ImVec4 *mesh_color = new ImVec4(0.45f, 0.f, 0.f, 1.00f);

        bool use_wireframe = true;

        bool selected_model = false;//标记是否打开模型
        TCHAR *szFileName;//Mesh File

        bool *show_demo_window = new bool(true);

        void render_imgui() {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            //ImGui::ShowDemoWindow(show_demo_window);
            {
                ImGui::Begin("Settings");

                ImGui::ColorEdit3("background color", (float *) clear_color);
                ImGui::ColorEdit3("mesh color", (float *) mesh_color);

                if (ImGui::Button("Open File")) {
                    szFileName = open_file_dialog();

                    _tprintf("# [ selected file %s ]\n", szFileName);
                    selected_model = true;
                    ::mesh::import_mesh(szFileName);
                }
                if (selected_model) {
                    ImGui::Text("Selected file: %s\n", szFileName);
                }

                ImGui::Text("Here set model, view, projection matrix");
                ImGui::Text("Here set model matrix(local space -> world space)");

                ImGui::SliderFloat3("set direction of model matrix", cam::world_vec, 0., 1.);
                ImGui::SliderFloat("set radians(0-360) of the given direction", &cam::world_radians, -360, 360);
                ImGui::SliderFloat3("set object translation", cam::world_vec2, -5, 5);
                ImGui::Text("Here set view matrix(world space -> view space)");
                ImGui::SliderFloat3("set camera position", cam::camera_position, -10., 10.);
                ImGui::SliderFloat3("set camera look at position(0,0,0 or free for fps camera)", cam::camera_lookat,
                                    -10., 10.);
                ImGui::SliderFloat3("set camera up position(0,1,0 for almost all cases)", cam::camera_up, -10., 10.);
                ImGui::Text(
                        "Here is two way to construct lookat matrix: keep gaze point (0,0,0) fixed or keep look direction (0,0,-1) fixed");
                ImGui::Text(
                        "you can see find first mode in 3d reconstruction tasks like nerf dataset, and the latter one in games");
                ImGui::Text(
                        "if you wish to control look direction freely, first activate below: ""allow set arbitrary look at direction"" and then set above camera look at matrix, or use mouse");
                if (ImGui::Button("switch camera mode:(look at 0/fps)"))
                    cam::set_camera_look_at_0 = !cam::set_camera_look_at_0;
                if (ImGui::Button("allow set arbitrary look at direction"))
                    cam::allow_camera_look_at_free = !cam::allow_camera_look_at_free;
                ImGui::Text("Here set projection matrix(view space -> clip space)");
                ImGui::SliderFloat("set near", &cam::projection_near, 0, 1);
                ImGui::SliderFloat("set far", &cam::projection_far, 0, 100);
                ImGui::SliderFloat("set fov", &cam::projection_fov, 0, 180);

                ImGui::Text("render triangle/ wireframe");
                ImGui::Checkbox("use wireframe", &use_wireframe);

                //-----------------------------------implement algorithms-----------------------
                if(ImGui::Button("perturb vertices")){
                    ::mesh::perturb_mesh(::mesh::mesh_buffer[::mesh::cur_mesh]);
                }

                if(ImGui::Button("CGAL isotropic remesh")){
                    ::mesh::cgal_isotropic_remesh(::mesh::mesh_buffer[::mesh::cur_mesh]);
                }

                //-------------unit test-----------------
                if(ImGui::Button("run cgal test")) ::mesh::run_test_cgal(::mesh::mesh_buffer[::mesh::cur_mesh]);
                //-------------------------------------------

                if(ImGui::Button("before model")){
                    if(::mesh::cur_mesh>0) {
                        ::mesh::cur_mesh--;
                        std::cout << "# [ switched to previous model " << ::mesh::cur_mesh << " ]" << std::endl;
                    }
                }
                if(ImGui::Button("after model")){
                    if(::mesh::cur_mesh<::mesh::mesh_buffer.size()-1) {
                        ::mesh::cur_mesh++;
                        std::cout << "# [ switched to next model " << ::mesh::cur_mesh << " ]" << std::endl;
                    }
                }
                ImGui::End();
            }
            // Rendering
            ImGui::Render();
        }

        void processInput(GLFWwindow *window) {
            float cameraSpeed = 0.05f; // adjust accordingly
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                cam::camera_go_w(cameraSpeed);
            }
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                cam::camera_go_s(cameraSpeed);
            }
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                cam::camera_go_a(cameraSpeed);
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                cam::camera_go_d(cameraSpeed);
            }
        }

    }

    void clean_up(){
        // Cleanup imgui
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        //clean up window
        glfwDestroyWindow(window);
        glfwTerminate();
    }

}
#endif //MYMESHLAB_FRONT_H
