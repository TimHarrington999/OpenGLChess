#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// function headers
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// screen settings
const unsigned int SCR_WIDTH = 2000;
const unsigned int SCR_HEIGHT = 2000;

// shader sources
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
const char *blackFragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
    "}\n\0";

int main()
{
    // ##############################
    // GLFW and GLAD
    // ##############################
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ##############################
    // Shaders
    // ##############################

    // __vertex shader__
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // __fragment shader__
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //__black fragment shader__
    unsigned int blackShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(blackShader, 1, &blackFragmentShaderSource, NULL);
    glCompileShader(blackShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glGetShaderiv(blackShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(blackShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // __link shaders__
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // __link board shaders__
    unsigned int boardShaderProgram = glCreateProgram();
    glAttachShader(boardShaderProgram, vertexShader);
    glAttachShader(boardShaderProgram, blackShader);
    glLinkProgram(boardShaderProgram);

    // check for linking errors
    glGetProgramiv(boardShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(boardShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(blackShader);

    // ##############################
    // Buffers
    // ##############################

    // raw normalized vertices for our chess board
    // there's probably a better way to do this but screw it
    float blackSquares[] = {
        -0.75f, 1.0f, 0.0f, // row 1
        -0.50f, 1.0f, 0.0f, 
        -0.25f, 1.0f, 0.0f, 
        0.0f, 1.0f, 0.0f,
        0.25f, 1.0f, 0.0f,
        0.5f, 1.0f, 0.0f,
        0.75f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, 0.75f, 0.0f, // row 2
        -0.75f, 0.75f, 0.0f, 
        -0.50f, 0.75f, 0.0f, 
        -0.25f, 0.75f, 0.0f, 
        0.0f, 0.75f, 0.0f,
        0.25f, 0.75f, 0.0f,
        0.5f, 0.75f, 0.0f,
        0.75f, 0.75f, 0.0f,
        1.0f, 0.75f, 0.0f,
        -1.0f, 0.5f, 0.0f, // row 3
        -0.75f, 0.5f, 0.0f, 
        -0.50f, 0.5f, 0.0f, 
        -0.25f, 0.5f, 0.0f, 
        0.0f, 0.5f, 0.0f,
        0.25f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.75f, 0.5f, 0.0f,
        1.0f, 0.5f, 0.0f,
        -1.0f, 0.25f, 0.0f, // row 4
        -0.75f, 0.25f, 0.0f, 
        -0.50f, 0.25f, 0.0f, 
        -0.25f, 0.25f, 0.0f, 
        0.0f, 0.25f, 0.0f,
        0.25f, 0.25f, 0.0f,
        0.5f, 0.25f, 0.0f,
        0.75f, 0.25f, 0.0f,
        1.0f, 0.25f, 0.0f,
        -1.0f, 0.0f, 0.0f, // row 5
        -0.75f, 0.0f, 0.0f, 
        -0.50f, 0.0f, 0.0f, 
        -0.25f, 0.0f, 0.0f, 
        0.0f, 0.0f, 0.0f,
        0.25f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.75f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        -1.0f, -0.25f, 0.0f, // row 6
        -0.75f, -0.25f, 0.0f, 
        -0.50f, -0.25f, 0.0f, 
        -0.25f, -0.25f, 0.0f, 
        0.0f, -0.25f, 0.0f,
        0.25f, -0.25f, 0.0f,
        0.5f, -0.25f, 0.0f,
        0.75f, -0.25f, 0.0f,
        1.0f, -0.25f, 0.0f,
        -1.0f, -0.5f, 0.0f, // row 7
        -0.75f, -0.5f, 0.0f, 
        -0.50f, -0.5f, 0.0f, 
        -0.25f, -0.5f, 0.0f, 
        0.0f, -0.5f, 0.0f,
        0.25f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.75f, -0.5f, 0.0f,
        1.0f, -0.5f, 0.0f,
        -1.0f, -0.75f, 0.0f, // row 8
        -0.75f, -0.75f, 0.0f, 
        -0.50f, -0.75f, 0.0f, 
        -0.25f, -0.75f, 0.0f, 
        0.0f, -0.75f, 0.0f,
        0.25f, -0.75f, 0.0f,
        0.5f, -0.75f, 0.0f,
        0.75f, -0.75f, 0.0f,
        1.0f, -0.75f, 0.0f,
        -1.0f, -1.0f, 0.0f, // row 9
        -0.75f, -1.0f, 0.0f, 
        -0.50f, -1.0f, 0.0f, 
        -0.25f, -1.0f, 0.0f, 
        0.0f, -1.0f, 0.0f,
        0.25f, -1.0f, 0.0f,
        0.5f, -1.0f, 0.0f,
        0.75f, -1.0f, 0.0f
    };

    // ___INDICES GO HERE___
    unsigned int blackSquareIndices[] = {
        0, 1, 9, 
        1, 9, 10, // sq 1
        2, 3, 11, 
        3, 11, 12,
        4, 5, 13, 
        5, 13, 14,
        6, 7, 15,
        7, 15, 16,
        8, 9, 17, 
        9, 17, 18, // sq 5
        10, 11, 19, 
        11, 19, 20,
        12, 13, 21, 
        13, 21, 22,
        14, 15, 23, 
        15, 23, 24,
        18, 19, 27, 
        19, 27, 28, // sq 9
        20, 21, 29, 
        21, 29, 30,
        22, 23, 31, 
        23, 31, 32,
        24, 25, 33, 
        25, 33, 34,
        26, 27, 35, 
        27, 35, 36, // sq 13
        28, 29, 37, 
        29, 37, 38,
        30, 31, 39, 
        31, 39, 40,
        32, 33, 41, 
        33, 41, 42,
        36, 37, 45, 
        37, 45, 46, // sq 17
        38, 39, 47, 
        39, 47, 48,
        40, 41, 49, 
        41, 49, 50,
        42, 43, 51, 
        43, 51, 52,
        44, 45, 53, 
        45, 53, 54, // sq 21
        46, 47, 55, 
        47, 55, 56,
        48, 49, 57, 
        49, 57, 58,
        50, 51, 59, 
        51, 59, 60,
        54, 55, 63, 
        55, 63, 64, // sq 25
        56, 57, 65, 
        57, 65, 66,
        58, 59, 67, 
        59, 67, 68,
        60, 61, 69, 
        61, 69, 70,
        62, 63, 71, 
        63, 71, 72, // sq 29
        64, 65, 73, 
        65, 73, 74,
        66, 67, 75, 
        67, 75, 76,
        68, 69, 77, 
        69, 77, 78
    };

    unsigned int BoardVBO, BoardVAO, BoardEBO;
    glGenVertexArrays(1, &BoardVAO);
    glGenBuffers(1, &BoardVBO);
    glGenBuffers(1, &BoardEBO);
    glBindVertexArray(BoardVAO);

    glBindBuffer(GL_ARRAY_BUFFER, BoardVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(blackSquares), blackSquares, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BoardEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(blackSquareIndices), blackSquareIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // ##############################
    // Render Loop
    // ##############################
    while (!glfwWindowShouldClose(window))
    {
        // input
        // _________
        processInput(window);

        // render
        // _________

        // create the board squares

        glUseProgram(boardShaderProgram);
        glBindVertexArray(BoardVAO);
        glDrawElements(GL_TRIANGLES, 192, GL_UNSIGNED_INT, 0);

        /* // background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); */

        // draw rectangle
        //glUseProgram(shaderProgram);
        //glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* // draw two neighboring triangles
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram2);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3); */
 
        // Swap buffers and poll for events
        // __________
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // once the window is requested to be closed, close out buffers and terminate
    glDeleteVertexArrays(1, &BoardVAO);
    glDeleteBuffers(1, &BoardVBO);
    glDeleteBuffers(1, &BoardEBO);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(boardShaderProgram);

    glfwTerminate();
    return 0;
}

// process keyboard input
void processInput(GLFWwindow *window)
{
    // pressing esc key will close the window and the application
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// setup callback for window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}