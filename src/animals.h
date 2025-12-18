#ifndef ANIMALS_H
#define ANIMALS_H

#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <AudioToolbox/AudioToolbox.h>
#include <CoreFoundation/CoreFoundation.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "stb_image.h"

// ---------------- STRUCTS ----------------
typedef struct { float x,y,z; } VPos;
typedef struct { float u,v; } VUV;
typedef struct { int v[3], uv[3]; } Face;
typedef struct { float x,y,z,u,v; } DrawVertex;

typedef struct {
    char* modelFile;
    char* textureFile;
    GLuint textureID;
    VPos *posList;
    VUV *uvList;
    Face *faces;
    DrawVertex *drawVerts;
    int posCount, uvCount, faceCount, drawCount;
    float centerX, centerY, centerZ;
    float scaleMul;
    float heightOffset;
    float xPos, zPos;
    float minY, maxY;
} Animal;

// ---------------- GLOBALS ----------------
extern int totalAnimals;
extern Animal animals[5];
extern float realSizeScale[5];
extern float camX, camZ, camAngle, camHeight;
extern float moveSpeed, rotSpeed;

// ---------------- FUNCTIONS ----------------
void playSound(char* file);
GLuint loadTexture(const char* file);
void loadOBJ(Animal* A);
void drawAnimal(Animal* A, int index);
void initAnimals();

#endif
