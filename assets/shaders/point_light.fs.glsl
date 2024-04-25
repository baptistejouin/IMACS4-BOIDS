#version 330 core

//variables d'entrées
in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

out vec4 fragColor;

const int nb_lights = 3; // Nombre de points de lumière supportés

//uniforms
uniform sampler2D uText;
uniform vec3 uKd; // Couleur diffuse
uniform vec3 uKs; // Couleur spéculaire
uniform float uShininess; // Brillance
uniform vec3 uLightPos_vs[nb_lights]; // Augmentez la taille du tableau selon le nombre de points de lumière supportés
uniform vec3 uLightIntensity[nb_lights]; // Intensité de chaque lumière

vec3 blinnPhong(vec3 lightPos_vs, vec3 lightIntensity)
{
    // Calcul des vecteurs nécessaires
    float d = distance(vPosition_vs, lightPos_vs);
    vec3 Li = lightIntensity / d;
    vec3 wi = normalize(lightPos_vs - vPosition_vs);
    vec3 N = normalize(vNormal_vs);
    vec3 H = normalize(wi + vec3(0, 0, 1));

    // Vérification des produits scalaires
    float cosTheta = dot(N, wi);
    float cosAlpha = dot(H, N);
    
    // Calcul de la lumière diffuse et spéculaire
    vec3 diffuse = uKd * max(0.0, cosTheta);
    vec3 specular = uKs * pow(max(0.0, cosAlpha), uShininess);

    return Li * (diffuse + specular); // Addition des composantes diffuse et spéculaire
}

void main()
{
    float ambient = 0.20f;
    vec3 total_light_color = vec3(0.0); // Couleur totale de la lumière

    for (int i = 0; i < nb_lights; ++i) // Parcours de chaque point de lumière
    {
        total_light_color += blinnPhong(uLightPos_vs[i], uLightIntensity[i]);
    }

    vec4 color = texture(uText, vTexCoords);
    
    fragColor = vec4(ambient * uKd + total_light_color, 1.0) * color;
}
