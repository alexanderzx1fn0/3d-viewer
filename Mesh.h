#ifndef MESH_H_
#define MESH_H_

#include "Vertex.h"
#include <string.h>

struct Mesh
{
    GLuint vao, vbo, nbo, ebo;
    unsigned int index_count;
    Vertex* pVertices;
    unsigned int* pIndices;

    Mesh() {}

    Mesh(Vertex* vertex_data, unsigned int verticesCount,
                  unsigned int* index_data, unsigned int indicesCount)
    {
        pVertices = &vertex_data[0];
	//memcpy(pVertices, vertex_data, sizeof(vertex_data));
	//memcpy(pIndices, index_data, sizeof(index_data));
        pIndices = &index_data[0];

        index_count = indicesCount;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // for example triangle consist of 3 vertices therefore size of triangle equal to 3 * sizeof(Vertex)
        // note sizeof(Vertex) equal to

        /*
        typedef struct Vertex
        {
            glm::vec3    position; // 4 byte for each float --> 3 * 4 = 12
            glm::vec3    normal;   // 12 bytes
            glm::vec3    tangent;   // 12 bytes
            glm::vec3    bitangent; // 12 bytes
            glm::vec2    texcoord; // 8 bytes
        } Vertex;
        */
        // total size of Vertex equal to 12 + 12 + 12 + 12 + 8 = 56 bytes
        glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Vertex), vertex_data, GL_STATIC_DRAW);

        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(unsigned int), index_data, GL_STATIC_DRAW);

        float *ptr = 0;
        // stride -- offset between each vertex
        // last parameter offset inside vertex from begining to the end of structure
        // struct Vertex
        //   position begining from 0 to sizeof(vec3)
        //   normal beginining from sizeof(vec3) and so on
        // задаем порядок данных
        // Нужно сообщить в каком порядке идут вершинные атрибуты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(ptr+=0)); // position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(ptr+=3)); // normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(ptr+=3)); // tangent
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(ptr+=3)); // bitangent
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(ptr+=3)); // textcoor
        glEnableVertexAttribArray( 0 );
        glEnableVertexAttribArray( 1 );
        glEnableVertexAttribArray( 2 );
        glEnableVertexAttribArray( 3 );
        glEnableVertexAttribArray( 4 );

        glBindVertexArray(0);
    }

    Mesh(const float* vertex_data, int verticesCount,
					   const unsigned int* index_data, int indicesCount, const float* customNormals)
{
    index_count = indicesCount;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(float) * 3, vertex_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // position
    glEnableVertexAttribArray( 0 );

    glGenBuffers(1, &nbo);
    glBindBuffer(GL_ARRAY_BUFFER, nbo);

    glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(float) * 3, customNormals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); // normals
    glEnableVertexAttribArray( 1 );
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(unsigned int), index_data, GL_STATIC_DRAW);

    float *ptr = 0;
    glBindVertexArray(0);


}

    ~Mesh()
    {
        /* делаем активным массив вершин */
        glBindVertexArray(vao);
        /* отцепляем буфер */
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &ebo);
        /* делаем неактивным массив вершин */
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &vao);

    }

    void draw_mesh()
    {
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }
};


#endif // MESH_H_
