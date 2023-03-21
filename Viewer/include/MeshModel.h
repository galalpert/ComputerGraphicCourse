#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"
#include "MatrixTransform.h"
#include "Material.h"
#include <glad/glad.h>


struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoords;
};

class MeshModel : public MatrixTransform
{
public:
    MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, const std::string& model_name = "");
    virtual ~MeshModel();
    
    const Face& GetFace(int index) const;
    int GetFacesCount() const;
    const std::string& GetModelName() const;
    const glm::vec3& GetVertices(int index) const;
    int GetVerticesSizes() const;
    const glm::vec3& getColor();
    void setColor(const glm::vec4& color_);
    const glm::vec3& GetNormals(int index) const;
    void SetModelBoundaries();
    float GetModelBoundaries(int i) const;
    
    
    std::string GetMaterialName() { return material_name; }
    void SetMaterial(const std::string name, const Material& material);
    std::vector<glm::vec3>& GetLightColors() { return light_colors; }
    Material material;
    
    const std::vector<Vertex>& GetModelVertices();
    GLuint GetVAO() const;
    glm::mat4 Normalize();

    int canonicalProjection = 0;

    int mapTex = 0; // 0 = none, 1 = toon, 2 = Normal, 3 = Environment

    
private:
    std::vector<Face> faces;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> textureCoords;
    std::vector<Vertex> modelVertices;
    GLuint vbo;
    GLuint vao;
    std::string model_name;
    glm::vec3 color;


    std::string material_name;
    std::vector<glm::vec3> light_colors;
    float model_boundaries[6];

    //local
    glm::mat4 local;
    glm::vec3 translate;
    glm::vec3 rotate;
    float scale_local;

    //world
    glm::mat4 world;
    glm::vec3 translate_world;
    glm::vec3 rotate_world;
    float scale_world;
    
};
