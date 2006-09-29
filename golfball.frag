uniform samplerCube Environment;

uniform float useFog;

uniform float reflection;

varying vec4 diffuse, ambient;
varying vec3 lightDir, normal;
varying vec3 vViewVec;

void main() {
	/* Entfernung vom nächsten Loch */
	vec3 offset = gl_TexCoord[0].xyz;

	/* Normale */
	vec3 n = normal;

	/* Wenn dieser Pixel in einem Loch liegt, wird die Normale verändert */
	if (length(offset) < 0.5) {
		n = n - 0.5 * gl_NormalMatrix * offset;
	}
	
	/* Und zum Schluss die Normale nochmal normieren. Dadurch wirkt der Ball auch runder. */
	n = normalize(n);

	/* diffuse Beleuchtung berechnen */
	float NdotL = max(dot(n, normalize(lightDir)), 0.0);
	vec4 colDiffuse = diffuse * NdotL;

	/* Richtung der Reflektion berechnen und entsprechenden Punkt in der CubeMap nachschauen. */
 	vec3 viewVec = normalize(vViewVec);
	vec3 reflVec = reflect(-viewVec, n);
	vec4 colReflection = textureCube(Environment, vec3(gl_TextureMatrix[0] * vec4(reflVec, 1)));

	/* Nebel berechnen */
	float fog = (1.0 - useFog * clamp((gl_FogFragCoord - gl_Fog.start) * gl_Fog.scale, 0.0, 1.0));

	/* Alles zusammensetzen */
	gl_FragColor = vec4(vec3((1.0 - reflection) * colDiffuse + reflection * colReflection), 1.0) * fog;
}
