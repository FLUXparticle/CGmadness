varying vec4 diffuse;
varying vec3 normal, lightDir;

void main()
{
	/* Farbe erstmal auf Schwarz setzen */
	vec4 color = vec4(0);
	
	/* Richtung des Spotlights bestimmen */
	vec3 light = normalize(lightDir);

	/* diffuse Beleuchtung berechnen */
	float NdotL = max(dot(normalize(normal), light), 0.0);
	/* Winkel von der Hauptachse des Spotlights an dieser Stelle */
	float spotEffect = dot(normalize(gl_LightSource[2].spotDirection), -light);

	/* Cutoff des Spotlights */
	if (spotEffect > gl_LightSource[2].spotCosCutoff) {
		spotEffect = pow(spotEffect, gl_LightSource[2].spotExponent);
		color = spotEffect * diffuse * NdotL;

		/* Das grün in der Mitte rausfiltern, damit das gelbe Spotlight in der mitte rot wird. */
		color.g *= min((1.0 - spotEffect) * 2.0, 1.0);
	}

	/* Farbe zurückgeben */	
	gl_FragColor = color;
}
