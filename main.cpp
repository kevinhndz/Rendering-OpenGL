#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>

// Datos de posicion y color.
struct Vec3  { float x, y, z; };
struct Color { float r, g, b; };

// Aqui se guardan posiciones y colores.
std::vector<float> datos;

// Ajusta el brillo de un color.
Color brillo(Color c, float k) {
    return { c.r * k, c.g * k, c.b * k };
}

// Agrega un triangulo.
void tri(Vec3 a, Vec3 b, Vec3 c, Color col) {
    Vec3 p[3] = { a, b, c };
    for (int i = 0; i < 3; i++) {
        datos.push_back(p[i].x);
        datos.push_back(p[i].y);
        datos.push_back(p[i].z);
        datos.push_back(col.r);
        datos.push_back(col.g);
        datos.push_back(col.b);
    }
}

// Forma un cuadrilatero con dos triangulos.
void quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Color col) {
    tri(a, b, c, col);
    tri(a, c, d, col);
}

// Forma una caja con seis lados.
void caja(float x0, float x1, float y0, float y1,
          float z0, float z1, Color col) {
    Vec3 a{x0, y0, z0}, b{x1, y0, z0}, c{x1, y1, z0}, d{x0, y1, z0}; // atras
    Vec3 e{x0, y0, z1}, f{x1, y0, z1}, g{x1, y1, z1}, h{x0, y1, z1}; // adelante

    quad(e, f, g, h, brillo(col, 1.0f)); // adelante
    quad(a, b, c, d, brillo(col, 0.6f)); // atras
    quad(a, e, h, d, brillo(col, 0.8f)); // izquierda
    quad(b, f, g, c, brillo(col, 0.8f)); // derecha
    quad(d, c, g, h, brillo(col, 0.9f)); // arriba
    quad(a, b, f, e, brillo(col, 0.5f)); // abajo
}

// Forma un prisma triangular.
void prisma(Vec3 a, Vec3 b, Vec3 c, Vec3 desp, Color col) {
    Vec3 a2{a.x + desp.x, a.y + desp.y, a.z + desp.z};
    Vec3 b2{b.x + desp.x, b.y + desp.y, b.z + desp.z};
    Vec3 c2{c.x + desp.x, c.y + desp.y, c.z + desp.z};

    tri(a, b, c,    brillo(col, 1.0f));
    tri(a2, b2, c2, brillo(col, 0.6f));
    quad(a, b, b2, a2, brillo(col, 0.8f));
    quad(b, c, c2, b2, brillo(col, 0.7f));
    quad(c, a, a2, c2, brillo(col, 0.9f));
}

// Forma una piramide.
void piramide(float x0, float x1, float z0, float z1, float yb,
              Vec3 punta, Color col) {
    Vec3 a{x0, yb, z0}, b{x1, yb, z0}, c{x1, yb, z1}, d{x0, yb, z1};

    tri(a, b, punta, brillo(col, 0.6f));
    tri(b, c, punta, brillo(col, 0.8f));
    tri(c, d, punta, brillo(col, 1.0f));
    tri(d, a, punta, brillo(col, 0.8f));
    quad(a, b, c, d, brillo(col, 0.5f)); // base
}

// Aqui se arma la nave.
void construirNave() {
    Color gris   {0.75f, 0.75f, 0.80f};
    Color rojo   {0.90f, 0.10f, 0.10f};
    Color azul   {0.15f, 0.35f, 0.90f};
    Color celeste{0.40f, 0.85f, 1.00f};
    Color naranja{1.00f, 0.55f, 0.00f};

    // Cuerpo
    caja(-0.15f, 0.15f, -0.40f, 0.30f, -0.15f, 0.15f, gris);

    // Punta
    piramide(-0.15f, 0.15f, -0.15f, 0.15f, 0.30f, {0.0f, 0.65f, 0.0f}, rojo);

    // Alas
    prisma({-0.15f, -0.10f, 0.03f}, {-0.15f, -0.40f, 0.03f},
           {-0.55f, -0.50f, 0.03f}, {0.0f, 0.0f, -0.06f}, azul);
    prisma({ 0.15f, -0.10f, 0.03f}, { 0.15f, -0.40f, 0.03f},
           { 0.55f, -0.50f, 0.03f}, {0.0f, 0.0f, -0.06f}, azul);

    // Aletas
    prisma({-0.03f, -0.10f,  0.15f}, {-0.03f, -0.40f,  0.15f},
           {-0.03f, -0.50f,  0.55f}, {0.06f, 0.0f, 0.0f}, azul);
    prisma({-0.03f, -0.10f, -0.15f}, {-0.03f, -0.40f, -0.15f},
           {-0.03f, -0.50f, -0.55f}, {0.06f, 0.0f, 0.0f}, azul);

    // Ventanas
    caja(-0.07f, 0.07f, 0.05f, 0.19f,  0.15f,  0.18f, celeste);
    caja(-0.07f, 0.07f, 0.05f, 0.19f, -0.18f, -0.15f, celeste);

    // Motor
    piramide(-0.10f, 0.10f, -0.10f, 0.10f, -0.40f, {0.0f, -0.70f, 0.0f}, naranja);
}

// Shaders para dibujar y colorear.
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 posicion;
layout (location = 1) in vec3 colorEntrada;

out vec3 colorVertice;

uniform float anguloY; // giro
uniform float anguloX; // inclinacion
uniform float aspecto; // proporcion de la ventana

void main() {
    vec3 p = posicion;

    // Giro horizontal.
    float cy = cos(anguloY);
    float sy = sin(anguloY);
    p = vec3(cy * p.x + sy * p.z, p.y, -sy * p.x + cy * p.z);

    // Inclinacion.
    float cx = cos(anguloX);
    float sx = sin(anguloX);
    p = vec3(p.x, cx * p.y - sx * p.z, sx * p.y + cx * p.z);

    // Aleja la nave.
    p.z -= 3.0;

    // Ajusta la perspectiva.
    float n = 0.1;
    float lejos = 10.0;
    float f = 1.0 / tan(radians(45.0) / 2.0);

    gl_Position = vec4(
        p.x * f / aspecto,
        p.y * f,
        -(lejos + n) / (lejos - n) * p.z - 2.0 * lejos * n / (lejos - n),
        -p.z
    );

    colorVertice = colorEntrada;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec3 colorVertice;
out vec4 color;

void main() {
    color = vec4(colorVertice, 1.0);
}
)";

// Prepara un shader.
unsigned int compilar(GLenum tipo, const char* fuente) {
    unsigned int shader = glCreateShader(tipo);
    glShaderSource(shader, 1, &fuente, nullptr);
    glCompileShader(shader);

    int ok;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "Error en shader:\n" << log << "\n";
    }
    return shader;
}

void ajustarVista(GLFWwindow*, int ancho, int alto) {
    glViewport(0, 0, ancho, alto);
}

// Inicio del programa.
int main() {
    if (!glfwInit()) {
        std::cerr << "No se pudo iniciar GLFW\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* ventana = glfwCreateWindow(
        800, 600, "Nave espacial 3D", nullptr, nullptr
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

    // Activa la profundidad 3D.
    glEnable(GL_DEPTH_TEST);

    // Prepara la nave.
    construirNave();
    int totalVertices = (int)(datos.size() / 6);

    // Prepara los datos para OpenGL.
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, datos.size() * sizeof(float),
                 datos.data(), GL_STATIC_DRAW);

    // Atributo de posicion.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Atributo de color.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Prepara los shaders.
    unsigned int vertexShader   = compilar(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = compilar(GL_FRAGMENT_SHADER, fragmentShaderSource);

    unsigned int programa = glCreateProgram();
    glAttachShader(programa, vertexShader);
    glAttachShader(programa, fragmentShader);
    glLinkProgram(programa);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    int ubicAnguloY = glGetUniformLocation(programa, "anguloY");
    int ubicAnguloX = glGetUniformLocation(programa, "anguloX");
    int ubicAspecto = glGetUniformLocation(programa, "aspecto");

    // Ciclo de dibujo.
    while (!glfwWindowShouldClose(ventana)) {
        if (glfwGetKey(ventana, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(ventana, GLFW_TRUE);
        }

        glClearColor(0.02f, 0.02f, 0.10f, 1.0f);
        // Limpia la profundidad.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Usa el tiempo para girar.
        float tiempo = (float)glfwGetTime();

        glfwGetFramebufferSize(ventana, &ancho, &alto);
        float aspecto = (alto > 0) ? (float)ancho / (float)alto : 1.0f;

        glUseProgram(programa);
        glUniform1f(ubicAnguloY, tiempo); // giro
        glUniform1f(ubicAnguloX, 0.35f); // inclinacion
        glUniform1f(ubicAspecto, aspecto);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, totalVertices);

        glfwSwapBuffers(ventana);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(programa);

    glfwTerminate();
    return 0;
}