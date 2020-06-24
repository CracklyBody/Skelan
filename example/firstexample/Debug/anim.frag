#version 330 

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

in vec2 textureCoords;

uniform Material material;

out vec4 color;

void main()
{
    vec4 result = vec4(0.0f);

    result = texture(material.texture_diffuse1, textureCoords);

    if (result.a < 0.1)
    {
        discard;    
    }

    color = result;
}
