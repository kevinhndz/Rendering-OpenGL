#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <vector>

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
    color = vec4(0.15, 0.75, 0.35, 1.0);
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
        800, 600, "Circulo", nullptr, nullptr
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

    const int segmentos = 100;
    const float radio = 0.5f;
    const float pi = 3.14159265358979323846f;
    std::vector<float> vertices;
    vertices.reserve((segmentos + 2) * 3);
    vertices.insert(vertices.end(), {0.0f, 0.0f, 0.0f});

    for (int i = 0; i <= segmentos; ++i) {
        float angulo = 2.0f * pi * i / segmentos;
        vertices.push_back(radio * std::cos(angulo));
        vertices.push_back(radio * std::sin(angulo));
        vertices.push_back(0.0f);
    }

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_STATIC_DRAW);
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
        glDrawArrays(GL_TRIANGLE_FAN, 0, segmentos + 2);
        glfwSwapBuffers(ventana);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(programa);
    glfwTerminate();
    return 0;
}
