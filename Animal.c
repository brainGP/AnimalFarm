// gcc Animal.c -o animal -framework OpenGL -framework GLUT
// ./animal


// #include <GLUT/glut.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

// typedef struct {
//     float x, y, z;
// } VPos;

// typedef struct {
//     float u, v;
// } VUV;

// typedef struct {
//     int v[3];
//     int uv[3];
// } Face;

// typedef struct {
//     float x, y, z;
//     float u, v;
// } DrawVertex;

// VPos *posList = NULL;
// VUV *uvList  = NULL;
// Face *faces  = NULL;

// DrawVertex *drawVerts = NULL;
// int drawCount = 0;

// int posCount = 0, uvCount = 0, faceCount = 0;

// GLuint textureID;
// float xRot = 0, yRot = 0;

// float centerX = 0, centerY = 0, centerZ = 0;
// float scale = 1.0f;

// // Load OBJ properly (unique pos+uv)
// void loadOBJ(const char *filename) {
//     FILE *f = fopen(filename, "r");
//     if (!f) {
//         printf("OBJ not found: %s\n", filename);
//         return;
//     }

//     char line[512];
//     posCount = uvCount = faceCount = 0;

//     // PASS 1 — count elements
//     while (fgets(line, sizeof(line), f)) {
//         if (!strncmp(line, "v ", 2)) posCount++;
//         else if (!strncmp(line, "vt ", 3)) uvCount++;
//         else if (!strncmp(line, "f ", 2)) faceCount++;
//     }

//     rewind(f);

//     posList = malloc(sizeof(VPos) * posCount);
//     uvList = malloc(sizeof(VUV) * uvCount);
//     faces   = malloc(sizeof(Face) * faceCount);

//     int pi = 0, ui = 0, fi = 0;

//     float minX=99999, minY=99999, minZ=99999;
//     float maxX=-99999, maxY=-99999, maxZ=-99999;

//     // PASS 2 — load data
//     while (fgets(line, sizeof(line), f)) {
//         if (!strncmp(line, "v ", 2)) {
//             float x,y,z;
//             sscanf(line, "v %f %f %f", &x, &y, &z);
//             posList[pi].x = x;
//             posList[pi].y = y;
//             posList[pi].z = z;

//             if (x<minX) minX=x; if (y<minY) minY=y; if (z<minZ) minZ=z;
//             if (x>maxX) maxX=x; if (y>maxY) maxY=y; if (z>maxZ) maxZ=z;
//             pi++;

//         } else if (!strncmp(line, "vt ", 3)) {
//             float u,v;
//             sscanf(line, "vt %f %f", &u, &v);
//             uvList[ui].u = u;
//             uvList[ui].v = 1.0f - v;
//             ui++;

//         } else if (!strncmp(line, "f ", 2)) {
//             int v[3], t[3];
//             sscanf(line, "f %d/%d %d/%d %d/%d",
//                    &v[0], &t[0],
//                    &v[1], &t[1],
//                    &v[2], &t[2]);

//             for (int k=0;k<3;k++){
//                 faces[fi].v[k]  = v[k] - 1;
//                 faces[fi].uv[k] = t[k] - 1;
//             }
//             fi++;
//         }
//     }
//     fclose(f);

//     // Build real render vertices
//     drawCount = faceCount * 3;
//     drawVerts = malloc(sizeof(DrawVertex) * drawCount);

//     int di = 0;
//     for (int i=0;i<faceCount;i++){
//         for (int k=0;k<3;k++){
//             int vi = faces[i].v[k];
//             int ti = faces[i].uv[k];

//             drawVerts[di].x = posList[vi].x;
//             drawVerts[di].y = posList[vi].y;
//             drawVerts[di].z = posList[vi].z;

//             drawVerts[di].u = uvList[ti].u;
//             drawVerts[di].v = uvList[ti].v;
//             di++;
//         }
//     }

//     centerX = (minX+maxX)/2;
//     centerY = (minY+maxY)/2;
//     centerZ = (minZ+maxZ)/2;

//     float sizeX = maxX-minX;
//     float sizeY = maxY-minY;
//     float sizeZ = maxZ-minZ;
//     float maxDim = sizeX;
//     if (sizeY > maxDim) maxDim = sizeY;
//     if (sizeZ > maxDim) maxDim = sizeZ;

//     scale = 30.0f / maxDim;
// }

// GLuint loadTexture(const char *file) {
//     int w,h,n;
//     unsigned char *data = stbi_load(file, &w, &h, &n, 3);
//     if (!data) {
//         printf("Texture load failed: %s\n", file);
//         return 0;
//     }

//     GLuint tex;
//     glGenTextures(1,&tex);
//     glBindTexture(GL_TEXTURE_2D,tex);

//     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

//     glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,w,h,0,GL_RGB,GL_UNSIGNED_BYTE,data);

//     stbi_image_free(data);
//     return tex;
// }

// void display(){
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//     glLoadIdentity();

//     glTranslatef(0,0,-50);
//     glRotatef(xRot,1,0,0);
//     glRotatef(yRot,0,1,0);
//     glScalef(scale,scale,scale);
//     glTranslatef(-centerX,-centerY,-centerZ);

//     glEnable(GL_TEXTURE_2D);
//     glBindTexture(GL_TEXTURE_2D, textureID);

//     glBegin(GL_TRIANGLES);
//     for (int i=0;i<drawCount;i++){
//         glTexCoord2f(drawVerts[i].u, drawVerts[i].v);
//         glVertex3f(drawVerts[i].x, drawVerts[i].y, drawVerts[i].z);
//     }
//     glEnd();

//     glutSwapBuffers();
// }

// void reshape(int w,int h){
//     glViewport(0,0,w,h);
//     glMatrixMode(GL_PROJECTION);
//     glLoadIdentity();
//     gluPerspective(45,(float)w/h,1,1000);
//     glMatrixMode(GL_MODELVIEW);
// }

// void keys(int key,int x,int y){
//     if (key == GLUT_KEY_LEFT)  yRot -= 3;
//     if (key == GLUT_KEY_RIGHT) yRot += 3;
//     if (key == GLUT_KEY_UP)    xRot -= 3;
//     if (key == GLUT_KEY_DOWN)  xRot += 3;
//     glutPostRedisplay();
// }

// int main(int argc,char **argv){
//     glutInit(&argc,argv);
//     glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
//     glutInitWindowSize(1920,1080);
//     glutCreateWindow("Animal Viewer");

//     glEnable(GL_DEPTH_TEST);
//     glClearColor(0.1,0.1,0.1,1); 
//     glDisable(GL_LIGHTING); 
//     glDisable(GL_LIGHT0);

//     loadOBJ("Camel2/Camel.obj");
//     textureID = loadTexture("Camel2/Camel2_basecolor.JPEG");

//     glutDisplayFunc(display);
//     glutReshapeFunc(reshape);
//     glutSpecialFunc(keys);

//     glutMainLoop();
//     return 0;
// }


// Animal.c — Fixed: per-animal sizes, sound (from Sounds/click.wav), safe reload/free
// Compile (macOS):
// gcc Animal.c -o animal -framework OpenGL -framework GLUT -framework AudioToolbox -framework CoreFoundation -DGL_SILENCE_DEPRECATION


// AnimalScene.c
// Builds a circular scene with 5 animals, camera moves in X/Z with arrow keys,
// and per-animal sounds triggered by keys (h,c,s,m,g).
//
// Compile (macOS):
// gcc AnimalScene.c -o animal -framework OpenGL -framework GLUT \
//   -framework AudioToolbox -framework CoreFoundation -DGL_SILENCE_DEPRECATION




// Animal.c
// Full scene: all 5 animals around player, human-height camera, WASD move, arrow rotate,
// green ground, per-animal sizes, per-animal sounds.
// Compile (macOS):
// gcc Animal.c -o animal -framework OpenGL -framework GLUT -framework AudioToolbox -framework CoreFoundation -DGL_SILENCE_DEPRECATION



#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <AudioToolbox/AudioToolbox.h>
#include <CoreFoundation/CoreFoundation.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

int totalAnimals=5;
Animal animals[5];


float realSizeScale[5] = {1.0f, 0.9f, 0.65f, 1.2f, 0.6f};

// camera
float camX=0, camZ=-50.0f;
float camAngle=180.0f;
float camHeight=8.0f; 
float moveSpeed=2.0f, rotSpeed=10.0f;

// ---------- AUDIO ----------
void playSound(char* file){
    SystemSoundID id;
    CFURLRef url = CFBundleCopyResourceURL(CFBundleGetMainBundle(),
                                           CFStringCreateWithCString(NULL,file,kCFStringEncodingUTF8),
                                           CFSTR("wav"),NULL);
    if(!url) return;
    AudioServicesCreateSystemSoundID(url,&id);
    AudioServicesPlaySystemSound(id);
}

// ---------- TEXTURE ----------
GLuint loadTexture(const char* file){
    int w,h,n;
    unsigned char* data = stbi_load(file,&w,&h,&n,3);
    if(!data){ printf("Texture FAILED: %s\n",file); return 0; }
    GLuint tex; glGenTextures(1,&tex); glBindTexture(GL_TEXTURE_2D,tex);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,w,h,0,GL_RGB,GL_UNSIGNED_BYTE,data);
    stbi_image_free(data);
    return tex;
}

// ---------- LOAD OBJ ----------
void loadOBJ(Animal* A){
    FILE* f = fopen(A->modelFile,"r");
    if(!f){ printf("OBJ not found: %s\n",A->modelFile); return; }
    char line[512];
    A->posCount=A->uvCount=A->faceCount=0;
    while(fgets(line,sizeof(line),f)){
        if(!strncmp(line,"v ",2)) A->posCount++;
        else if(!strncmp(line,"vt ",3)) A->uvCount++;
        else if(!strncmp(line,"f ",2)) A->faceCount++;
    }
    rewind(f);
    A->posList=malloc(sizeof(VPos)*A->posCount);
    A->uvList=malloc(sizeof(VUV)*A->uvCount);
    A->faces=malloc(sizeof(Face)*A->faceCount);
    int pi=0,ui=0,fi=0;
    float minX=99999,minY=99999,minZ=99999;
    float maxX=-99999,maxY=-99999,maxZ=-99999;
    while(fgets(line,sizeof(line),f)){
        if(!strncmp(line,"v ",2)){
            float x,y,z; sscanf(line,"v %f %f %f",&x,&y,&z);
            A->posList[pi].x=x; A->posList[pi].y=y; A->posList[pi].z=z;
            if(x<minX) minX=x; if(y<minY) minY=y; if(z<minZ) minZ=z;
            if(x>maxX) maxX=x; if(y>maxY) maxY=y; if(z>maxZ) maxZ=z;
            pi++;
        } else if(!strncmp(line,"vt ",3)){
            float u,v; sscanf(line,"vt %f %f",&u,&v);
            A->uvList[ui].u=u; A->uvList[ui].v=1.0f-v; ui++;
        } else if(!strncmp(line,"f ",2)){
            int v[3],t[3]; sscanf(line,"f %d/%d %d/%d %d/%d",&v[0],&t[0],&v[1],&t[1],&v[2],&t[2]);
            for(int k=0;k<3;k++){ A->faces[fi].v[k]=v[k]-1; A->faces[fi].uv[k]=t[k]-1; }
            fi++;
        }
    }
    fclose(f);
    A->drawCount=A->faceCount*3;
    A->drawVerts=malloc(sizeof(DrawVertex)*A->drawCount);
    int di=0;
    for(int i=0;i<A->faceCount;i++){
        for(int k=0;k<3;k++){
            int vi=A->faces[i].v[k]; int ti=A->faces[i].uv[k];
            A->drawVerts[di].x=A->posList[vi].x;
            A->drawVerts[di].y=A->posList[vi].y;
            A->drawVerts[di].z=A->posList[vi].z;
            A->drawVerts[di].u=A->uvList[ti].u;
            A->drawVerts[di].v=A->uvList[ti].v;
            di++;
        }
    }
    A->centerX=(minX+maxX)/2; A->centerY=(minY+maxY)/2; A->centerZ=(minZ+maxZ)/2;
    float maxDim = fmax(fmax(maxX-minX,maxY-minY),maxZ-minZ);
    A->scaleMul = 25.0f/maxDim;
    A->heightOffset = -minY*A->scaleMul;
    A->minY=minY; A->maxY=maxY;
}

// ---------- DRAW ANIMAL ----------
void drawAnimal(Animal* A, int index){
    glPushMatrix();
    // move X/Z
    glTranslatef(A->xPos, 0, A->zPos); // start Y at 0
    glRotatef(90.0f,0,1,0); // rotate Y
    // scale
    glScalef(A->scaleMul * realSizeScale[index],
             A->scaleMul * realSizeScale[index],
             A->scaleMul * realSizeScale[index]);
    // move model center so base touches Y=0
    glTranslatef(-A->centerX, -A->minY, -A->centerZ); 
    glBindTexture(GL_TEXTURE_2D,A->textureID);
    glBegin(GL_TRIANGLES);
    for(int j=0;j<A->drawCount;j++){
        glTexCoord2f(A->drawVerts[j].u,A->drawVerts[j].v);
        glVertex3f(A->drawVerts[j].x,A->drawVerts[j].y,A->drawVerts[j].z);
    }
    glEnd();
    glPopMatrix();
}


// ---------- DISPLAY ----------
void display(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    float rad=camAngle*M_PI/180.0;
    gluLookAt(camX,camHeight,camZ, camX+sin(rad),camHeight,camZ-cos(rad), 0,1,0);
    glEnable(GL_TEXTURE_2D);
    for(int i=0;i<totalAnimals;i++) drawAnimal(&animals[i], i);
    glutSwapBuffers();
}

// ---------- KEYBOARD ----------
void keyboard(unsigned char key,int x,int y){
    switch(key){
        case 27: exit(0);
        case 'H': playSound("Horse"); break;
        case 'C': playSound("Cow"); break;
        case 'S': playSound("Sheep"); break;
        case 'M': playSound("Camel"); break;
        case 'G': playSound("Goat"); break;
    }
}

// ---------- CAMERA MOVEMENT ----------
void special(int key,int x,int y){
    float rad=camAngle*M_PI/180.0;
    if(key==GLUT_KEY_UP){ camX+=sin(rad)*moveSpeed; camZ-=cos(rad)*moveSpeed; }
    if(key==GLUT_KEY_DOWN){ camX-=sin(rad)*moveSpeed; camZ+=cos(rad)*moveSpeed; }
    if(key==GLUT_KEY_LEFT){ camAngle-=rotSpeed; }
    if(key==GLUT_KEY_RIGHT){ camAngle+=rotSpeed; }
    glutPostRedisplay();
}

// ---------- INIT ----------
void init(){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.5,0.8,1.0,1.0);

    // animal models
    animals[0].modelFile="Models/Horse/Horse.obj"; animals[0].textureFile="Models/Horse/Horse.jpeg";
    animals[1].modelFile="Models/Cow/Cow.obj";     animals[1].textureFile="Models/Cow/Cow.jpeg";
    animals[2].modelFile="Models/Sheep/Sheep.obj"; animals[2].textureFile="Models/Sheep/Sheep.jpeg";
    animals[3].modelFile="Models/Camel/Camel.obj"; animals[3].textureFile="Models/Camel/Camel.jpeg";
    animals[4].modelFile="Models/Goat/Goat.obj";   animals[4].textureFile="Models/Goat/Goat.jpeg";

    // position animals in line
    float startX=-20.0f;
    float spacing=10.0f;

    for(int i=0;i<totalAnimals;i++){
        loadOBJ(&animals[i]);
        animals[i].textureID=loadTexture(animals[i].textureFile);
        animals[i].xPos=startX + i*spacing;
        animals[i].zPos=0;
    }
}

// ---------- RESHAPE ----------
void reshape(int w,int h){
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,(float)w/h,0.1,200.0);
    glMatrixMode(GL_MODELVIEW);
}

// ---------- MAIN ----------
int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(1280,720);
    glutCreateWindow("Mongolian Animals 3D");
    glutFullScreen();

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);

    glutMainLoop();
    return 0;
}
