#include <iostream>

#include "scene.h"

int main() {
    triangle trig1 = triangle(vector3(0, 0, 0), vector3(1, 0, 0), vector3(0, 1, 0));
    triangle trig2 = triangle(vector3(0, 0, 0), vector3(1, 0, 0), vector3(0, 0, 1));

//    std::vector<triangle> triangles = {trig1, trig2};
//    bvh hierarchy(triangles);

    scene scene("../resources/car.obj");
    bvh hierarchy = scene.get_bvh();

    hierarchy.build();
    hierarchy.print();

    auto boxes = hierarchy.serialize();
    bvh::aabb_boxes_save_obj(boxes, "../resources/car_boxes.obj");

//    scene scene("../resources/teapot.obj");
//    bvh hierarchy = scene.get_bvh();
//
//    hierarchy.build();
//    hierarchy.print();

    return 0;
}

//#include <stack>
//
//#include "scene.h"
//
//int main() {
//    scene scene("../resources/teapot.obj");
//
//    bvh x = scene.get_bvh();
//    bvh_node* root = x.build();
//
//    std::vector<aabb> boxes;
//
//    std::stack<bvh_node*> stack;
//
//    stack.push(root);
//
//    while (!stack.empty()) {
//        bvh_node* node = stack.top();
//        stack.pop();
//
//        boxes.push_back(node->box);
//
//        if (node->left) {
//            stack.push(node->left);
//        }
//
//        if (node->right) {
//            stack.push(node->right);
//        }
//    }
//
//    std::cout << boxes.size() << std::endl;
//
//    for (const auto& box : boxes) {
////        std::cout << "Box: (" << box.min_corner.data[0] << ", " << box.min_corner.data[1] << ", " << box.min_corner.data[2] << ") - ("
////                  << box.max_corner.data[0] << ", " << box.max_corner.data[1] << ", " << box.max_corner.data[2] << ")" << std::endl;
//    }
//
//    return 0;
//}
//
//
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <iostream>
//#include <vector>
//
//#include "scene.h"
//
//// Window dimensions
//const GLint WIDTH = 800, HEIGHT = 600;
//
//// Shaders
//const GLchar* vertexShaderSource = R"(
//    #version 330 core
//    layout (location = 0) in vec3 position;
//
//    void main() {
//        gl_Position = vec4(position.x, position.y, position.z, 4);
//    }
//)";
//
//const GLchar* fragmentShaderSource = R"(
//    #version 330 core
//    out vec4 color;
//
//    void main() {
//        color = vec4(vec3(gl_FragCoord.z), 1.0);
//    }
//)";
//
//// Function prototypes
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
//void drawMesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices);
//
//int main() {
//    // Initialize GLFW
//    if (!glfwInit()) {
//        std::cerr << "GLFW initialization failed" << std::endl;
//        return -1;
//    }
//
//    // Set OpenGL version and profile to use
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//
//    // Create a GLFW window
//    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Mesh Renderer", nullptr, nullptr);
//    if (!window) {
//        std::cerr << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//
//    // load scene
//    scene scene("../resources/teapot.obj");
//
//    // Make the OpenGL context current
//    glfwMakeContextCurrent(window);
//
//    // Initialize GLEW
//    glewExperimental = GL_TRUE;
//    if (glewInit() != GLEW_OK) {
//        std::cerr << "GLEW initialization failed" << std::endl;
//        return -1;
//    }
//
//    // Define viewport dimensions
//    glViewport(0, 0, WIDTH, HEIGHT);
//
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
//
//    glShadeModel(GL_FLAT);
//
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_TEXTURE_2D);
//
//    // Set key callback function
//    glfwSetKeyCallback(window, key_callback);
//
//    std::vector<float> vertices = scene.vertices();
//    std::vector<unsigned int> indices = scene.indices();
//
//    // Rendering loop
//    while (!glfwWindowShouldClose(window)) {
//        // Check and call events
//        glfwPollEvents();
//
//        // Clear the color buffer
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        // Draw the mesh
//        drawMesh(vertices, indices);
//
//        // Swap the buffers
//        glfwSwapBuffers(window);
//    }
//
//    // Terminate GLFW
//    glfwTerminate();
//
//    return 0;
//}
//
//// Function to handle key events
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
//    // Close the window if the escape key is pressed
//    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
//        glfwSetWindowShouldClose(window, GL_TRUE);
//    }
//}
//
//// Function to compile and link shaders
//GLuint createShaderProgram(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource) {
//    // Create vertex shader
//    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);
//
//    // Check for vertex shader compile errors
//    GLint success;
//    GLchar infoLog[512];
//    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//    if (!success) {
//        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//        std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
//    }
//
//    // Create fragment shader
//    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//
//    // Check for fragment shader compile errors
//    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//    if (!success) {
//        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//        std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
//    }
//
//    // Create shader program
//    GLuint shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    // Check for shader program linking errors
//    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//    if (!success) {
//        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
//    }
//
//    // Delete the shaders as they're linked into our program now and no longer necessary
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    return shaderProgram;
//}
//
//// Function to draw the mesh
//void drawMesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices) {
//    // Create shader program
//    GLuint shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
//
//    // Create Vertex Array Object (VAO), Vertex Buffer Object (VBO), and Element Buffer Object (EBO)
//    GLuint VAO, VBO, EBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    // Bind VAO
//    glBindVertexArray(VAO);
//
//    // Bind and set VBO data
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
//
//    // Bind and set EBO data
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
//
//    // Set vertex attribute pointers
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
//    glEnableVertexAttribArray(0);
//
//    // Use shader program
//    glUseProgram(shaderProgram);
//
//    // Draw the mesh
//    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
//
//    // Unbind VAO and shader program
//    glBindVertexArray(0);
//    glUseProgram(0);
//
//    // Delete VAO, VBO, and EBO
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteBuffers(1, &EBO);
//}
