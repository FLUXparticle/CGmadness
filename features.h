#ifndef _features_h_
#define _features_h_

void initFeatures(int argc, char* argv[]);

int hasShader(void);
int hasFramebuffer(void);
int hasVertexbuffer(void);
int hasTwoSideStencil(void);

void setFog(int use);
void setShadows(int use);

int useShadows(void);
int useFog(void);

int hasSpotlight(void);
int useSpotlight(void);

#endif
