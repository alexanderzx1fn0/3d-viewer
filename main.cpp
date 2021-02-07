#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

#include "shader.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if(!gladLoadGL()) { exit(-1); }
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version: %s\n", glGetString(GL_VERSION));
    printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glClearColor(0.5f,0.5f,0.5f,1.0f);


    // DEFINE GEOMETRY
    static const glm::vec3 kCubeVertices[] =
    {
	glm::vec3( 1.0f, 1.0f, -1.0f ),
	glm::vec3( -1.0f, 1.0f, -1.0f ),
	glm::vec3( -1.0f, -1.0f, -1.0f ),
	glm::vec3( 1.0f, -1.0f, -1.0f ),
	glm::vec3( 1.0f, 1.0f, 1.0f ),
	glm::vec3( -1.0f, 1.0f, 1.0f ),
	glm::vec3( -1.0f, -1.0f, 1.0f ),
	glm::vec3( 1.0f, -1.0f, 1.0f ),
    };
    
    static const unsigned int kCubeIndices[] = {
	0, 2, 1, 0, 3, 2,
	4, 3, 0, 4, 7, 3,
	4, 1, 5, 4, 0, 1,
	1, 6, 5, 1, 2, 6,
	3, 6, 2, 3, 7, 6,
	5, 7, 4, 5, 6, 7,
    };

    float vertices[] = {-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f};
    unsigned int indices[] = {0, 1, 2};

    unsigned int nVertices = sizeof(kCubeVertices) / sizeof(kCubeVertices[0]);
    unsigned int nIndices = sizeof(kCubeIndices) / sizeof(kCubeIndices[0]);
    printf("nVertices: %d\n", nVertices);
    printf("nIndices: %d\n", nIndices);
    // LOAD SHADERS
    Shader *program =  new Shader("data/shaders/basic_vertex.glsl", "data/shaders/basic_fragment.glsl");
    // SEND GEOMETRY DATA ON GPU
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, nVertices * sizeof(float) * 3, kCubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // position
    glEnableVertexAttribArray( 0 );

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(unsigned int), kCubeIndices, GL_STATIC_DRAW);

    float *ptr = 0;
    glBindVertexArray(0);

    // Some transformations
    float aspect = (float)WINDOW_WIDTH / (float)WINDOW_WIDTH; 
    glm::mat4 mProj = glm::perspective(glm::radians(90.0f), aspect, 0.01f, 100.0f);
    glm::mat4 mView = glm::mat4(1.0f);
    mView = glm::translate(mView, glm::vec3(0.0, 0.0, -5.0));


    /* Loop until the user closes the window */
    while( ! glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE) )
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

	// UPDATE

	// RENDER
	program->bind();
        glUniformMatrix4fv(glGetUniformLocation(program->id, "proj"), 1, GL_FALSE, glm::value_ptr(mProj));
        glUniformMatrix4fv(glGetUniformLocation(program->id, "modelView"), 1, GL_FALSE, glm::value_ptr(mView));
        glBindVertexArray(vao);

        glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    delete program;
    glfwTerminate();
    return 0;
}
