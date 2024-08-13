#include "InputManager.h"

// Define the variables
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); // The point we are looking at
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float distanceToCenter = 500.0f;
float yaw = -90.0f;
float pitch = 0.0f;
float cameraSensitivity = 0.1f;
bool firstMouse = true;
double lastMouseX = 0.0;
double lastMouseY = 0.0;
float fov = 80.0f;
bool cursorLocked = true;
double lastEscapePressTime = 0;
double escapePressDelay = 0.2;  // 200 ms delay

bool wireframeMode = false; // Wireframe mode toggle
static bool rKeyPressedLastFrame = false;

void processInput(GLFWwindow* window)
{
    double currentTime = glfwGetTime();

    // Toggle cursor lock with Escape key
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        if (currentTime - lastEscapePressTime > escapePressDelay)
        {
            cursorLocked = !cursorLocked; // Toggle the cursorLocked variable

            if (!cursorLocked)
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Unlock the cursor
            }
            else
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Lock the cursor
            }

            lastEscapePressTime = currentTime;  // Update the last escape press time
        }
    }

    // Toggle wireframe mode with R key
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (!rKeyPressedLastFrame)
        {
            wireframeMode = !wireframeMode;
            glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
            rKeyPressedLastFrame = true;
        }
    }
    else
    {
        rKeyPressedLastFrame = false;
    }
}


void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (cursorLocked) // Check if the cursor is locked
    {
        if (firstMouse)
        {
            lastMouseX = xpos;
            lastMouseY = ypos;
            firstMouse = false;
        }

        double xoffset = xpos - lastMouseX;
        double yoffset = lastMouseY - ypos;
        lastMouseX = xpos;
        lastMouseY = ypos;

        xoffset *= cameraSensitivity;
        yoffset *= cameraSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraPos = cameraTarget + glm::normalize(direction) * distanceToCenter;
    }
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    float scrollSpeed = 5.0f; // Increase this value to make the scroll effect stronger
    distanceToCenter -= yoffset * scrollSpeed;
    if (distanceToCenter < 1.0f)
        distanceToCenter = 1.0f;
    if (distanceToCenter > 500.0f)
        distanceToCenter = 500.0f;
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraPos = cameraTarget + glm::normalize(direction) * distanceToCenter;
}
