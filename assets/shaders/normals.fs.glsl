#version 330 core

in vec3 vPosition_vs; // Position du sommet transformée dans l'espace View (vs)
in vec3 vNormal_vs; // Normale du sommet transformée dans l'espace View (vs)
in vec2 vTexCoords; // Coordonnées de texture du sommet

out vec4 fragColor; // Couleur du fragment

void main()
{
	// On normalise la normale pour l'afficher comme une couleur
	vec3 color = normalize(vNormal_vs);
	fragColor = vec4(color, 1.0);
}
