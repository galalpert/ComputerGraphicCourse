#version 330 core
#define PI 3.1415926538
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

// The model/view/projection matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int canonicalProject;


out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoords;
out vec3 orig_fragPos;

void main()
{
    fragPos = vec3(model * vec4(pos, 1.0f));
    fragNormal = mat3(view * model) * normal;
    orig_fragPos = vec3(vec4(pos, 1.0f));
   
    switch (canonicalProject){
          case 0:           // plane
            fragTexCoords = texCoords;
            break;
          case 1:            // cylinder
            fragTexCoords = vec2(atan(orig_fragPos.z, orig_fragPos.x), orig_fragPos.y);
            break;
          case 2:               // sphere
           fragTexCoords.x = 0.5 + atan(orig_fragPos.z, orig_fragPos.x);
           fragTexCoords.y = 0.5 - atan(-orig_fragPos.y, length(orig_fragPos.xz));
            break;
    }

    gl_Position = projection * view *  model  * vec4(pos, 1.0f);
    
}







