#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "Texture.h"
#include "InputManager.h"
#include "Shader.h"

#include <GL/glut.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


constexpr int Window_Width = 1920;
constexpr int Window_Height = 1080;
const std::string Model_File = "Model/Lowpoly_Fox.obj";

struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoords;
    glm::vec3 Normal;
};

void initGLFW() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }
}

GLFWwindow* createWindow(int width, int height, const std::string& title) {
    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    return window;
}

void initGLEW() {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void setupGLFWCallbacks(GLFWwindow* window){
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glViewport(0, 0, Window_Width, Window_Height);
}

void loadOBJ(const std::string& inputfile, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = std::filesystem::path(inputfile).parent_path().string();

    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(inputfile, reader_config)) {
        std::cerr << "TinyObjReader: " << reader.Error();
        exit(EXIT_FAILURE);
    }

    const auto& attrib = reader.GetAttrib();
    const auto& shapes = reader.GetShapes();

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex;
            vertex.Position = { attrib.vertices[3 * index.vertex_index + 0], attrib.vertices[3 * index.vertex_index + 1], attrib.vertices[3 * index.vertex_index + 2] };
            vertex.TexCoords = (index.texcoord_index >= 0) ? glm::vec2(attrib.texcoords[2 * index.texcoord_index + 0], 1.0f - attrib.texcoords[2 * index.texcoord_index + 1]) : glm::vec2(0.0f, 0.0f);
            vertex.Normal = (index.normal_index >= 0) ? glm::vec3(attrib.normals[3 * index.normal_index + 0], attrib.normals[3 * index.normal_index + 1], attrib.normals[3 * index.normal_index + 2]) : glm::vec3(0.0f, 0.0f, 1.0f);
            vertices.push_back(vertex);
            indices.push_back(indices.size());
        }
    }
}

void setupBuffers(GLuint& VAO, GLuint& VBO, GLuint& EBO, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glBindVertexArray(0);
}

void loadTextures(const std::vector<tinyobj::material_t>& materials, const std::string& mtl_search_path, std::map<std::string, GLuint>& textureIDs) {
    for (const auto& material : materials) {
        if (!material.diffuse_texname.empty()) {
            std::filesystem::path texturePath = std::filesystem::path(mtl_search_path) / material.diffuse_texname;
            GLuint texID = LoadTexture(texturePath.string().c_str());
            if (texID > 0) {
                textureIDs[material.name] = texID;
            }
        }
    }
}

void drawScene(GLuint VAO, const Shader& shader, const std::vector<tinyobj::shape_t>& shapes, const std::vector<tinyobj::material_t>& materials, const std::map<std::string, GLuint>& textureIDs) {
    glBindVertexArray(VAO);
    for (const auto& shape : shapes) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            int material_id = shape.mesh.material_ids[f];
            if (material_id >= 0) {
                auto texIDIter = textureIDs.find(materials[material_id].name);
                if (texIDIter != textureIDs.end()) {
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, texIDIter->second);
                    shader.setInt("texture_diffuse", 0);
                }
            }
            glDrawElements(GL_TRIANGLES, 3 * shape.mesh.num_face_vertices[f], GL_UNSIGNED_INT, (void*)(index_offset * sizeof(unsigned int)));
            index_offset += 3 * shape.mesh.num_face_vertices[f];
        }
    }
    glBindVertexArray(0);
}

void renderBitmapString(float x, float y, void* font, const char* string) {
    glWindowPos2f(x, y);
    for (const char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void renderText() {
    glDisable(GL_DEPTH_TEST);  // Disable depth test to render text on top of everything
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, Window_Width, Window_Height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 1.0f);  // Set the text color to bright white
    renderBitmapString(10, 30, GLUT_BITMAP_HELVETICA_18, "R to toggle Wireframe");
    renderBitmapString(10, 50, GLUT_BITMAP_HELVETICA_18, "Scroll up and down to zoom in and out");

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);  // Re-enable depth test
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); // Initialize GLUT
    initGLFW();
    GLFWwindow* window = createWindow(2560, 1440, "OBJ Viewer");
    initGLEW();
    Shader shader("VertexShader.glsl", "FragmentShader.glsl");
    setupGLFWCallbacks(window);

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    loadOBJ(Model_File, vertices, indices);

    GLuint VAO, VBO, EBO;
    setupBuffers(VAO, VBO, EBO, vertices, indices);

    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = std::filesystem::path(Model_File).parent_path().string();
    reader.ParseFromFile(Model_File, reader_config);
    auto& materials = reader.GetMaterials();
    std::map<std::string, GLuint> textureIDs;
    loadTextures(materials, reader_config.mtl_search_path, textureIDs);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    shader.use();
    shader.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(fov), static_cast<float>(Window_Width) / Window_Height, 0.1f, 1000.0f);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        drawScene(VAO, shader, reader.GetShapes(), materials, textureIDs);

        renderText(); // Render instructions

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}