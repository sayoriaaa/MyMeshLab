//
// Created by sayori on 2023/4/27.
//

#ifndef MYMESHLAB_DEMO_H
#define MYMESHLAB_DEMO_H

//here is some demo
#include "header.h"
#include "front.h"
#include "shader/shader.h"
#include "mesh.h"
namespace demo{
    bool use_demo = false;
    int demo_index = 0;

    namespace quad{
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

        Shader compile_shader(std::string projdir){
            //编译shader
            //气死我啦，这里一定得传绝对地址，而且xmake也没能用预定义宏把projdir传进来，暂时只能在前面手动设一下
            std::string vdir = "src/shader/quad_demo/shader.vs";
            std::string fdir = "src/shader/quad_demo/shader.fs";
            vdir = projdir+vdir;
            fdir = projdir+fdir;
            auto ret = new Shader(vdir.c_str(), fdir.c_str());
            return *ret;
        }

        void buff_shader(){
            vertices[3] = front::quad::vcolor1->x * front::quad::vcolor1->w;
            vertices[4] = front::quad::vcolor1->y * front::quad::vcolor1->w;
            vertices[5] = front::quad::vcolor1->z * front::quad::vcolor1->w;

            vertices[9] = front::quad::vcolor2->x * front::quad::vcolor2->w;
            vertices[10] = front::quad::vcolor2->y * front::quad::vcolor2->w;
            vertices[11] = front::quad::vcolor2->z * front::quad::vcolor2->w;

            vertices[15] = front::quad::vcolor3->x * front::quad::vcolor3->w;
            vertices[16] = front::quad::vcolor3->y * front::quad::vcolor3->w;
            vertices[17] = front::quad::vcolor3->z * front::quad::vcolor3->w;

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
        }

        void draw_background(){
            glClearColor(front::quad::clear_color->x * front::quad::clear_color->w, front::quad::clear_color->y * front::quad::clear_color->w, front::quad::clear_color->z * front::quad::clear_color->w, front::quad::clear_color->w);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        void shade(Shader ourShader){
            unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
            unsigned int viewLoc  = glGetUniformLocation(ourShader.ID, "view");
            // pass them to the shaders (3 different ways)
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cam::model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &cam::view[0][0]);
            // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
            ourShader.setMat4("projection", cam::projection);

            glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
            //glDrawArrays(GL_TRIANGLES, 0, 3);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        void release_buff(){
            // optional: de-allocate all resources once they've outlived their purpose:
            // ------------------------------------------------------------------------
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
        }
    }

    namespace main{

        unsigned int VBO, VAO, EBO;

        Shader compile_shader(std::string projdir){
            //编译shader
            //气死我啦，这里一定得传绝对地址，而且xmake也没能用预定义宏把projdir传进来，暂时只能在前面手动设一下
            std::string vdir = "src/shader/mesh/shader.vs";
            std::string fdir = "src/shader/mesh/shader.fs";
            vdir = projdir+vdir;
            fdir = projdir+fdir;
            auto ret = new Shader(vdir.c_str(), fdir.c_str());
            return *ret;
        }

        void buff_shader(){

            auto vertices = mesh::mesh_buffer[mesh::cur_mesh]->vertices;
            auto indices = mesh::mesh_buffer[mesh::cur_mesh]->indices;

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(double), &vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(std::size_t), &indices[0], GL_STATIC_DRAW);


            // 位置属性
            glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)0);
            glEnableVertexAttribArray(0);

            // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
            // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
            glBindVertexArray(0);

            /*
            // 绑定VBO，将其映射到系统内存中并打印数据
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            float* vertexData = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);

            if (vertexData != NULL) {
                for (int i = 0; i < mesh::vertices.size(); i+=3) {
                    printf("Vertex %d: (%f, %f, %f)\n", i, vertexData[3*i], vertexData[3*i+1], vertexData[3*i+2]);
                }

                // 解除映射
                glUnmapBuffer(GL_ARRAY_BUFFER);
            }
             */

        }

        void draw_background(){
            glClearColor(front::mesh::clear_color->x * front::mesh::clear_color->w, front::mesh::clear_color->y * front::mesh::clear_color->w, front::mesh::clear_color->z * front::mesh::clear_color->w, front::mesh::clear_color->w);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        void shade(Shader ourShader){

            auto vertices = mesh::mesh_buffer[mesh::cur_mesh]->vertices;
            auto indices = mesh::mesh_buffer[mesh::cur_mesh]->indices;


            unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
            unsigned int viewLoc  = glGetUniformLocation(ourShader.ID, "view");
            unsigned int colorLoc  = glGetUniformLocation(ourShader.ID, "OurColor");
            // pass them to the shaders (3 different ways)
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cam::model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &cam::view[0][0]);

            glUniform4f(colorLoc, front::mesh::mesh_color->x, front::mesh::mesh_color->y, front::mesh::mesh_color->z, front::mesh::mesh_color->w);
            // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
            ourShader.setMat4("projection", cam::projection);

            glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
            //glDrawArrays(GL_TRIANGLES, 0, mesh::vertices.size()/3);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            if(front::mesh::use_wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        void release_buff(){
            // optional: de-allocate all resources once they've outlived their purpose:
            // ------------------------------------------------------------------------
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
        }
    }

}
#endif //MYMESHLAB_DEMO_H
