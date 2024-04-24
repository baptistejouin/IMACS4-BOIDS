
#version 330 core

//variables d'entrées
in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

out vec4 fragColor;

//uniforms
uniform sampler2D uText;

void main()
{
    vec4 texColor = texture(uText, vTexCoords);

    // use normal for shading
    fragColor = texColor;
}