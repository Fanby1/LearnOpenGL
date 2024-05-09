#include "CWindow.h"
#include <iostream>


CWindow::CWindow(unsigned int vWidth, unsigned int vHeight): m_Width(vWidth), m_Height(vHeight)
{
    // glfw window creation
    // --------------------
    m_Window = glfwCreateWindow(m_Width, m_Height, "LearnOpenGL", NULL, NULL);
    if (m_Window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_Window);
    glfwSetFramebufferSizeCallback(m_Window, _framebuffer_size_callback);
}

void CWindow::render(std::vector<unsigned int> vShaderProgram, std::vector<unsigned int> vVAO)
{
    while (!glfwWindowShouldClose(m_Window))
    {
        // input
        // -----
        _processInput();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        // ..:: Drawing code (in render loop) :: ..
        for (int i = 0; i < vShaderProgram.size(); i++) {
            glUseProgram(vShaderProgram[i]);
            glBindVertexArray(vVAO[i]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
        // glBindVertexArray(0); // no need to unbind it every time 

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void CWindow::_processInput()
{
    if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_Window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void CWindow::_framebuffer_size_callback(GLFWwindow* window, int vWidth, int vHeight)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, vWidth, vHeight);
}
