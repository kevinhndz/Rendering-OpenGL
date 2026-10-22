#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void ajustarVista(GLFWwindow*, int ancho, int alto) {
    glViewport(0, 0, ancho, alto);
}

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 posicion;

void main() {
    gl_Position = vec4(posicion, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 color;

void main() {
    color = vec4(0.95, 0.55, 0.12, 1.0);
}
)";

int main() {
    if (!glfwInit()) {
        std::cerr << "No se pudo iniciar GLFW\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* ventana = glfwCreateWindow(
        800, 600, "Rectangulo", nullptr, nullptr
    );

    if (!ventana) {
        std::cerr << "No se pudo crear la ventana\n";
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(ventana);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "No se pudo iniciar GLAD\n";
        glfwTerminate();
        return 1;
    }

    glfwSetFramebufferSizeCallback(ventana, ajustarVista);

    int ancho, alto;
    glfwGetFramebufferSize(ventana, &ancho, &alto);
    glViewport(0, 0, ancho, alto);

    float vertices[] = {
        -0.65f,  0.40f, 0.0f,
         0.65f,  0.40f, 0.0f,
         0.65f, -0.40f, 0.0f,
        -0.65f,  0.40f, 0.0f,
         0.65f, -0.40f, 0.0f,
        -0.65f, -0.40f, 0.0f
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
                 vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    unsigned int programa = glCreateProgram();
    glAttachShader(programa, vertexShader);
    glAttachShader(programa, fragmentShader);
    glLinkProgram(programa);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    while (!glfwWindowShouldClose(ventana)) {
        if (glfwGetKey(ventana, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(ventana, GLFW_TRUE);
        }

        glClearColor(0.08f, 0.25f, 0.36f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(programa);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(ventana);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(programa);
    glfwTerminate();
    return 0;
}
