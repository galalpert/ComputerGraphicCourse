#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include "Renderer.h"
#include "InitShader.h"
#include "MeshModel.h"
#include "Utils.h"
#include <glm/gtx/transform.hpp>

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))


Renderer::Renderer(int viewport_width, int viewport_height) :
    viewport_width(viewport_width),
    viewport_height(viewport_height)
{
    InitOpenglRendering();
    CreateBuffers(viewport_width, viewport_height);
}

Renderer::~Renderer()
{
    delete[] color_buffer;
}

void Renderer::PutPixel(int i, int j, const glm::vec3& color)
{
    if (i < 0) return; if (i >= viewport_width) return;
    if (j < 0) return; if (j >= viewport_height) return;

    color_buffer[INDEX(viewport_width, i, j, 0)] = color.x;
    color_buffer[INDEX(viewport_width, i, j, 1)] = color.y;
    color_buffer[INDEX(viewport_width, i, j, 2)] = color.z;
}

void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
    // TODO: Implement bresenham algorithm
    // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

    float x1 = p1[0], x2 = p2[0];
    float y1 = p1[1], y2 = p2[1];

    if (x1 > x2)
    {
        float tmpX = x1;
        x1 = x2;
        x2 = tmpX;

        float tmpY = y1;
        y1 = y2;
        y2 = tmpY;
    }

    float x = x1, y = y1;
    float deltaX = x2 - x1, deltaY = y2 - y1;

    //vertical line case
    if (deltaX == 0)
    {
        if (y1 > y2)
        {
            float tmpY = y1;
            y1 = y2;
            y2 = tmpY;
            y = y1;
        }
        while (y <= y2)
        {
            PutPixel(x, y, color);
            y++;
        }
        return;
    }

    //claculate the slope
    float slope = (deltaY) / (deltaX);
    float e = -1 * deltaX, c = y1 - (slope * x1);

    //check slope cases
    //moderte positive slope
    if (slope > 0 && slope < 1)
    {
        while (x <= x2)
        {
   
            //increase y
            if (e > 0)
            {
                y++;
                e = e - 2 * deltaX;
            }
            PutPixel(x, y, color);
            //increase x
            x++;
            e = e + 2 * deltaY;
        }
    }
    //steep positive slope
    else if (slope >= 1)
    {
        e = -1 * deltaY;
        while (y <= y2)
        {
            //increase x
            if (e > 0)
            {
                x++;
                e = e - 2 * deltaY;
            }
            PutPixel(x, y, color);
            //increase y
            y++;
            e = e + 2 * deltaX;
        }
    }
    //moderate negative slope
    else if (slope < 0 && slope > -1)
    {
        e = deltaX;
        while (x <= x2)
        {
            //decrease y
            if (e < 0)
            {
                y--;
                e = e + 2 * deltaX;
            }
            //increase x
            PutPixel(x, y, color);
            x++;
            e = e + 2 * deltaY;
        }
    }
    //steep negative slope
    else if (slope <= -1)
    {
        e = deltaY;

        while (y >= y2)
        {
            //increase x
            if (e > 0)
            {
                x++;
                e = e + 2 * deltaY;
            }
            //decrease y
            PutPixel(x, y, color);
            y--;
            e = e + 2 * deltaX;
        }
    }
    //horizontal line case
    else if (slope == 0)
    {
        while (x <= x2)
        {
            PutPixel(x, y, color);
            x++;
        }
    }
}

void Renderer::CreateBuffers(int w, int h)
{
    CreateOpenglBuffer(); //Do not remove this line.
    color_buffer = new float[3 * w * h];
    ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::InitOpenglRendering()
{
    // Creates a unique identifier for an opengl texture.
    glGenTextures(1, &gl_screen_tex);

    // Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
    glGenVertexArrays(1, &gl_screen_vtc);

    GLuint buffer;

    // Makes this VAO the current one.
    glBindVertexArray(gl_screen_vtc);

    // Creates a unique identifier for a buffer.
    glGenBuffers(1, &buffer);

    // (-1, 1)____(1, 1)
    //	     |\  |
    //	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
    //	     |__\|
    // (-1,-1)    (1,-1)
    const GLfloat vtc[] = {
        -1, -1,
         1, -1,
        -1,  1,
        -1,  1,
         1, -1,
         1,  1
    };

    const GLfloat tex[] = {
        0,0,
        1,0,
        0,1,
        0,1,
        1,0,
        1,1 };

    // Makes this buffer the current one.
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // This is the opengl way for doing malloc on the gpu. 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vtc) + sizeof(tex), NULL, GL_STATIC_DRAW);

    // memcopy vtc to buffer[0,sizeof(vtc)-1]
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

    // memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

    // Loads and compiles a sheder.
    GLuint program = InitShader("vshader.glsl", "fshader.glsl");

    // Make this program the current one.
    glUseProgram(program);

    // Tells the shader where to look for the vertex position data, and the data dimensions.
    GLint  vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Same for texture coordinates data.
    GLint  vTexCoord = glGetAttribLocation(program, "vTexCoord");
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)sizeof(vtc));

    //glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

    // Tells the shader to use GL_TEXTURE0 as the texture id.
    glUniform1i(glGetUniformLocation(program, "texture"), 0);
}

void Renderer::CreateOpenglBuffer()
{
    // Makes GL_TEXTURE0 the current active texture unit
    glActiveTexture(GL_TEXTURE0);

    // Makes glScreenTex (which was allocated earlier) the current texture.
    glBindTexture(GL_TEXTURE_2D, gl_screen_tex);

    // malloc for a texture on the gpu.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewport_width, viewport_height, 0, GL_RGB, GL_FLOAT, NULL);
    glViewport(0, 0, viewport_width, viewport_height);
}

void Renderer::SwapBuffers()
{
    // Makes GL_TEXTURE0 the current active texture unit
    glActiveTexture(GL_TEXTURE0);

    // Makes glScreenTex (which was allocated earlier) the current texture.
    glBindTexture(GL_TEXTURE_2D, gl_screen_tex);

    // memcopy's colorBuffer into the gpu.
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport_width, viewport_height, GL_RGB, GL_FLOAT, color_buffer);

    // Tells opengl to use mipmapping
    glGenerateMipmap(GL_TEXTURE_2D);

    // Make glScreenVtc current VAO
    glBindVertexArray(gl_screen_vtc);

    // Finally renders the data.
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
    for (int i = 0; i < viewport_width; i++)
    {
        for (int j = 0; j < viewport_height; j++)
        {
            PutPixel(i, j, color);
        }
    }
}

void Renderer::drawCircle( const glm::vec2& center, float radius, const glm::vec3& color) 
{
    int step = 1000;
   
    for (int i = 0; i < step; i++)
    {
        const glm::ivec2 point(center[0] + radius * sin((2*M_PI * i) / step), center[1] + radius * cos((2 * M_PI * i) / step));
        Renderer::DrawLine(center, point, color);
    }

}


void Renderer::Render(const Scene& scene)
{
    // TODO: Replace this code with real scene rendering code
    int half_width = viewport_width / 2;
    int half_height = viewport_height / 2;
    
    //draw mesh
    if (scene.GetModelCount() != 0)
    {
        glm::vec3 color(1, 0, 0);

        for (int i = 0; i < scene.GetModelCount(); i++)
        {
            MeshModel& model = scene.GetModel(i);

            glm::mat4 local = model.local_transformation(model.ScaleModel(model.GetScale()), model.TranslateModel(model.GetLocalX(), model.GetLocalY(), model.GetLocalZ()), model.RotateXModel(model.GetLocalAngleX()), model.RotateYModel(model.GetLocalAngleY()), model.RotateZModel(model.GetLocalAngleZ()));

            glm::mat4 world = model.world_transformation(model.ScaleModel(model.GetScale()), model.TranslateModel(model.GetWorldX(), model.GetWorldY(), model.GetWorldZ()), model.RotateXModel(model.GetWorldAngleX()), model.RotateYModel(model.GetWorldAngleY()), model.RotateZModel(model.GetWorldAngleZ()));

            glm::mat4 finalTrans = model.final_transformation(world, local);


            //draw
            for (int i = 0; i < model.GetFacesCount(); i++)
            {
                glm::vec4 p1_(model.GetVertices(model.GetFace(i).GetVertexIndex(0) - 1), 1.0f);
                glm::vec4 p2_(model.GetVertices(model.GetFace(i).GetVertexIndex(1) - 1), 1.0f);
                glm::vec4 p3_(model.GetVertices(model.GetFace(i).GetVertexIndex(2) - 1), 1.0f);

                p1_ = finalTrans * p1_;
                p2_ = finalTrans * p2_;
                p3_ = finalTrans * p3_;

                glm::vec3 p1(p1_);
                glm::vec3 p2(p2_);
                glm::vec3 p3(p3_);

                DrawLine(p1, p2, color);
                DrawLine(p1, p3, color);
                DrawLine(p2, p3, color);
            }

        }

    }
       
}

int Renderer::GetViewportWidth() const
{
    return viewport_width;
}

int Renderer::GetViewportHeight() const
{
    return viewport_height;
}
