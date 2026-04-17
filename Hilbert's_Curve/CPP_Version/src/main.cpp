// Group 5
// BENZIL EMMANUEL - SCT211-0463/2023
// FRANCIS MUNG'ANG'U - SCT211-0032/2023

//================= HEADERS =================

#include <glad/glad.h>  // communicates to OpenGL
#include <GLFW/glfw3.h>  // creates window + handles input
#include <vector>  // stores points dynamically
#include <iostream>  // prints errors

// ================= SHADERS =================
// Takes each point and sends it to the GPU for rendering.
const char* vertexShaderSource = R"(
layout (location = 0) in vec2 aPos;

void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

// ================== FRAGMENT SHADER ==================
// Colors the curve light blue.
const char* fragmentShaderSource = R"(
out vec4 FragColor;

void main() {
    FragColor = vec4(0.2, 0.8, 1.0, 1.0); // light blue
}
)";

// ================= SHADER UTIL =================
unsigned int compileShader(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // Error check
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "Shader Error:\n" << infoLog << std::endl;
    }

    return shader;
}

// Creates a shader program by compiling and linking vertex and fragment shaders.
unsigned int createProgram() {
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

// ================= HILBERT CURVE GENERATOR =================
void generateHilbert(std::vector<float>& vertices,
    // Parameters
                     float x, float y, // starting position
                     float xi, float xj, // direction for width
                     float yi, float yj, // direction for height
                     int order) // Level of detail

// Base Case - Stop condition                     
{
    if (order == 0) {
        float px = x + (xi + yi) / 2;
        float py = y + (xj + yj) / 2;

        vertices.push_back(px);
        vertices.push_back(py);
        return;
    }

    // Recursive case - Subdivide into 4 quadrants
    generateHilbert(vertices, x, y, yi/2, yj/2, xi/2, xj/2, order - 1);
    generateHilbert(vertices, x + xi/2, y + xj/2, xi/2, xj/2, yi/2, yj/2, order - 1);
    generateHilbert(vertices, x + xi/2 + yi/2, y + xj/2 + yj/2, xi/2, xj/2, yi/2, yj/2, order - 1);
    generateHilbert(vertices, x + xi/2 + yi, y + xj/2 + yj,
                    -yi/2, -yj/2, -xi/2, -xj/2, order - 1);
}

// ================= MAIN =================
int main() {
    // Init GLFW - Window creation + input handling
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "Hilbert Curve", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Load OpenGL
    if (!gladLoadGL()) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    // Create shader program
    unsigned int program = createProgram();

    // Generate curve
    std::vector<float> vertices;
    int order = 5;

    generateHilbert(vertices,
        -1.0f, -1.0f,   // bottom-left
         2.0f,  0.0f,   // width
         0.0f,  2.0f,   // height
         order);

    // Setup VAO & VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(float),
                 vertices.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(VAO);

        glDrawArrays(GL_LINE_STRIP, 0, vertices.size() / 2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}