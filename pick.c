/*
 * CG Madness - a Marble Madness clone
 * Copyright (C) 2007  Sven Reinck
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * $Id$
 *
 */

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
