#ifndef _keyboard_h_
#define _keyboard_h_

#define KEY_ESC 27
#define KEY_ENTER 13

#define CURSOR_UP 0
#define CURSOR_DOWN 1
#define CURSOR_LEFT 2
#define CURSOR_RIGHT 3

int wasKeyPressed(unsigned char key);
int isKeyPressed(unsigned char key);
int isCursorPressed(int cursor);
int wasCursorPressed(int cursor);
int isFunctionPressed(int index);
int wasFunctionPressed(int index);

void startKeyboard(void);

#endif
