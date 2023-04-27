//
// Created by sayori on 2023/4/24.
//
#include "mesh.h"
#include "ui_utils.h"
#include "io.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader/shader.h"

//lyq空间负责主窗口的创建、imgui的绘制及imgui上相关设置变量的访问
 namespace lyq{
    //imgui页面的一些设置变量

    bool* show_demo_window = new bool(true);
    bool* show_another_window = new bool(false);


    ImVec4 *clear_color = new ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 *vcolor1 = new ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 *vcolor2 = new ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 *vcolor3 = new ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    GLFWwindow* create_glfw_window(){
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
                const char* glsl_version = "#version 130";
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
                //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
                //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
        #endif

        GLFWwindow* window = glfwCreateWindow(1280, 720, "MyMeshLab", NULL, NULL);
        if (window == NULL){
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // Enable vsync

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
        }
        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }
        return window;
    }

    bool selected_model = false;//标记是否打开模型
    TCHAR *szFileName;//Mesh File

    float world_radians = -55.;
    float world_vec[3] = {1., 0., 0.};
    float world_vec2[3] = {0., 0., 0.};

    float camera_position[3] = {0., 0., 3.};
    float camera_lookat[3] = {0., 0., 0.};
    float camera_up[3] = {0., 1., 0.};

    float projection_near = 0.1;
    float projection_far = 100.;
    float projection_fov = 45.f;

    bool set_camera_look_at_0 = false;

    void render_imgui(){
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
            ImGui::ColorEdit3("background color", (float*)clear_color);
            ImGui::ColorEdit3("vertex color 1", (float*)vcolor1);
            ImGui::ColorEdit3("vertex color 2", (float*)vcolor2);
            ImGui::ColorEdit3("vertex color 3", (float*)vcolor3);

            if (ImGui::Button("Button"))
                counter++;

            if (ImGui::Button("Open File"))
            {
                szFileName = open_file_dialog();

                _tprintf("selected: %s\n", szFileName);
                selected_model = true;
                std::vector<double> V, P; // V: 3*vn, P: 4*pn.
                std::vector<size_t> M; // M: 3*fn.
                read_mesh(szFileName, V, M);

            }
            if(selected_model){
                ImGui::Text("Selected file: %s\n", szFileName);
            }

            ImGui::Text("Here set model, view, projection matrix");
            ImGui::Text("Here set model matrix(local space -> world space)");

            ImGui::SliderFloat3("set direction of model matrix", world_vec, 0., 1.);
            ImGui::SliderFloat("set radians(0-360) of the given direction", &world_radians, -360, 360);
            ImGui::SliderFloat3("set object translation", world_vec2, -5, 5);
            ImGui::Text("Here set view matrix(world space -> view space)");
            ImGui::SliderFloat3("set camera position", camera_position, -10., 10.);
            ImGui::SliderFloat3("set camera look at position(0,0,0 or free for fps camera)", camera_lookat, -10., 10.);
            ImGui::SliderFloat3("set camera up position(0,1,0 for almost all cases)", camera_up, -10., 10.);
            ImGui::Text("Here set projection matrix(view space -> clip space)");
            ImGui::SliderFloat("set near", &projection_near, 0, 1);
            ImGui::SliderFloat("set far", &projection_far, 0, 100);
            ImGui::SliderFloat("set fov", &projection_fov, 0, 180);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", show_another_window);
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                *show_another_window = false;
            ImGui::End();
        }
        // Rendering
        ImGui::Render();
    }


     float cameraFront[3] = {0., 0., -1.};
     void processInput(GLFWwindow *window)
     {
         float cameraSpeed = 0.05f; // adjust accordingly
         if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
             camera_position[0] += cameraSpeed * cameraFront[0];
             camera_position[1] += cameraSpeed * cameraFront[1];
             camera_position[2] += cameraSpeed * cameraFront[2];
         }
         if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
             camera_position[0] -= cameraSpeed * cameraFront[0];
             camera_position[1] -= cameraSpeed * cameraFront[1];
             camera_position[2] -= cameraSpeed * cameraFront[2];
         }
         if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
             auto temp = glm::normalize(glm::cross(glm::vec3(cameraFront[0], cameraFront[1], cameraFront[2]), glm::vec3(camera_up[0], camera_up[1], camera_up[2]))) * cameraSpeed;
             camera_position[0] -= temp.x;
             camera_position[1] -= temp.y;
             camera_position[2] -= temp.z;
         }
         if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
             auto temp = glm::normalize(glm::cross(glm::vec3(cameraFront[0], cameraFront[1], cameraFront[2]), glm::vec3(camera_up[0], camera_up[1], camera_up[2]))) * cameraSpeed;
             camera_position[0] += temp.x;
             camera_position[1] += temp.y;
             camera_position[2] += temp.z;
         }

     }
}

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
    GLFWwindow* window = lyq::create_glfw_window();

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
    float vertices[] = {
            // 位置              // 颜色
            0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右上角
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // 右下角
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,    // 左下角
            -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // 左上角
    };

    unsigned int indices[] = {
            // 注意索引从0开始!
            // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
            // 这样可以由下标代表顶点组合成矩形

            0, 1, 3, // 第一个三角形
            1, 2, 3  // 第二个三角形
    };
    unsigned int VBO, VAO, EBO;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {

        lyq::render_imgui();
        vertices[3] = lyq::vcolor1->x * lyq::vcolor1->w;
        vertices[4] = lyq::vcolor1->y * lyq::vcolor1->w;
        vertices[5] = lyq::vcolor1->z * lyq::vcolor1->w;

        vertices[9] = lyq::vcolor2->x * lyq::vcolor2->w;
        vertices[10] = lyq::vcolor2->y * lyq::vcolor2->w;
        vertices[11] = lyq::vcolor2->z * lyq::vcolor2->w;

        vertices[15] = lyq::vcolor3->x * lyq::vcolor3->w;
        vertices[16] = lyq::vcolor3->y * lyq::vcolor3->w;
        vertices[17] = lyq::vcolor3->z * lyq::vcolor3->w;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


        // 位置属性
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // 颜色属性
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
        glEnableVertexAttribArray(1);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        //background
        glClearColor(lyq::clear_color->x * lyq::clear_color->w, lyq::clear_color->y * lyq::clear_color->w, lyq::clear_color->z * lyq::clear_color->w, lyq::clear_color->w);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        ourShader.use();

        // create transformations
        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 projection    = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(lyq::world_radians), glm::vec3(lyq::world_vec[0], lyq::world_vec[1], lyq::world_vec[2]));
        model = glm::translate(model, glm::vec3(lyq::world_vec2[0], lyq::world_vec2[1], lyq::world_vec2[2]));


        if(lyq::set_camera_look_at_0 == false) {//for fps camera
            lyq::camera_lookat[0] = lyq::camera_position[0] + lyq::cameraFront[0];
            lyq::camera_lookat[1] = lyq::camera_position[1] + lyq::cameraFront[1];
            lyq::camera_lookat[2] = lyq::camera_position[2] + lyq::cameraFront[2];
        }

        glm::mat4 view = glm::lookAt(glm::vec3(lyq::camera_position[0], lyq::camera_position[1], lyq::camera_position[2]),
                           glm::vec3(lyq::camera_lookat[0], lyq::camera_lookat[1], lyq::camera_lookat[2]),
                           glm::vec3(lyq::camera_up[0], lyq::camera_up[1], lyq::camera_up[2]));

        projection = glm::perspective(glm::radians(lyq::projection_fov), (float)display_w / (float)display_h, lyq::projection_near, lyq::projection_far);
        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc  = glGetUniformLocation(ourShader.ID, "view");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("projection", projection);

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
        lyq::processInput(window);
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

