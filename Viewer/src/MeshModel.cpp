#include "MeshModel.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include <random>


MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords ,const std::string& model_name) :
    faces(faces),
    vertices(vertices),
    normals(normals)
{
    int i = model_name.find_last_of('.');
    this->model_name = model_name.substr(0, i);
    this->color = glm::vec3(0.f, 0.f, 0.f);

    glm::mat4 scaleMat = Normalize();

    material = {glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f};
    modelVertices.reserve(3 * faces.size());
    for (int i = 0; i < faces.size(); i++)
    {
        Face currentFace = faces.at(i);
        for (int j = 0; j < 3; j++){
            int vertexIndex = currentFace.GetVertexIndex(j) - 1;
            
            Vertex vertex;
            glm::vec4 v(vertices[vertexIndex], 1.f);
            v = scaleMat * v;
//            v = v/v.w;
            vertex.position = v;
            vertex.normal = normals[vertexIndex];

            if (textureCoords.size() > 0)
            {
                int textureCoordsIndex = currentFace.GetTextureIndex(j) - 1;
                vertex.textureCoords = textureCoords[textureCoordsIndex];
            }
            
            modelVertices.push_back(vertex);
        }
    }
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(Vertex), &modelVertices[0], GL_STATIC_DRAW);

    // Vertex Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Normals attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Vertex Texture Coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    // Vertex Tangent
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(9 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);

    // Vertex Tangent
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(12 * sizeof(GLfloat)));
    glEnableVertexAttribArray(4);
    
    
    // unbind to make sure other code does not change it somewhere else
    glBindVertexArray(0);
}


GLuint MeshModel::GetVAO() const
{
    return vao;
}

const std::vector<Vertex>& MeshModel::GetModelVertices()
{
    return modelVertices;
}


MeshModel::~MeshModel()
{
}

const Face& MeshModel::GetFace(int index) const
{
    return faces[index];
}

int MeshModel::GetFacesCount() const
{
    return faces.size();
}

const std::string& MeshModel::GetModelName() const
{
    return model_name;
}

const glm::vec3& MeshModel::GetVertices(int index) const
{
    return vertices[index];
}

const glm::vec3& MeshModel::GetNormals(int index) const
{
    return normals[index];
}


int MeshModel::GetVerticesSizes() const
{
    return vertices.size();
}


void MeshModel::setColor(const glm::vec4& color_)
{
    this->color = color_;
}

const glm::vec3& MeshModel::getColor()
{
    return color;
}

glm::mat4 MeshModel::Normalize()
{
    float minX, maxX,  minY, maxY, maxZ, minZ;
    minX = vertices[0].x;
    maxX = vertices[0].x;
    minY = vertices[0].y;
    maxY = vertices[0].y;
    minZ = vertices[0].z;
    maxZ = vertices[0].z;
    
    for(int i = 1; i < vertices.size(); i++)
    {
        minX = std::min(minX, vertices[i].x);
        minY = std::min(minY, vertices[i].y);
        minZ = std::min(minY, vertices[i].z);
        maxX = std::max(maxX, vertices[i].x);
        maxY = std::max(maxY, vertices[i].y);
        maxZ = std::max(maxY, vertices[i].z);
    }
    
    float size = std::max(maxX-minX, maxY-minY);
    size = std::max(size, maxZ-minZ);
    float scale_factor = 100.f/size;
    return glm::scale(glm::mat4(1.0f), glm::vec3(scale_factor, scale_factor, scale_factor));
}


void MeshModel::SetMaterial(const std::string name, const Material& material_)
{
    material_name = name;
    material.ambient = material_.ambient;
    material.diffuse = material_.diffuse;
    material.specular = material_.specular;
    material.shininess = material_.shininess;
}


