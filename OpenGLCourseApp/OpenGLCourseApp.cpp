#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

#include <iostream>

// Window dimensions
const GLint SCR_WIDTH = 800;
const GLint SCR_HEIGHT = 600;

GLuint VAO, VBO, shaderProgram;

// Vertex shader
static const char* vShader = "      \n\
#version 330                        \n\
                                    \n\
layout (location = 0) in vec3 pos;  \n\
                                    \n\
void main ()                        \n\
{                                   \n\
    gl_Position = vec4(pos, 1.0);   \n\
}                                   \n\
";

// Fragment shader
static const char* fShader = "          \n\
#version 330                            \n\
                                        \n\
out vec4 color;                         \n\
                                        \n\
void main ()                            \n\
{                                       \n\
    color = vec4(1.0, 0.0, 0.0, 1.0);   \n\
}                                       \n\
";

void createTriangle()
{
    GLfloat vertices[] = {
        -.4f, -.4f, 0.f,
        .4f, -.4f, 0.f,
        0.f, .4f, 0.f,
    };

    // Create vertex array object
    // holds many VBO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // Create vertex buffer object
    // defines where the triangles are
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Connect buffer data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Enable vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AddShader(GLuint program, const char* code, GLenum type)
{
    GLuint shader = glCreateShader(type);

    const GLchar* theCode[1];
    theCode[0] = code;

    GLint codeLength[1];
    codeLength[0] = strlen(code);

    glShaderSource(shader, 1, theCode, codeLength);
    glCompileShader(shader);
    
    GLint result = 0;
    GLchar errorLog[1024] = { 0 };

    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        glGetShaderInfoLog(shader, sizeof(errorLog), NULL, errorLog);
        std::cerr << "ERROR COMPILING " << type << " SHADER::" << errorLog << std::endl;
        return;
    }

    glAttachShader(program, shader);
}

void CompileShaders() {
    shaderProgram = glCreateProgram();

    if (!shaderProgram)
    {
        std::cerr << "Error creating shader program\n";
        return;
    }

    AddShader(shaderProgram, vShader, GL_VERTEX_SHADER);
    AddShader(shaderProgram, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar errorLog[1024] = { 0 };

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
        std::cerr << "ERROR LINKING PROGRAM::" << errorLog << std::endl;
        return;
    }

    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
        std::cerr << "ERROR VALIDATING PROGRAM::" << errorLog << std::endl;
        return;
    }
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "GLFW initialization failed\n";
        return 1;
    }

    // Setup GLFW window properties
    // OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    // Core profile, no backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

    GLFWwindow* mainWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Open GL Course App", NULL, NULL);
    if (!mainWindow)
    {
        std::cerr << "GLFW window creation failed\n";
        return 1;
    }

    // Get buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW initialization failed\n";
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    createTriangle();
    CompileShaders();

    // Loop until window closed
    while (!glfwWindowShouldClose(mainWindow))
    {
        // Get and handle user inptu events
        glfwPollEvents();

        // Clear the window
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);


        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}