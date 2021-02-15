#ifndef GAME_H_
#define GAME_H_

#include <glad/glad.h>
//#include "openglextensions.h"
#include "context.h"
#include "camera.h"
#include "ui.h"
#include "obj_loader.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

// GEOMETRY
//#include "data/gun.h"
#include "data/bloom_cubes.h"
//#include "data/barrel.h"
//#include "data/cube.h"
//#include "data/sci-fi_crate.h"
#define window_width 1280
#define window_height 720

#define BUFFER_SIZE 2
struct BloomRenderer
{

    BloomRenderer()
    : width(window_width), height(window_height), fbo(0), depth_buffer(0),
      firstPass(NULL), finalPass(NULL), vao(0), vbo(0)
    {

    }
    ~BloomRenderer() {}
    int width;
    int height;

    GLuint fbo, temp_fbo;
    GLuint color_buffer[BUFFER_SIZE];
    GLuint depth_buffer;

    Shader* firstPass;
    Shader* secondPass;
    Shader* finalPass;

    GLuint vao, vbo; // for quad
};

class Crate
{
public:
    Crate()
    {
    load_geometry();
    create_bloom_renderer();
    }
    ~Crate()
    {
        delete box;
        delete lamps;
        delete R->firstPass;
        delete R->secondPass;
        delete R->finalPass;
        delete R;
    }

    void load_geometry()
    {
        box  = new Mesh(basicPositions, basicVertices, basicIndices, basicIndicesCount, basicNormals);
        lamps  = new Mesh(bloomPositions, bloomVertices, bloomIndices, bloomIndicesCount, bloomNormals);
    }

    void create_bloom_renderer()
    {
        R = new BloomRenderer;
        // Create framebuffer
        glGenFramebuffers(1, &R->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, R->fbo);
        
        // Сгенерировать два аттач
        glGenTextures(BUFFER_SIZE, &R->color_buffer[0]);

        // pos
        glBindTexture(GL_TEXTURE_2D, R->color_buffer[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window_width, window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // Связать текстуру с объектом буфера кадра
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, R->color_buffer[0], 0);
        
        // Создаем буфер глубины
        glGenRenderbuffers(1, &R->depth_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, R->depth_buffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window_width, window_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, R->depth_buffer);

        // передаем номер текстуры в gl_FragData[] или
        // layout(location = 0) out vec4 OutColor;
        // которую нужно записать
        GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, DrawBuffers);

        // Провера полностью ли заполнен буфер кадра
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("Error with framebuffer\n");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        // TEMP FBO
        glGenFramebuffers(1, &R->temp_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, R->temp_fbo);

        glGenTextures(BUFFER_SIZE, &R->color_buffer[1]);
        glBindTexture(GL_TEXTURE_2D, R->color_buffer[1]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window_width, window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // Связать текстуру с объектом буфера кадра
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, R->color_buffer[1], 0);
        glDrawBuffers(1, DrawBuffers);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Квадрат для буфера кадра
    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &R->vao);
    glGenBuffers(1, &R->vbo);
    glBindVertexArray(R->vao);
    glBindBuffer(GL_ARRAY_BUFFER, R->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

        R->firstPass = new Shader("data/shaders/edge/first_pass_vertex.glsl",
                            "data/shaders/edge/first_pass_fragment.glsl");
        R->secondPass = new Shader("data/shaders/edge/second_pass_vertex.glsl",
                            "data/shaders/edge/second_pass_fragment.glsl");
        R->finalPass = new Shader("data/shaders/edge/final_pass_vertex.glsl",
                            "data/shaders/edge/final_pass_fragment.glsl");


        // BIND FINAL SHADER AND PASS SOME DATA
        R->firstPass->bind();
        glUniform3fv(glGetUniformLocation(R->firstPass->id, "Light.L"), 1, glm::value_ptr(glm::vec3(1.0f)));
        glUniform3fv(glGetUniformLocation(R->firstPass->id, "Light.La"), 1, glm::value_ptr(glm::vec3(0.2f)));
        glm::vec4 lightPos = glm::vec4(0.0f, 4.0f, 2.5f, 1.0f);
        glUniform4fv(glGetUniformLocation(R->firstPass->id, "Light.Position"), 1, glm::value_ptr(lightPos));
        glUniform3f(glGetUniformLocation(R->firstPass->id, "Material.Kd"), 0.9f, 0.9f, 0.9f);
        glUniform3f(glGetUniformLocation(R->firstPass->id, "Material.Ks"), 0.95f, 0.95f, 0.95f);
        glUniform3f(glGetUniformLocation(R->firstPass->id,"Material.Ka"), 0.1f, 0.1f, 0.1f);
        glUniform1f(glGetUniformLocation(R->firstPass->id, "Material.Shininess"), 100.0f);


        R->secondPass->bind();
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(R->secondPass->id, "renderTex"), 0); // reference to textrue slot #0
        glUniform2f(glGetUniformLocation(R->secondPass->id, "u_viewPort"), window_width, window_height); // reference to textrue slot #0
        R->finalPass->bind();
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(R->finalPass->id, "renderTex"), 0); // reference to textrue slot #0
        glUniform2f(glGetUniformLocation(R->finalPass->id, "u_viewPort"), window_width, window_height); // reference to textrue slot #0

        float weights[5], sum, sigma2 = 8.0f;

        // Compute and sum the weights
        weights[0] = gauss(0,sigma2);
        sum = weights[0];
        for( int i = 1; i < 5; i++ ) {
            weights[i] = gauss(float(i), sigma2);
            sum += 2 * weights[i];
        }

        // Normalize the weights and set the uniform
        for( int i = 0; i < 5; i++ ) {
            float val = weights[i] / sum;
            R->secondPass->bind();
            glUniform1f(glGetUniformLocation(R->secondPass->id, "Weight") + 0, val);
            glUniform1f(glGetUniformLocation(R->secondPass->id, "Weight") + 1, val);
            glUniform1f(glGetUniformLocation(R->secondPass->id, "Weight") + 2, val);
            glUniform1f(glGetUniformLocation(R->secondPass->id, "Weight") + 3, val);
            glUniform1f(glGetUniformLocation(R->secondPass->id, "Weight") + 4, val);
            R->finalPass->bind();
            glUniform1f(glGetUniformLocation(R->finalPass->id, "Weight") + 0, val);
            glUniform1f(glGetUniformLocation(R->finalPass->id, "Weight") + 1, val);
            glUniform1f(glGetUniformLocation(R->finalPass->id, "Weight") + 2, val);
            glUniform1f(glGetUniformLocation(R->finalPass->id, "Weight") + 3, val);
            glUniform1f(glGetUniformLocation(R->finalPass->id, "Weight") + 4, val);
        }
        


    }

    float gauss(float x, float sigma2 )
    {
        double coeff = 1.0 / (glm::two_pi<double>() * sigma2);
        double expon = -(x*x) / (2.0 * sigma2);
        return (float) (coeff*exp(expon));
    }

    void render(Camera* camera)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, R->fbo);
        glViewport(0, 0, window_width, window_height);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        
        R->firstPass->bind();
        glm::mat4 modelView = camera->mView;
        //gl_NormalMatrix = glm::inverseTranspose(glm::mat3(gl_ModelViewMatrix));
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(camera->mView)));
        glUniformMatrix4fv(glGetUniformLocation(R->firstPass->id, "viewProj"), 1, GL_FALSE, glm::value_ptr(camera->mViewProj));
        glUniformMatrix4fv(glGetUniformLocation(R->firstPass->id, "modelView"), 1, GL_FALSE, glm::value_ptr(modelView));
        glUniformMatrix3fv(glGetUniformLocation(R->firstPass->id, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat3( glm::vec3(camera->mView[0]), glm::vec3(camera->mView[1]), glm::vec3(camera->mView[2]))));
        box->draw_mesh();
        lamps->draw_mesh();


        glBindFramebuffer(GL_FRAMEBUFFER, R->temp_fbo);
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT);

        R->secondPass->bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, R->color_buffer[0]); // в эту текстуру рисовали на первом проходе

        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(R->vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);



        glBindFramebuffer(GL_FRAMEBUFFER, 0); // this default fbo
        glClear(GL_COLOR_BUFFER_BIT);

        R->finalPass->bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, R->color_buffer[1]); 

        glBindVertexArray(R->vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);


    }

    void update(){}

    // MEMBERS
    BloomRenderer* R;
    Mesh* box;
    Mesh* lamps;
};
  

namespace Game
{
    Camera* camera;
    Crate* crate;
    void init() // loadGame
    {
        camera = new Camera();
        camera->freeCam = true;

        crate = new Crate();
    }

    void update()
    {
       // Physic();
    }

        void renderView(float aspect)
        {

            //camera->pos = ((Player*)curPlayer)->getHeadPos();
            //camera->rot = ((Player*)curPlayer)->rot;
            camera->aspect = aspect;

        camera->update();

        crate->render(camera);
//      RenderBunker(camera);
        //RenderObjects(camera);
        }

        void render()
        {
            float aspect = (float)Context::width / (float)Context::height;
            glClearColor(0.14f, 0.14f, 0.14f, 1.0); // #2e2e2e


            glViewport(0, 0, Context::width, Context::height);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            renderView(aspect);
        }

    void deinit()
    {
        delete crate;
        delete camera;
    }
};

#endif // GAME_H_
