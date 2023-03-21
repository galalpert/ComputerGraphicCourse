# Assignment 3

## 1. We removed all irrelevant code from the Renderer.
## Renderer::InitOpenGLRendering() and It's way of action:
1. It creates two unique identifiers:
   - The first is for OpenGL texture stored in `gl_screen_tex`
   - The second one is for VAO, and stored in `gl_screen_vtc`.
2. `gl_screen_vtc` is set to be the current vertex array.
3. A unique buffer identifier is created and stored in a buffer.
4. The GLFloat's vtc and tex are copied to the GPU.
5. Is alloactes memory on the GPU using `glBufferData` and then copies vtc and tex to it using `glBufferSubData`.
6. It loads and compiles the sheders `vshader.glsl` and `fshader.glsl`.
7. Tells the shader where to look for the vertex position data, and the data dimensions.
8. It defines the two attributes `vPosition` and `vTexCoord`:
   - The first is for vertex position data.
   - The second is for texture position data with whom the shader will find them.
9.  Tells the shader `GL_TEXTURE0` as the texture id, that means that the GL_TEXTURE0 is used as the texture identifier for a uniform variable texture.

## 3. Vertex Shader
We first wrote a vertex shader with all the transformations.

```cpp
#version 330 core

layout(location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view *  model * vec4(pos, 1.0f);
}

```

##4. Fragment Shader
We wrote a fragment shader with a black color.

```cpp
#version 330 core

out vec4 frag_color;

void main()
{
    frag_color = vec4(0, 0, 0, 0);
}
```

## 5. Render Code:

```cpp
void Renderer::Render(const Scene& scene)
{
    Camera& camera = scene.GetActiveCamera();
    float half_width = viewport_width / 2.0f;
    float half_height = viewport_height / 2.0f;
    float DollyZoom;
    for(int i=0 ; i < scene.GetModelCount(); i++)
    {
        MeshModel& model = scene.GetModel(i);
        // Activate the 'colorShader' program (vertex and fragment shaders)
        colorShader.use();
        glm::mat4 model1 = model.get_mat_transformation();
        glm::mat4 modelview = camera.GetCameraTransformation();
        glm::mat4 modelpro = camera.GetProjectionTransformation();

        colorShader.setUniform("model", model.get_mat_transformation());
        colorShader.setUniform("view", camera.GetCameraTransformation());
        colorShader.setUniform("projection", camera.GetProjectionTransformation());
        colorShader.setUniform("material.textureMap", 0);

        texture.bind(0);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindVertexArray(model.GetVAO());
        glDrawArrays(GL_TRIANGLES, 0, model.GetModelVertices().size());
        glBindVertexArray(0);
        
        texture.unbind(0);
        colorShader.setUniform("color", model.getColor());

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(model.GetVAO());
        glDrawArrays(GL_TRIANGLES, 0, model.GetModelVertices().size());
        glBindVertexArray(0);
    }
}
```
### Images of the visible model:
![988d1235-e6ee-4ef1-8095-71892d29b7ac](https://user-images.githubusercontent.com/103389828/221681035-7effacab-d203-45f8-93f0-7742f23029e3.jpg)

![af3c8add-6d75-4d06-a961-a50c61a7766e](https://user-images.githubusercontent.com/103389828/221681046-756f0544-851a-401a-af20-f3f69d3b56b9.jpg)


## 6. Phong Shading
![phong1](https://user-images.githubusercontent.com/103389828/221692462-4a2f4684-1d31-4756-99f6-34ee3a8786fe.jpg)

![phong2](https://user-images.githubusercontent.com/103389828/221692507-608fb873-8f89-422c-b891-031e2d68db10.jpg)


## 7. Texture Mapping
### Plane:
![4eb13ff9-0e37-4d7a-b8d4-4091559ad5c4](https://user-images.githubusercontent.com/103389828/221694874-173b7c52-4373-44fd-b1b8-acf65f61b124.jpg)

### Cylinder
![cylindrical](https://user-images.githubusercontent.com/103389828/221692709-4e5ce286-d2c4-4161-b692-4a045b6dfe29.jpg)

### Sphere
![spherical](https://user-images.githubusercontent.com/103389828/221692681-31d31b76-740b-42e5-a879-b83dc77d50b2.jpg)

![spherical2](https://user-images.githubusercontent.com/103389828/221692697-af75e28e-baaa-49c9-9d6d-5f1be95daf63.jpg)


## 8. Texture Mapping
### Environment Mapping:

### Normal Mapping:
Without:
![WithoutNormalMap](https://user-images.githubusercontent.com/103389828/221694048-db4d7611-768f-4530-9e14-31b0ca4459f0.jpg)

With:
![WithNormalMap](https://user-images.githubusercontent.com/103389828/221694005-34c7cc6f-6a3a-4291-97cf-38046c13c101.jpg)


### Toon Shading
![toon](https://user-images.githubusercontent.com/103389828/221694503-688c8556-5715-48be-a5f5-35a46a9d02e9.jpg)

