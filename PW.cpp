#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 0.0, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"uniform int isDayMode;\n"
"void main()\n"
"{\n"
"   if(isDayMode == 1) {\n"
"       FragColor = vec4(ourColor + vec3(0.5, 0.5, 0.5), 1.0);\n"
"   } else {\n"
"       FragColor = vec4(ourColor, 1.0);\n"
"   }\n"
"}\n\0";

bool isDay = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        isDay = !isDay;
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Night Landscape Postcard", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    float vertices[] = {
        -1.0f,  1.0f,   0.0f, 0.0f, 0.2f,
         1.0f,  1.0f,   0.0f, 0.0f, 0.2f,
        -1.0f, -0.2f,   0.1f, 0.2f, 0.5f,
         1.0f,  1.0f,   0.0f, 0.0f, 0.2f,
         1.0f, -0.2f,   0.1f, 0.2f, 0.5f,
        -1.0f, -0.2f,   0.1f, 0.2f, 0.5f,

        -1.0f, -0.2f,   0.0f, 0.3f, 0.0f,
         1.0f, -0.2f,   0.0f, 0.3f, 0.0f,
        -1.0f, -1.0f,   0.0f, 0.2f, 0.0f,
         1.0f, -0.2f,   0.0f, 0.3f, 0.0f,
         1.0f, -1.0f,   0.0f, 0.2f, 0.0f,
        -1.0f, -1.0f,   0.0f, 0.2f, 0.0f,

        -0.8f, -0.2f,   0.2f, 0.3f, 0.2f,
        -0.3f,  0.4f,   0.4f, 0.5f, 0.4f,
         0.2f, -0.2f,   0.2f, 0.3f, 0.2f,

         0.6f,  0.7f,   0.9f, 0.9f, 0.6f,

        -0.6f,  0.8f,   1.0f, 1.0f, 1.0f,
         0.2f,  0.6f,   1.0f, 1.0f, 1.0f,
         0.8f,  0.9f,   1.0f, 1.0f, 1.0f,

        -0.8f, -0.2f,   0.0f, 0.0f, 0.0f,
        -0.3f,  0.4f,   0.0f, 0.0f, 0.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    int isDayLocation = glGetUniformLocation(shaderProgram, "isDayMode");

    glEnable(GL_PROGRAM_POINT_SIZE);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glUniform1i(isDayLocation, isDay ? 1 : 0);

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawArrays(GL_TRIANGLES, 6, 6);
        glDrawArrays(GL_TRIANGLES, 12, 3);

        glPointSize(30.0f);
        glDrawArrays(GL_POINTS, 15, 1);

        glPointSize(3.0f);
        glDrawArrays(GL_POINTS, 16, 3);

        glDrawArrays(GL_LINES, 19, 2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}