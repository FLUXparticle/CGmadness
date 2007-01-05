#include "callback.h"

#include "light.h"
#include "text.h"
#include "features.h"

#include "debug.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include <stdio.h>
#include <math.h>

Viewport sgWindowViewport;

static RenderTarget gTargetWindow;

/*** Scene ***/

static funcUpdate gUpdate = NULL;

static int gSceneDirty = 1;

void setUpdateFunc(funcUpdate update) {
	gUpdate = update;
}

/*** Display ***/

static float gFPS = 0.0f;

static Matrix gProjectionText;

static List gRenderTargets = NULL;

void addRenderTarget(RenderTarget* target) {
	if (hasFramebuffer()) {
		target->enabled = 1;
		gRenderTargets = prependElement(gRenderTargets, target);
	}
}

void framerate(void) {
  /* Zeitpunkt der letzten Neuberechnung */
  static int timebase = 0;
  /* Anzahl der Aufrufe seit letzter Neuberechnung */
  static int frameCount = 0;
  /* aktuelle "Zeit" */
  static int time = 0;

  /* Diesen Aufruf hinzuzaehlen */
  frameCount++;

  /* seit dem Start von GLUT vergangene Zeit ermitteln */
  time = glutGet(GLUT_ELAPSED_TIME);

  /* Eine Sekunde ist vergangen */
  if (time - timebase > 1000) {
		/* FPS-Wert neu berechnen */
		gFPS = frameCount * 1000.0 / (time - timebase);

		/* Zureuecksetzen */
		timebase = time;
		frameCount = 0;
	}
}

/*
 * Schaltet den Framebuffer um und löscht dessen Inhalt
 */
void switchRenderTarget(RenderTarget* target) {
	if (hasFramebuffer()) {
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, target->framebuffer);
		if (target->framebuffer != 0) {
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, target->texTarget, target->texID, 0);	
			glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
		} else {
			glReadBuffer(GL_BACK);
		}
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, target->width, target->height);
}

/*
 * Callback-Funktion: Zeichnet alle registrierten Framebuffer neu
 */
void display(void) {
	List run;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (run = gRenderTargets; run; run = run->next) {
		RenderTarget* target = run->info;
		Viewport* v = target->viewport;
		if (target->enabled && v) {
			float aspect = (float) target->height / target->width;

			switchRenderTarget(target);
			
			/* Perspektiven-Modus */
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(&v->projection[0][0]);
			glScalef(aspect, 1.0f, 1.0f);
			
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(&v->view[0][0]);

			glEnable(GL_LIGHTING);
			setLights();

			drawObject(v->world);
		}
	}

	if (hasFramebuffer()) {
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
	glViewport(0, 0, gTargetWindow.width, gTargetWindow.height);

	/* Orthogonal-Modus für die Framecounter ausgabe */
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&gProjectionText[0][0]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	{
		char text[20];
		sprintf(text, "FPS: %4.1f", gFPS);
		glDisable(GL_LIGHTING);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glRasterPos2f(0.0f, 0.0f);
		drawBitmapText(text);
	}

	/* Bild anzeigen */
	glutSwapBuffers();
	framerate();

	gSceneDirty = 0;
}

void reshape(int w, int h) {
	gTargetWindow.width = w;
	gTargetWindow.height = h;
}

/*
 * Registriert die Display-Methode und inizialisiert den Haupt-Viewport
 */
void startDisplay(void) {
	/** Allgemein **/

	/* Pojektions-Matrix für Text setzen */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	glGetFloatv(GL_PROJECTION_MATRIX, &gProjectionText[0][0]);

	{
		/*
		 * Viewmatrix auf Einheits-Matrix setzen
		 */
		int x;
		int y;
		Viewport* v = &sgWindowViewport;

		for (x = 0; x < 4; x++) {
			for (y = 0; y < 4; y++) {
				v->view[x][y] = (x == y);
			}
		}

		initProjectMat(v->projection, FOV);
	}

	/* RenderTarget für Fenster erstellen */
	gTargetWindow.enabled = 1;
	gTargetWindow.framebuffer = 0;
	gTargetWindow.viewport = &sgWindowViewport;

	gRenderTargets = prependElement(gRenderTargets, &gTargetWindow);

	/* Reshape-Callback (wird ausgefuehrt, wenn neu gezeichnet wird
	 * z.B. nach Erzeugen oder Groessenaenderungen des Fensters) */
	glutReshapeFunc(reshape);

	/* Display-Callback (wird an mehreren Stellen imlizit
	 * oder explizit durch glutPostRedisplay angestossen) */
	glutDisplayFunc(display);
	glutIdleFunc(display);
}

/*** Update ***/

int gMillis;

/*
 * Ruft regelmäßig die registrierte Update-Funktion auf, um das Spielgeschehen fortlaufen zu lassen.
 * Dabei wird darauf geachtet, dass die Update-Funktion mit einem möglichst Konstantem intervalwert aufgerufen wird,
 * da die Physik-Funktionen im Spiel darauf angewiesen sind, auch wenn der timer-call mal etwas länger auf sich warten läßt.
 */
void timer(int lastCallTime) {
	/* Zeit zwischen Animationen berechnen */
  int thisCallTime = glutGet(GLUT_ELAPSED_TIME);
	int lastUpdateTime = lastCallTime;
	int nextUpdateTime = lastUpdateTime + gMillis;
	int diff;

	/* Neuen Timer anstossen */
	
	while (nextUpdateTime < thisCallTime) {
		double interval = (double) (nextUpdateTime - lastUpdateTime) / 1000.0f;
		/* Szene aktualisieren */
		gUpdate(interval);
		lastUpdateTime = nextUpdateTime;
		nextUpdateTime += gMillis;
	}

	diff = nextUpdateTime - glutGet(GLUT_ELAPSED_TIME);
	if (diff < 0) {
		diff = 0;
	}
	glutTimerFunc(diff, timer, lastUpdateTime);

	if (!gSceneDirty) {
		gSceneDirty = 1;
	
	  /* Neuzeichnen anstossen */
		glutPostRedisplay();
	}
}

/*
 * Registriert die Timer-Funktion
 */
void startTimer(int callsPerSecond) {
	/* Aufrufhaeufigkeit in Millisekunden */
  gMillis = 1000 / callsPerSecond;

  /* Callback Funktion nach berechneter Zeit aufrufen */
	glutTimerFunc(gMillis, timer, glutGet(GLUT_ELAPSED_TIME));
}

/*
 * Führt ein Picking an den übergebenen Koordinaten durch.
 */
int pick(int x, int y) {
  int viewport[4];
	float aspect;

	int width  = gTargetWindow.width;
	int height = gTargetWindow.height;

	Viewport* v = gTargetWindow.viewport;

	viewport[0] = 0;
	viewport[1] = 0;
	viewport[2] = width;
	viewport[3] = height;
	aspect = (float) height / width;

	/* Es soll für Picking gerendert werden */
	glRenderMode(GL_SELECT);

	/* Name-Stack initialisieren */
	glInitNames();

	/* Projektions-Matrix für Picking vorbereiten */ 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPickMatrix(x,        /* X-Position der Maus */
                height - y,  /* Y-Position der Maus */
                5,         /* Breite der Picking-Region */
                5,         /* Hoehe der Picking-Region */
                viewport);

	glMultMatrixf(&v->projection[0][0]);
	glScalef(aspect, 1.0f, 1.0f);

	/* Objekte für Picking zeichnen */
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&v->view[0][0]);
	pickObject(gTargetWindow.viewport->world);
	glFlush();

	return glRenderMode(GL_RENDER);
}
