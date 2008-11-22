/*
 * CG Madness - a Marble Madness clone
 * Copyright (C) 2007  Sven Reinck <sreinck@gmail.com>
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
 */

#ifndef Editor_hpp
#define Editor_hpp

#include "Process.hpp"

#include "ball/Ball.hpp"

#include "utils/SmartPointer.hpp"

class Editor : public Process
{
public:
	typedef enum
	{
		STATE_PAUSED,
		STATE_EDITING,
		STATE_TESTING
	} EditorState;

public:
	Editor();
	virtual ~Editor();

	void start(Process* previous, bool push);

	void suspend();
	void resume();

	void update(float interval);
	void draw() const;

private:
	Ball mBall;

	EditorState mState;

	void enableTestMode();
	void disableTestMode();

	SmartPointer<class ScreenEditorMain> mScreenEditorMain;
	SmartPointer<class ScreenWait> mScreenWait;
	SmartPointer<class ScreenInfo> mScreenInfo;

	Process* mPrevious;

	void lightMapsReady();
	void saveLevel();

	friend class ScreenEditorMain;

};

void drawEditorField(bool showCursor = false);

#endif
