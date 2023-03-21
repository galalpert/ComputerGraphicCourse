#version 330 core
#define PI 3.1415926538

struct Material
{
    sampler2D textureMap;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


uniform Material material;
uniform Light lights[4];
uniform int num_lights;
uniform vec3 cameraPos;
uniform int mapTex;
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

in vec3 fragPos;
in vec3 fragNormal;
in vec3 orig_fragPos;
in vec2 fragTexCoords;

// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{
    vec3 ia = vec3(0.0f);
    vec3 id = vec3(0.0f);
    vec3 is = vec3(0.0f);
    vec3 color = vec3(0.0f);
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(cameraPos - fragPos);
    float diff, spec ,levels, level;
    
    vec3 baseColor;
    vec3 lightColor; // Example light color
    vec3 shadowColor; // Example shadow color
    vec3 finalColor;
    vec4 texelColor;
    vec3 texelNormal;
    vec3 normal;

    // toon
    if (mapTex != 2 && mapTex != 3){ 
       
        vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));

        for (int i = 0; i < num_lights; i++) {
            vec3 l = normalize(lights[i].position - fragPos);
            vec3 r = reflect(-l, norm);

            diff = max(dot(norm, l), 0.0f);
            
            spec = pow(max(dot(viewDir, r), 0.0f), material.shininess);
            vec3 curr_diffuse = lights[i].diffuse * (diff * material.diffuse);
            vec3 curr_specular = lights[i].specular * (spec * material.specular);

            ia = ia + lights[i].ambient * material.ambient * textureColor;
            id = (id + curr_diffuse) * textureColor;
            is = (is + curr_specular) * textureColor;
            color = ia + id + is;
            
            switch (mapTex){
            case 0:
                frag_color = vec4(textureColor.rgb * color, 1.0f);
                break;
           
            case 1:  //toon
               
                levels = 4.0;
                level = floor(diff * levels) / levels;

                baseColor = textureColor.rgb;
                lightColor = vec3(1.0, 1.0, 1.0); // Example light color
                shadowColor = vec3(0.5, 0.5, 0.5); // Example shadow color
                finalColor = mix(shadowColor, mix(baseColor, lightColor, level), level);
                frag_color = vec4(finalColor * color, textureColor.x);
                break;
                
            }
        }
    }
    
    //Normal mapping
    else if(mapTex == 2){
        normal = texture(normalMap, fragTexCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0);
      
        color = texture(diffuseMap, fragTexCoords).rgb;
       
        ia = 0.1 * color;
        for (int i = 0; i < num_lights; i++) {
            vec3 lightDir = normalize(lights[i].position - fragPos);
            float diff = max(dot(lightDir, normal), 0.0);
            id += diff * color;
            vec3 reflectDir = reflect(-lightDir, normal);
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
            is += vec3(0.2) * spec;
        }
        frag_color = vec4( ia + id + is, 1.0);
    }
}
    


   
    




