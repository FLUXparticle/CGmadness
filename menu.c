#include "menu.h"

#include "text.h"
#include "graph.h"
#include "pick.h"
#include "objects.h"
#include "texture.h"
#include "ball.h"
#include "game.h"
#include "features.h"
#include "keyboard.h"

#include <GL/glut.h>

#include <string.h>

#define SCALE_FONT 0.5f

/*** allgemeiner Teil des Menüs ***/

typedef void (*funcClick)(void);

typedef void (*funcChange)(void* self);

/*
 * Ein Objekt, dass angeklickt werden kann und dadurch ein Funktion auslößt.
 */
typedef struct {
	Object oButton;
	Pick pButton;
	funcClick click;
} Button;

/*
 * Ein Objekt, dass bei anklicken zwischen zwei Zuständen hin- und herschaltet und
 * eine Funktion aufruft um den aktuellen Zustand mitzuteilen.
 */
typedef struct {
	int value;
	Object oCheck;
	Pick pCheck;
	funcChange change;
} Check;

/*
 * Ein Element mit einem Pfeil nach Links und einem nach Rechts, um eine Auswahl zu tereffen.
 */
typedef struct {
	int value;
	int minValue;
	int maxValue;
	Object oSpinEdit;
	Object oLeft;
	Object oRight;
	Pick pLeft;
	Pick pRight;
	funcChange change;
} SpinEdit;

static Object goLeft;
static Object goRight;

/*
 * Erzeugt ein Quadrat mit einer Texture, das als Element im Menü angezeigt werden kann.
 */
void initMenuObject(Object* obj, int texture) {
	initObject(obj, drawSquare);
	obj->texture = texture;
	obj->ambient = 1.0f;
	obj->diffuse = 0.0f;
}

/*
 * Erzeugt ein Objekt mit 3D-Text, das zentriert wird und auf eine bestimmte Höhe gesetzt wird.
 */
void initTextObject(Object* obj, char* text, float z) {
	float length = makeTextObject(obj, text) * SCALE_FONT;
	setObjectPosition3f(obj, -length / 2, 0.0f, z);
	setObjectScalef(obj, SCALE_FONT);
	rotateObjectX(obj, 90.0f);
}

/*** Button ***/

/*
 * Callback-Funktion: Ruft die Klick-Funktion eines Buttons auf, wenn das zugehörige Scene-Objekt gepickt wird.
 */
void pickButton(void* data) {
	Button* button = data;
	button->click();
}

/*
 * Erzeugt einen Botton aus 3D-Text.
 */
void init3dButton(Button* button, float z, funcClick click, char* text) {
	initTextObject(&button->oButton, text, z);
 	
	button->click = click;

	if (click) {
		initPick(&button->pButton, pickButton, button);
		setObjectPick(&button->oButton, &button->pButton);
	}	
}

/*** Check ***/

/*
 * Passt die Farbe des Callback-Objektes dem aktuellen Wert an und
 * ruft die change-Funktion auf um den aktuellen Wert mitzuteilen.
 */
void setCheck(Check* check, int value) {
	check->value = value;

	if (check->value) {
		setObjectGroupColor(&check->oCheck, 1.0f, 1.0f, 1.0f);
	} else {
		setObjectGroupColor(&check->oCheck, 0.5f, 0.5f, 0.5f);
	}

	check->change(check);
}

/*
 * Callback-Funktion: Toggelt den Wert des Check-Objektes.
 */
void pickCheck(void* data) {
	Check* check = data;
	setCheck(check, !check->value);
}

/*
 * Erzeugt ein Check-Objekt. Der Default-Wert eines solchen Objektes ist eins. 
 */
void init3dCheck(Check* check, float z, funcChange change, char* text) {
	initTextObject(&check->oCheck, text, z);

	check->change = change;

	setCheck(check, 1);

	initPick(&check->pCheck, pickCheck, check);
	setObjectPick(&check->oCheck, &check->pCheck);
}

/*** SpinEdit ***/

/*
 * Callback-Funktion: Erniedrigt den Wert dieses SpinEdits.
 */
void pickSpinEditLeft(void* data) {
	SpinEdit* spinedit = data;
	int* value = &spinedit->value;
	if (*value > spinedit->minValue) {
		(*value)--;
		spinedit->change(spinedit);
	}
}

/*
 * Callback-Funktion: Erhöht den Wert dieses SpinEdits.
 */
void pickSpinEditRight(void* data) {
	SpinEdit* spinedit = data;
	int* value = &spinedit->value;
	if (*value < spinedit->maxValue) {
		(*value)++;
		spinedit->change(spinedit);
	}
}

/*
 * Erzeugt ein SpinEdit. Es kann ein Objekt mitgegeben werden, dass den Wert des SpinEdits repräsentiert.
 */
void init3dSpinEdit(SpinEdit* spinedit, int value, int min, int max, float z, Object* obj, funcChange change) {
	spinedit->value = value;
	spinedit->minValue = min;
	spinedit->maxValue = max;

	initObjectGroup(&spinedit->oSpinEdit);

	setObjectPosition3f(&spinedit->oSpinEdit, 0.0f, 0.0f, z);
	rotateObjectX(&spinedit->oSpinEdit, 90.0f);
	setObjectScalef(&spinedit->oSpinEdit, SCALE_FONT);

	/* Pfeil nach Links */
	initObjectGroup(&spinedit->oLeft);
	setObjectPosition2f(&spinedit->oLeft, -4.3f, 0.0f);
	addSubObject(&spinedit->oLeft, &goLeft);
	addSubObject(&spinedit->oSpinEdit, &spinedit->oLeft);
	
	/* Pfeil nach Rechts */	
	initObjectGroup(&spinedit->oRight);
	setObjectPosition2f(&spinedit->oRight, 3.3f, 0.0f);
	addSubObject(&spinedit->oRight, &goRight);
	addSubObject(&spinedit->oSpinEdit, &spinedit->oRight);
	
	/* Objekt in der Mitte */
	addSubObject(&spinedit->oSpinEdit, obj);

	spinedit->change = change;

	change(spinedit);

	/* Events für die Pfeile registrieren */
	initPick(&spinedit->pLeft, pickSpinEditLeft, spinedit);
	setObjectPick(&spinedit->oLeft, &spinedit->pLeft);

	initPick(&spinedit->pRight, pickSpinEditRight, spinedit);
	setObjectPick(&spinedit->oRight, &spinedit->pRight);

}

/*** spezieller Teil fürs Spiel ***/

static Button gbStart;
static Button gbResume;

static Check gcShadows;
static Check gcFog;

/*
 * Die Hilfetexte
 */
static char* gTextHelp[] = {
	"Cursor", "Move",
	"Space", "Jump",
	"W A S D", "Camera",
	"R F", "Zoom",
	"Enter", "Reset",
	"Esc","Menu",
};

static Object goHelpText;
static Object goMainText;

static int gIsPauseMenu;

/* Events */

int gCntBallLayouts;
int gBallLayouts[MAX_BALL_LAYOUTS];

void clickButtonStart(void) {
	resumeGame();
}

void changeBallEdit(void* self) {
	changeBall(gBallLayouts[((SpinEdit*) self)->value]);
}

void changeShadows(void* self) {
	Check* check = self;
	setShadows(check->value);
}

void changeFog(void* self) {
	Check* check = self;
	setFog(check->value);
}

void clickButtonHelp(void) {
	goHelpText.visible = 1;
	goMainText.visible = 0;
}

void clickButtonQuit(void) {
	exit(0);
}

void clickButtonBack(void) {
	goHelpText.visible = 0;
	goMainText.visible = 1;
}

/*
 * Reagiert auf Tastatur eingaben im Menü.
 */
void updateMenu(double interval) {
	if (goMainText.visible) {
		if (!gIsPauseMenu && wasKeyPressed(KEY_ENTER)) {
			clickButtonStart();
		}
		
		if (gIsPauseMenu && wasKeyPressed(KEY_ESC)) {
			clickButtonStart();
		}

		if (wasKeyPressed('h')) {
			clickButtonHelp();
		}

		if (wasKeyPressed('q')) {
			clickButtonQuit();
		}
	} else if (goHelpText.visible) {
		if (wasKeyPressed(KEY_ESC)) {
			clickButtonBack();
		}
	}
}

/*
 * Teielt dem Manü mit, dass es demnächst angezeigt wird und
 * sagt, ob das Menü über ESC angezeigt wurde, oder durch einen Level start.
 */

void showMenu(int pause) {
	gbStart.oButton.visible = !pause;
	gbResume.oButton.visible = pause;

	setCheck(&gcFog, useFog());
	setCheck(&gcShadows, useShadows());

	gIsPauseMenu = pause;

	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

/*
 * Erzeugt das Menü des Spiels.
 */
void initMenu(Object* obj) {
	static Button bQuit;
	static Button bHelp;
	static Button bBack;
	
	static SpinEdit spinEditBall;

	static Object oLogo;	
	static Object oTextHelp[length(gTextHelp)];
	static Object oBall;

	int i;

	/*
	 * Erstellt eine Liste der verfügbaren Bälle.
	 */
	gCntBallLayouts = 0;

	gBallLayouts[gCntBallLayouts++] = BALL_LAYOUT_DEFAULT;

	if (hasBallTexture()) {
		gBallLayouts[gCntBallLayouts++] = BALL_LAYOUT_TEXTURE;
	}
	
	if (hasBallReflection()) {
		gBallLayouts[gCntBallLayouts++] = BALL_LAYOUT_METAL;
	}
	
	if (hasBallShader()) {
		gBallLayouts[gCntBallLayouts++] = BALL_LAYOUT_GOLFBALL;
	}
	
	if (hasBallShader() && hasBallReflection()) {
		gBallLayouts[gCntBallLayouts++] = BALL_LAYOUT_GOLFBALL_METAL;
	}

	/*
	 * Das Menü zusammensetzen
	 */

	initObjectGroup(obj);
	
	/* Menulogo setzen */
	initObject(&oLogo, drawSquare);
	oLogo.texture = loadTexture("data/logo.tga", 0);
	setObjectPosition3f(&oLogo, 0.0f, 0.0f, 8.0f);
	setObjectScale3f(&oLogo, 4.0f, 1.0f, 1.0f);
		
	rotateObjectX(&oLogo, 90.0f);
	addSubObject(obj, &oLogo);

	/* Pfeile initialisieren */
	initMenuObject(&goLeft, loadTexture("data/left.tga", 0));
	initMenuObject(&goRight, loadTexture("data/right.tga", 0));

	/* Menu */
	initObjectGroup(&goMainText);

	/* Buttons */
	initObject(&oBall, drawMenuBall);

	init3dButton(&gbStart, 6.0f, clickButtonStart, "Start");
  addSubObject(&goMainText, &gbStart.oButton);
	
	init3dButton(&gbResume, 6.0f, clickButtonStart, "Resume");
	gbResume.oButton.visible = 0;
  addSubObject(&goMainText, &gbResume.oButton);

	gIsPauseMenu = 0;
	
	init3dSpinEdit(&spinEditBall, gCntBallLayouts - 1, 0, gCntBallLayouts - 1, 5.2f, &oBall, changeBallEdit);
	addSubObject(&goMainText, &spinEditBall.oSpinEdit);
	
	init3dCheck(&gcShadows, 4.0f, changeShadows, "Shadows");
  addSubObject(&goMainText, &gcShadows.oCheck);

	init3dCheck(&gcFog, 3.0f, changeFog, "Fog");
  addSubObject(&goMainText, &gcFog.oCheck);

	init3dButton(&bHelp, 2.0f,clickButtonHelp, "Help");
  addSubObject(&goMainText, &bHelp.oButton);
	
	init3dButton(&bQuit, 1.0f, clickButtonQuit, "Quit");
  addSubObject(&goMainText, &bQuit.oButton);

	addSubObject(obj, &goMainText);

	/* Hilfe */
	initObjectGroup(&goHelpText);
	
	goHelpText.visible = 0;

	for (i = 0; i < length(gTextHelp); i += 2) {
		char* left = gTextHelp[i];
		char* right = ((i + 1) < length(gTextHelp)) ? gTextHelp[i + 1] : NULL;
		float z = 6.0f - i / 2;
		float length;

		if (left) {
			Object* o = &oTextHelp[i];

			length = makeTextObject(o, left) * SCALE_FONT;
			setObjectPosition3f(o, -5.0f, 0.0f, z);
			setObjectScalef(o, SCALE_FONT);
			rotateObjectX(o, 90.0f);
			
	  	addSubObject(&goHelpText, o);
		}

		if (right) {
			Object* o = &oTextHelp[i + 1];
			
			length = makeTextObject(o, right) * SCALE_FONT;
			setObjectPosition3f(o, 5.0f - length, 0.0f, z);
			setObjectScalef(o, SCALE_FONT);
			rotateObjectX(o, 90.0f);
			
	  	addSubObject(&goHelpText, o);
		}
	}

	init3dButton(&bBack, 6.0f - length(gTextHelp) / 2, clickButtonBack, "back");
 	addSubObject(&goHelpText, &bBack.oButton);

	addSubObject(obj, &goHelpText);
}
