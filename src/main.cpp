#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "shader.h"
#include "compute_shader.h"

#include "scene.h"

#define SCR_WIDTH 1920
#define SCR_HEIGHT 1080

unsigned int quadVAO = 0;
unsigned int quadVBO;

void render_quad() {
    if (quadVAO == 0) {
        float quadVertices[] = {
                // positions        // texture Coords
                -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };

        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

const unsigned int TEXTURE_WIDTH = SCR_WIDTH, TEXTURE_HEIGHT = SCR_HEIGHT;

GLuint verticesSSBO;

struct glsl_vec {
    glm::vec4 pos;
};

void ssbo_vertices(const scene& s) {
    auto vertices = s.vertices();
    std::vector<glsl_vec> glsl_vertices;
    for (const auto& v : vertices) {
        glsl_vertices.push_back(glsl_vec{glm::vec4(v, 1.0f)});
    }

    glGenBuffers(1, &verticesSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, verticesSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, vertices.size() * sizeof(glsl_vec), glsl_vertices.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, verticesSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void init_scene(const std::string& filename) {
    scene s(filename);
    ssbo_vertices(s);
}

int main()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Path Tracing", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    shader s("../shaders/screen_quad.vs", "../shaders/screen_quad.fs");
    compute_shader cs("../shaders/path_tracer.cs");

    s.use();
    s.set_int("tex", 0);

    unsigned int texture;

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);

    glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    init_scene("../resources/test.obj");

    int cnt = 0;
    while (!glfwWindowShouldClose(window))
    {
        cnt += 1;
        cs.use();
        cs.set_float("time", cnt);
        cs.set_int("frame", cnt);
        glDispatchCompute((unsigned int)SCR_WIDTH/16, (unsigned int)SCR_HEIGHT/16, 1);

        // make sure writing to image has finished before read
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        // render image to quad
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        s.use();
        render_quad();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteTextures(1, &texture);
    glDeleteProgram(s.id);
    glDeleteProgram(cs.id);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}