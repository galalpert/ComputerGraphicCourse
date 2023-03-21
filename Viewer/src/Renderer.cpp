#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include "iostream"

#include "Renderer.h"
#include "InitShader.h"


#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))


Renderer::Renderer(int viewport_width, int viewport_height) :
    viewport_width(viewport_width),
    viewport_height(viewport_height)
{
    LoadShaders();
}

Renderer::~Renderer()
{}


void Renderer::Render(const Scene& scene)
{
    Camera& camera = scene.GetActiveCamera();
    float half_width = viewport_width / 2.0f;
    float half_height = viewport_height / 2.0f;
    float DollyZoom;
    for (int i = 0; i < scene.GetModelCount(); i++) {
        MeshModel& model = scene.GetModel(i);

        LoadTextures(model.GetModelName());

        // Activate the 'colorShader' program (vertex and fragment shaders)
        colorShader.use();


        glm::mat4 model1 = model.getTransformationMatrix();
        glm::mat4 modelview = camera.GetCameraTransformation();
        glm::mat4 modelpro = camera.GetProjectionTransformation();

        // Set the uniform variables
        colorShader.setUniform("model", model.getTransformationMatrix());
        colorShader.setUniform("view", camera.GetCameraTransformation());
        colorShader.setUniform("projection", camera.GetProjectionTransformation());
        colorShader.setUniform("material.textureMap", 0);

        colorShader.setInt("diffuseMap", 0);
        colorShader.setInt("normalMap", 1);

        //Set  material properties
        colorShader.setUniform("material.ambient", glm::vec3(model.material.ambient));
        colorShader.setUniform("material.diffuse", glm::vec3(model.material.diffuse));
        colorShader.setUniform("material.specular", glm::vec3(model.material.specular));
        colorShader.setUniform("material.shininess", model.material.shininess);

        colorShader.setUniform("cameraPos", camera.GetCameraPosition());

        colorShader.setUniform("canonicalProject", model.canonicalProjection);

        colorShader.setUniform("mapTex", model.mapTex);


        // Set up the light sources
        colorShader.setUniform("num_lights", scene.GetLightCount());
        for (int i = 0; i < scene.GetLightCount(); i++) {
            Light& light = scene.GetLightAtIndex(i);
            std::string lightName = "lights[" + std::to_string(i) + "].";
            colorShader.setUniform((lightName + "ambient").c_str(), glm::vec3(light.ambient));
            colorShader.setUniform((lightName + "diffuse").c_str(), glm::vec3(light.diffuse));
            colorShader.setUniform((lightName + "specular").c_str(), glm::vec3(light.specular));
            colorShader.setUniform((lightName + "position").c_str(), glm::vec3(light.GetPosition()));
        }


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 1);


        // Set 'texture1' as the active texture at slot #0
        texture.bind(0);

        // Drag our model's faces (triangles) in fill mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindVertexArray(model.GetVAO());
        glDrawArrays(GL_TRIANGLES, 0, model.GetModelVertices().size());
        glBindVertexArray(0);
        // Unset 'texture1' as the active texture at slot #0
        texture.unbind(0);

    }
}
void Renderer::LoadTextures(string name)
{
    string path = "C:/Users/galal/OneDrive/מסמכים/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/" + name + ".jpg";

    //texture.loadTexture("C:/Users/galal/OneDrive/מסמכים/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/bricks.jpeg", true);
    texture.loadTexture("C:/Users/galal/OneDrive/מסמכים/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/marble.jpeg", true);

}

void Renderer::LoadShaders()
{
    colorShader.loadShaders("vshader.glsl", "fshader.glsl");
}

int Renderer::GetViewportWidth() const
{
    return viewport_width;
}

int Renderer::GetViewportHeight() const
{
    return viewport_height;
}

void Renderer::SetViewportWidth(const int _viewport_width)
{
    this->viewport_width = _viewport_width;
}

void Renderer::SetViewportHeight(const int _viewport_height)
{
    this->viewport_height = _viewport_height;
}
