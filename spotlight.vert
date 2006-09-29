varying vec4 diffuse;
varying vec3 normal, lightDir;
	
void main()
{	
	/* Richtung des Spotlights berechnen */
	vec3 ecPos = vec3(gl_ModelViewMatrix * gl_Vertex);
	lightDir = vec3(gl_LightSource[2].position) - ecPos;

	/* diffuser Anteil */	
	diffuse = gl_LightSource[2].diffuse * gl_Color;
	
	/* Normale berechnen */
	normal = normalize(gl_NormalMatrix * gl_Normal);
	
	/* aktuellen Vertex transformieren */
	gl_Position = ftransform();
} 
