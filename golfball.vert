varying vec4 diffuse, ambient;
varying vec3 lightDir, normal;
varying vec3 vViewVec;

void main() {	
	/* Berechnen aus welcher Richtung das Licht kommt */
	vec4 ecPos = gl_ModelViewMatrix * gl_Vertex;
	lightDir = vec3(gl_LightSource[1].position - ecPos);

	/* Die Entefernung für den Nebel übergeben */
	gl_FogFragCoord = -ecPos.z;

	/* Texture-Koordinaten durchreichen */	
	gl_TexCoord[0] = gl_MultiTexCoord0;

	/* Ambient- und Diffuse-Anteil berechnen */
	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[1].diffuse;
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	
	/* Richtung der Kamera berechnen */
	vViewVec = -ecPos.xyz;

	/* Die Normale berechnen */
 	normal = normalize(gl_NormalMatrix * vec3(gl_Normal));

	/* Den aktuerllen Vertex projezieren */
	gl_Position = ftransform();
} 
