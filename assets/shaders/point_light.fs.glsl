#version 330 core

//variables d'entrées
in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

out vec4 fragColor;

const int MAX_LIGHTS = 5;

//uniforms
uniform sampler2D uText;
uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec3 uLightPos_vs[MAX_LIGHTS];
uniform vec3 uLightIntensity[MAX_LIGHTS];

vec3 blinnPhong(vec3 lightPos_vs, vec3 lightIntensity)
{
    float d = distance(vPosition_vs, lightPos_vs);
    vec3 wi = normalize(lightPos_vs - vPosition_vs);
    vec3 Li = lightIntensity / (d * d);
    vec3 N = normalize(vNormal_vs);
    vec3 H = (N + wi) / 2.0; // vec3 H = normalize(wi + vec3(0, 0, 1));

    return Li * (uKd * max(0.0, dot(N, wi)) + uKs * pow(max(0.0, dot(N, H)), uShininess));
}

void main()
{
    vec3 lighting = vec3(0.0);
    for (int i = 0; i < MAX_LIGHTS; ++i) {
        if (i >= MAX_LIGHTS) break;
        lighting += blinnPhong(uLightPos_vs[i], uLightIntensity[i]);
    }

    vec4 color = texture(uText, vTexCoords);
    fragColor = vec4(color.rgb * lighting, color.a);
}
