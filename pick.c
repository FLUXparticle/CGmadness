#include "pick.h"

#include "list.h"

#include "debug.h"

#include <GL/gl.h>

#include <stdio.h>

int gCntPickFuncs = 0;

List gPickList = NULL;

/*
 * initzialisiert ein Pick-Objekt
 */
void initPick(Pick* pick, funcPick f, void* data) {
	pick->pickName = -1;
	pick->pick = f;
	pick->data = data;
}

/*
 * verbindet ein Pick-Objekt mit einem Objekt aus dem Scene-Graphen,
 * so dass bei anklicken dieses Objektes die entsprechende Pick-Funktion ausgelößt wird.
 */

void setObjectPick(Object* obj, Pick* pick) {
	obj->pickName = gCntPickFuncs;
	pick->pickName = gCntPickFuncs;
	gPickList = appendElement(gPickList, pick);
	gCntPickFuncs++;
}

/*
 * Ruft die Pick-Funktion des angeklickten Objektes auf
 */
void doPick(int name) {
	List run;
	/* Callback-Funktion finden und ausführen */
	for (run = gPickList; run; run = run->next) {
		Pick* pick = run->info;
		if (pick->pickName == name) {
			pick->pick(pick->data);
		}
	}
}
