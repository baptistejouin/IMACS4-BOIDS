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
    vec3 color = vec3(1.0, 1.0, 1.0);

    fragColor = vec4(color, 1.0);
}