#ifndef _game_h_
#define _game_h_

void loadNewLevel(void); 
void initGame(char* filename);
void initLevel(char* filename);
void resumeGame(void);

void drawGameReflection(void);

extern int sgGameMainLight;
extern int sgGameSpotLight;

extern int sgRenderPass;

#endif
