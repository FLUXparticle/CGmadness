#include "texture.h"

#include <cgimage.h>

#include <GL/gl.h>
#include <GL/glu.h>

static int gUseTextures = 0;

/*
 * Richtet das TextureEnvironment ein
 */
void initTextures(void) {
  /*
	 * Farbinformation der Textur mit der Farbinformation aus der
   * Lichtberechnung mischen
	 */
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gUseTextures = 1;
}

/*
 * Bestimmt das Format der übergebenen Pixeldaten
 */
unsigned int calculateGLBitmapMode (CGImage* image) {
  switch (image->bpp) {
    case 1:
      return GL_LUMINANCE;
    case 2:
      return GL_LUMINANCE_ALPHA;
    case 3:
      return GL_RGB;
    case 4:
      return GL_RGBA;
    default:
      return GL_RGB;
	}
}

/*
 * Lädt eine Texture aus einer Datei und berechnet ggf. auch die Mipmaps dafür.
 */
int loadTexture(char* filename, int mipmapping) {
	GLuint id;
	CGImage *image = CGImage_load(filename);

	if (image == NULL) {
		return -1;
	}

	if (!gUseTextures) {
		initTextures();
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	if (mipmapping) {
		gluBuild2DMipmaps(GL_TEXTURE_2D,
				   image->bpp,
				   image->width,
				   image->height,
				   calculateGLBitmapMode(image),
				   GL_UNSIGNED_BYTE,
				   image->data
				);
	} else {
		glTexImage2D(GL_TEXTURE_2D,      /* 2D-Textur */
								 0,          /* level-of-detail fuer mipmapping */
								 image->bpp, /* internes Format / bbp */
								 image->width,       /* Breite */
								 image->height,      /* Hoehe */
								 0,          /* Border */
								 calculateGLBitmapMode(image),      /* Art der Pixelwerte im Buffer */
								 GL_UNSIGNED_BYTE,   /* Datentyp der Pixelwerte im Buffer */
								 image->data);
	}

	
	/* Randbehandlung: Wiederholung */
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/* Linerare Interpolation bei Skalierung der Textur */
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (mipmapping) {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	} else {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	/* Speicher freigeben */
	CGImage_free(image);
	
	return id;
}
