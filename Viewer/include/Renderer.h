#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Material.h"
#include "Texture2D.h"
#include "ShaderProgram.h"

class Renderer
{
public:
    
    void LoadShaders();
    Renderer(int viewportWidth, int viewportHeight);
    Renderer();

    virtual ~Renderer();
    void Render(const Scene& scene);
    int GetViewportWidth() const;
    int GetViewportHeight() const;
    void SetViewportWidth(const int _viewport_width);
    void SetViewportHeight(const int _viewport_height);
    void LoadTextures(string name);
 
//private:

    ShaderProgram lightShader;
    ShaderProgram colorShader;
    Texture2D texture;
    Texture2D diffuseMap;
    Texture2D normalMap;
    
    int viewport_width;
    int viewport_height;
    GLuint gl_screen_tex;
    GLuint gl_screen_vtc;

};
