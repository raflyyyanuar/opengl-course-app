#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

#include <iostream>

// Window dimensions
const GLint SCR_WIDTH = 600;
const GLint SCR_HEIGHT = 600;

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

    // Loop until window closed
    while (!glfwWindowShouldClose(mainWindow))
    {
        // Get and handle user inptu events
        glfwPollEvents();

        // Clear the window
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}