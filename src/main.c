
// #include <GL/gl.h>
// #include "my_glu.h"
// #include <GLUT/glut.h>

// #include <stdio.h>
// #include <stdlib.h>
// #include <math.h>
// #include <unistd.h>
// #include <stdbool.h>

// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

// #ifndef M_PI
// #define M_PI 3.14159265358979323846
// #endif

// // ------------------ SKYBOX ------------------
// GLuint textures[6];

// // Camera
// float cameraYaw = 0.0f;
// float cameraPitch = 0.0f;
// int lastX = 640, lastY = 360;
// int firstMouse = 1;

// // ------------------ ANIMALS ------------------
// typedef struct { float x,y,z; } VPos;
// typedef struct { float u,v; } VUV;
// typedef struct { int v[3]; int uv[3]; } Face;
// typedef struct { float x,y,z,u,v; } DrawVertex;

// typedef struct {
//     char* modelFile;
//     char* textureFile;
//     GLuint textureID;
//     VPos* posList;
//     VUV* uvList;
//     Face* faces;
//     DrawVertex* drawVerts;
//     int posCount, uvCount, faceCount, drawCount;
//     float centerX, centerY, centerZ;
//     float scaleMul;
//     float heightOffset;
//     float minY, maxY;
//     float xPos, zPos;
// } Animal;

// #define totalAnimals 5
// Animal animals[totalAnimals];
// float realSizeScale[totalAnimals] = {1,1,1,1,1};

// // ------------------ TEXTURE LOADING ------------------
// GLuint loadTexture(const char* filename) {
//     GLuint textureID;
//     glGenTextures(1, &textureID);
    
//     int width, height, nrChannels;
//     unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
//     if (data) {
//         GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
//         glBindTexture(GL_TEXTURE_2D, textureID);
//         glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//         stbi_image_free(data);
//         return textureID;
//     } else {
//         printf("Failed to load texture: %s\n", filename);
//         return 0;
//     }
// }

// // ------------------ OBJ LOADING ------------------
// void loadOBJ(Animal* A) {
//     FILE* f = fopen(A->modelFile,"r");
//     if(!f){ printf("OBJ not found: %s\n",A->modelFile); return; }
//     char line[512];
//     A->posCount=A->uvCount=A->faceCount=0;
//     while(fgets(line,sizeof(line),f)){
//         if(!strncmp(line,"v ",2)) A->posCount++;
//         else if(!strncmp(line,"vt ",3)) A->uvCount++;
//         else if(!strncmp(line,"f ",2)) A->faceCount++;
//     }
//     rewind(f);
//     A->posList=malloc(sizeof(VPos)*A->posCount);
//     A->uvList=malloc(sizeof(VUV)*A->uvCount);
//     A->faces=malloc(sizeof(Face)*A->faceCount);
//     int pi=0, ui=0, fi=0;
//     float minX=99999,minY=99999,minZ=99999;
//     float maxX=-99999,maxY=-99999,maxZ=-99999;
//     while(fgets(line,sizeof(line),f)){
//         if(!strncmp(line,"v ",2)){
//             float x,y,z; sscanf(line,"v %f %f %f",&x,&y,&z);
//             A->posList[pi].x=x; A->posList[pi].y=y; A->posList[pi].z=z;
//             if(x<minX) minX=x; if(y<minY) minY=y; if(z<minZ) minZ=z;
//             if(x>maxX) maxX=x; if(y>maxY) maxY=y; if(z>maxZ) maxZ=z;
//             pi++;
//         } else if(!strncmp(line,"vt ",3)){
//             float u,v; sscanf(line,"vt %f %f",&u,&v);
//             A->uvList[ui].u=u; A->uvList[ui].v=1.0f-v; ui++;
//         } else if(!strncmp(line,"f ",2)){
//             int v[3],t[3]; sscanf(line,"f %d/%d %d/%d %d/%d",&v[0],&t[0],&v[1],&t[1],&v[2],&t[2]);
//             for(int k=0;k<3;k++){ A->faces[fi].v[k]=v[k]-1; A->faces[fi].uv[k]=t[k]-1; }
//             fi++;
//         }
//     }
//     fclose(f);
//     A->drawCount=A->faceCount*3;
//     A->drawVerts=malloc(sizeof(DrawVertex)*A->drawCount);
//     int di=0;
//     for(int i=0;i<A->faceCount;i++){
//         for(int k=0;k<3;k++){
//             int vi=A->faces[i].v[k]; int ti=A->faces[i].uv[k];
//             A->drawVerts[di].x=A->posList[vi].x;
//             A->drawVerts[di].y=A->posList[vi].y;
//             A->drawVerts[di].z=A->posList[vi].z;
//             A->drawVerts[di].u=A->uvList[ti].u;
//             A->drawVerts[di].v=A->uvList[ti].v;
//             di++;
//         }
//     }
//     A->centerX=(minX+maxX)/2; A->centerY=(minY+maxY)/2; A->centerZ=(minZ+maxZ)/2;
//     float maxDim = fmax(fmax(maxX-minX,maxY-minY),maxZ-minZ);
//     A->scaleMul = 25.0f/maxDim;
//     A->heightOffset = -minY*A->scaleMul;
//     A->minY=minY; A->maxY=maxY;
// }

// // ------------------ DRAW ANIMAL ------------------
// void drawAnimal(Animal* A, int index) {
//     glPushMatrix();
//     glTranslatef(A->xPos, 0, A->zPos);
//     glRotatef(90.0f,0,1,0);
//     glScalef(A->scaleMul * realSizeScale[index],
//              A->scaleMul * realSizeScale[index],
//              A->scaleMul * realSizeScale[index]);
//     glTranslatef(-A->centerX, -A->minY, -A->centerZ);
//     glBindTexture(GL_TEXTURE_2D,A->textureID);
//     glBegin(GL_TRIANGLES);
//     for(int j=0;j<A->drawCount;j++){
//         glTexCoord2f(A->drawVerts[j].u,A->drawVerts[j].v);
//         glVertex3f(A->drawVerts[j].x,A->drawVerts[j].y,A->drawVerts[j].z);
//     }
//     glEnd();
//     glPopMatrix();
// }

// // ------------------ INIT ANIMALS ------------------
// void initAnimals() {
//     animals[0].modelFile="Models/Horse/Horse.obj"; animals[0].textureFile="Models/Horse/Horse.jpeg";
//     animals[1].modelFile="Models/Cow/Cow.obj";     animals[1].textureFile="Models/Cow/Cow.jpeg";
//     animals[2].modelFile="Models/Sheep/Sheep.obj"; animals[2].textureFile="Models/Sheep/Sheep.jpeg";
//     animals[3].modelFile="Models/Camel/Camel.obj"; animals[3].textureFile="Models/Camel/Camel.jpeg";
//     animals[4].modelFile="Models/Goat/Goat.obj";   animals[4].textureFile="Models/Goat/Goat.jpeg";

//     float startX=-20.0f;
//     float spacing=15.0f;
//     for(int i=0;i<totalAnimals;i++){
//         loadOBJ(&animals[i]);
//         animals[i].textureID=loadTexture(animals[i].textureFile);
//         animals[i].xPos=startX + i*spacing;
//         animals[i].zPos=0;
//     }
// }

// // ------------------ DISPLAY ------------------
// void display() {
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
//     glMatrixMode(GL_PROJECTION);
//     glLoadIdentity();
//     gluPerspective(30.0, 1280.0 / 720.0, 0.1, 5000.0);  
    
//     glMatrixMode(GL_MODELVIEW);
//     glLoadIdentity();
    
//     glRotatef(-cameraPitch, 1.0f, 0.0f, 0.0f);
//     glRotatef(-cameraYaw, 0.0f, 1.0f, 0.0f);
    
//     glDisable(GL_LIGHTING);

//     float size = 100.0f;  
    
//     glEnable(GL_TEXTURE_2D);
//     glColor3f(1.0f, 1.0f, 1.0f);
//     glDepthMask(GL_FALSE);
//     glDisable(GL_CULL_FACE);
    
//     // --- Skybox ---
//     GLuint skyIndices[6] = {4,5,0,1,2,3};
//     float coords[6][4][3] = {
//         {{-size,-size,size},{size,-size,size},{size,size,size},{-size,size,size}},
//         {{size,-size,-size},{-size,-size,-size},{-size,size,-size},{size,size,-size}},
//         {{size,-size,size},{size,-size,-size},{size,size,-size},{size,size,size}},
//         {{-size,-size,-size},{-size,-size,size},{-size,size,size},{-size,size,-size}},
//         {{-size,size,size},{size,size,size},{size,size,-size},{-size,size,-size}},
//         {{-size,-size,-size},{size,-size,-size},{size,-size,size},{-size,-size,size}}
//     };
//     for(int i=0;i<6;i++){
//         glBindTexture(GL_TEXTURE_2D,textures[skyIndices[i]]);
//         glBegin(GL_QUADS);
//         for(int j=0;j<4;j++) glTexCoord2f((j==1||j==2), (j>=2)) , glVertex3f(coords[i][j][0], coords[i][j][1], coords[i][j][2]);
//         glEnd();
//     }

//     glDepthMask(GL_TRUE);

//     // --- Draw animals ---
//     for(int i=0;i<totalAnimals;i++){
//         drawAnimal(&animals[i], i);
//     }

//     glDisable(GL_TEXTURE_2D);
    
//     glutSwapBuffers();
// }

// // ------------------ OTHER CALLBACKS ------------------
// void reshape(int w, int h){ glViewport(0,0,w,h); }
// void keyboard(unsigned char key,int x,int y){ if(key==27) exit(0); }
// void specialKeys(int key,int x,int y){
//     float rot=2.0f;
//     switch(key){
//         case GLUT_KEY_LEFT: cameraYaw-=rot; break;
//         case GLUT_KEY_RIGHT: cameraYaw+=rot; break;
//         case GLUT_KEY_UP: cameraPitch+=rot; if(cameraPitch>89) cameraPitch=89; break;
//         case GLUT_KEY_DOWN: cameraPitch-=rot; if(cameraPitch<-89) cameraPitch=-89; break;
//     }
//     glutPostRedisplay();
// }
// void passiveMotion(int x,int y){
//     if(firstMouse){ lastX=x; lastY=y; firstMouse=0; return; }
//     float xoff=x-lastX; float yoff=lastY-y; lastX=x; lastY=y;
//     float sens=0.1f; xoff*=sens; yoff*=sens;
//     cameraYaw+=xoff; cameraPitch+=yoff;
//     if(cameraPitch>89) cameraPitch=89; if(cameraPitch<-89) cameraPitch=-89;
//     glutPostRedisplay();
// }
// void idle(){ glutPostRedisplay(); }

// // ------------------ MAIN ------------------
// int main(int argc,char** argv){
//     char cwd[1024]; if(getcwd(cwd,sizeof(cwd))) printf("Current dir: %s\n",cwd);

//     glutInit(&argc,argv);
//     glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
//     glutInitWindowSize(1280,720);
//     glutCreateWindow("Skybox + Animals");

//     glEnable(GL_DEPTH_TEST);
//     glDepthFunc(GL_LEQUAL);
//     glClearColor(0.1f,0.1f,0.1f,1.0f);
//     stbi_set_flip_vertically_on_load(0);

//     // Load skybox textures
//     const char* faces[6] = {"Images/pz.png","Images/nz.png","Images/py.png","Images/ny.png","Images/px.png","Images/nx.png"};
//     for(int i=0;i<6;i++) textures[i]=loadTexture(faces[i]);

//     // Load animals
//     initAnimals();

//     glutDisplayFunc(display);
//     glutReshapeFunc(reshape);
//     glutKeyboardFunc(keyboard);
//     glutSpecialFunc(specialKeys);
//     glutPassiveMotionFunc(passiveMotion);
//     glutIdleFunc(idle);
//     glutSetCursor(GLUT_CURSOR_NONE);
//     glutMainLoop();
//     return 0;
// }




#include <GL/gl.h>
#include <GLUT/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <stdbool.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <AudioToolbox/AudioToolbox.h>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ------------------ SKYBOX ------------------
GLuint textures[6];

// ------------------ CAMERA ------------------
float camX=0.0f, camY=5.0f, camZ=-20.0f;
float cameraYaw=90.0f, cameraPitch=0.0f;
float camSpeed=0.5f;
float dirX=0.0f, dirY=0.0f, dirZ=-1.0f;

int lastX=640,lastY=360;
int firstMouse=1;

// ------------------ ANIMALS ------------------
typedef struct { float x,y,z; } VPos;
typedef struct { float u,v; } VUV;
typedef struct { int v[3]; int uv[3]; } Face;
typedef struct { float x,y,z,u,v; } DrawVertex;

typedef struct {
    char* modelFile;
    char* textureFile;
    GLuint textureID;
    VPos* posList;
    VUV* uvList;
    Face* faces;
    DrawVertex* drawVerts;
    int posCount, uvCount, faceCount, drawCount;
    float centerX, centerY, centerZ;
    float scaleMul;
    float heightOffset;
    float minY, maxY;
    float xPos, zPos;
} Animal;

#define totalAnimals 5
Animal animals[totalAnimals];
float realSizeScale[totalAnimals] = {1.0f,0.8f,0.5f,0.9f,0.4f};

// ------------------ TEXTURE LOADING ------------------
GLuint loadTexture(const char* filename){
    GLuint textureID;
    glGenTextures(1,&textureID);
    int w,h,nr;
    unsigned char* data = stbi_load(filename,&w,&h,&nr,0);
    if(data){
        GLenum format=(nr==4)?GL_RGBA:GL_RGB;
        glBindTexture(GL_TEXTURE_2D,textureID);
        glTexImage2D(GL_TEXTURE_2D,0,format,w,h,0,format,GL_UNSIGNED_BYTE,data);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        stbi_image_free(data);
        return textureID;
    }else{ printf("Failed to load: %s\n",filename); return 0; }
}

// ------------------ SOUND OF ANIMAL ------------------
SystemSoundID animalSounds[totalAnimals];

void initSounds() {
    const char* soundFiles[totalAnimals] = {
        "Sounds/Horse.wav",
        "Sounds/Cow.wav",
        "Sounds/Sheep.wav",
        "Sounds/Camel.wav",
        "Sounds/Goat.wav"
    };

    for(int i=0; i<totalAnimals; i++){
        CFStringRef path = CFStringCreateWithCString(NULL, soundFiles[i], kCFStringEncodingUTF8);
        CFURLRef soundURL = CFURLCreateWithFileSystemPath(NULL, path, kCFURLPOSIXPathStyle, false);
        AudioServicesCreateSystemSoundID(soundURL, &animalSounds[i]);
        CFRelease(soundURL);
        CFRelease(path);
    }
}



// ------------------ OBJ LOADING ------------------
void loadOBJ(Animal* A){
    FILE* f=fopen(A->modelFile,"r");
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
    float minX=99999,minY=99999,minZ=99999,maxX=-99999,maxY=-99999,maxZ=-99999;
    while(fgets(line,sizeof(line),f)){
        if(!strncmp(line,"v ",2)){
            float x,y,z; sscanf(line,"v %f %f %f",&x,&y,&z);
            A->posList[pi].x=x; A->posList[pi].y=y; A->posList[pi].z=z;
            if(x<minX) minX=x; if(y<minY) minY=y; if(z<minZ) minZ=z;
            if(x>maxX) maxX=x; if(y>maxY) maxY=y; if(z>maxZ) maxZ=z;
            pi++;
        }else if(!strncmp(line,"vt ",3)){
            float u,v; sscanf(line,"vt %f %f",&u,&v);
            A->uvList[ui].u=u; A->uvList[ui].v=1.0f-v; ui++;
        }else if(!strncmp(line,"f ",2)){
            int v[3],t[3]; sscanf(line,"f %d/%d %d/%d %d/%d",&v[0],&t[0],&v[1],&t[1],&v[2],&t[2]);
            for(int k=0;k<3;k++){ A->faces[fi].v[k]=v[k]-1; A->faces[fi].uv[k]=t[k]-1; }
            fi++;
        }
    }
    fclose(f);
    A->drawCount=A->faceCount*3;
    A->drawVerts=malloc(sizeof(DrawVertex)*A->drawCount);
    int di=0;
    for(int i=0;i<A->faceCount;i++)
        for(int k=0;k<3;k++){
            int vi=A->faces[i].v[k]; int ti=A->faces[i].uv[k];
            A->drawVerts[di].x=A->posList[vi].x;
            A->drawVerts[di].y=A->posList[vi].y;
            A->drawVerts[di].z=A->posList[vi].z;
            A->drawVerts[di].u=A->uvList[ti].u;
            A->drawVerts[di].v=A->uvList[ti].v;
            di++;
        }
    A->centerX=(minX+maxX)/2; A->centerY=(minY+maxY)/2; A->centerZ=(minZ+maxZ)/2;
    float maxDim=fmax(fmax(maxX-minX,maxY-minY),maxZ-minZ);
    A->scaleMul=15.0f/maxDim;
    A->heightOffset=-minY*A->scaleMul;
    A->minY=minY; A->maxY=maxY;
}

// ------------------ DRAW ANIMAL ------------------
void drawAnimal(Animal* A,int index){
    glPushMatrix();
    glTranslatef(A->xPos,0,A->zPos);
    glRotatef(90,0,1,0);
    glScalef(A->scaleMul*realSizeScale[index],
             A->scaleMul*realSizeScale[index],
             A->scaleMul*realSizeScale[index]);
    glTranslatef(-A->centerX,-A->minY,-A->centerZ);
    glBindTexture(GL_TEXTURE_2D,A->textureID);
    glBegin(GL_TRIANGLES);
    for(int j=0;j<A->drawCount;j++){
        glTexCoord2f(A->drawVerts[j].u,A->drawVerts[j].v);
        glVertex3f(A->drawVerts[j].x,A->drawVerts[j].y,A->drawVerts[j].z);
    }
    glEnd();
    glPopMatrix();
}

// ------------------ DRAW SHADOW ------------------
void drawShadow(Animal* A,int index){
    float light[3]={50.0f,100.0f,50.0f}; // light position
    float shadowMat[16] = {
        light[1],0,0,0,
        -light[0],0,-light[2],-1,
        0,0,light[1],0,
        0,0,0,light[1]
    };
    glPushMatrix();
    glTranslatef(0,0,0);
    glMultMatrixf(shadowMat);
    glTranslatef(A->xPos,0,A->zPos);
    glRotatef(90,0,1,0);
    glScalef(A->scaleMul*realSizeScale[index],
             A->scaleMul*realSizeScale[index],
             A->scaleMul*realSizeScale[index]);
    glTranslatef(-A->centerX,-A->minY,-A->centerZ);
    glColor4f(0,0,0,0.5f);
    glBegin(GL_TRIANGLES);
    for(int j=0;j<A->drawCount;j++){
        glVertex3f(A->drawVerts[j].x,A->drawVerts[j].y,A->drawVerts[j].z);
    }
    glEnd();
    glPopMatrix();
}

// ------------------ INIT ANIMALS ------------------
void initAnimals(){
    animals[0].modelFile="Models/Horse/Horse.obj"; animals[0].textureFile="Models/Horse/Horse.jpeg";
    animals[1].modelFile="Models/Cow/Cow.obj";     animals[1].textureFile="Models/Cow/Cow.jpeg";
    animals[2].modelFile="Models/Sheep/Sheep.obj"; animals[2].textureFile="Models/Sheep/Sheep.jpeg";
    animals[3].modelFile="Models/Camel/Camel.obj"; animals[3].textureFile="Models/Camel/Camel.jpeg";
    animals[4].modelFile="Models/Goat/Goat.obj";   animals[4].textureFile="Models/Goat/Goat.jpeg";

    float startX=-20.0f,spacing=15.0f;
    for(int i=0;i<totalAnimals;i++){
        loadOBJ(&animals[i]);
        animals[i].textureID=loadTexture(animals[i].textureFile);
        animals[i].xPos=startX + i*spacing;
        animals[i].zPos=50;
    }
}

// ------------------ CAMERA UPDATE ------------------
void updateCameraDir(){
    float radYaw=cameraYaw*M_PI/180.0f;
    float radPitch=cameraPitch*M_PI/180.0f;
    dirX=cos(radYaw)*cos(radPitch);
    dirY=sin(radPitch);
    dirZ=sin(radYaw)*cos(radPitch);
}

// ------------------ GROUND ------------------
void drawGround(){
    float size=500.0f;
    glColor3f(0.3f,0.7f,0.3f);
    glBegin(GL_QUADS);
        glVertex3f(-size,0,-size);
        glVertex3f(size,0,-size);
        glVertex3f(size,0,size);
        glVertex3f(-size,0,size);
    glEnd();
}

// ------------------ DISPLAY ------------------
// ------------------ DISPLAY ------------------
void display(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // --- CAMERA ---
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0,1280.0/720.0,0.1,5000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camX,camY,camZ, camX+dirX,camY+dirY,camZ+dirZ, 0,1,0);

    glDisable(GL_LIGHTING);

    // --- SKYBOX ---
    float size=100.0f;
    glEnable(GL_TEXTURE_2D); glColor3f(1,1,1); glDepthMask(GL_FALSE); glDisable(GL_CULL_FACE);

    GLuint skyIndices[6]={4,5,0,1,2,3};
    float coords[6][4][3]={
        {{-size,-size,size},{size,-size,size},{size,size,size},{-size,size,size}},
        {{size,-size,-size},{-size,-size,-size},{-size,size,-size},{size,size,-size}},
        {{size,-size,size},{size,-size,-size},{size,size,-size},{size,size,size}},
        {{-size,-size,-size},{-size,-size,size},{-size,size,size},{-size,size,-size}},
        {{-size,size,size},{size,size,size},{size,size,-size},{-size,size,-size}},
        {{-size,-size,-size},{size,-size,-size},{size,-size,size},{-size,-size,size}}
    };
    for(int i=0;i<6;i++){
        glBindTexture(GL_TEXTURE_2D,textures[skyIndices[i]]);
        glBegin(GL_QUADS);
        for(int j=0;j<4;j++) glTexCoord2f((j==1||j==2),(j>=2)), glVertex3f(coords[i][j][0],coords[i][j][1],coords[i][j][2]);
        glEnd();
    }
    glDepthMask(GL_TRUE);

    // --- SHADOWS ---
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0,0,0,0.5f); // shadow color
    for(int i=0;i<totalAnimals;i++) drawShadow(&animals[i], i);
    glDisable(GL_BLEND);

    // Reset color and textures for animals
    glColor3f(1.0f,1.0f,1.0f);
    glEnable(GL_TEXTURE_2D);

    // --- ANIMALS ---
    for(int i=0;i<totalAnimals;i++) drawAnimal(&animals[i],i);

    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
}


// ------------------ INPUT ------------------
void reshape(int w,int h){ glViewport(0,0,w,h); }
void keyboard(unsigned char key,int x,int y){
    float rightX=-dirZ,rightZ=dirX;
    switch(key){
        case 27: exit(0); break;
        case 'w': camX+=dirX*camSpeed; camZ+=dirZ*camSpeed; break;
        case 's': camX-=dirX*camSpeed; camZ-=dirZ*camSpeed; break;
        case 'a': camX+=rightX*camSpeed; camZ+=rightZ*camSpeed; break;
        case 'd': camX-=rightX*camSpeed; camZ-=rightZ*camSpeed; break;

        // Example keys to play animal sounds
        case '1': AudioServicesPlaySystemSound(animalSounds[0]); break; // Horse
        case '2': AudioServicesPlaySystemSound(animalSounds[1]); break; // Cow
        case '3': AudioServicesPlaySystemSound(animalSounds[2]); break; // Sheep
        case '4': AudioServicesPlaySystemSound(animalSounds[3]); break; // Camel
        case '5': AudioServicesPlaySystemSound(animalSounds[4]); break; // Goat
    }
    glutPostRedisplay();
}



void specialKeys(int key,int x,int y){
    float rightX = -dirZ;
    float rightZ = dirX;
    switch(key){
        case GLUT_KEY_LEFT:  camX -= rightX * camSpeed; camZ -= rightZ * camSpeed; break;  // strafe left
        case GLUT_KEY_RIGHT: camX += rightX * camSpeed; camZ += rightZ * camSpeed; break;  // strafe right
        case GLUT_KEY_UP:    camX += dirX * camSpeed;  camZ += dirZ * camSpeed; break;     // forward
        case GLUT_KEY_DOWN:  camX -= dirX * camSpeed;  camZ -= dirZ * camSpeed; break;     // backward
    }
    glutPostRedisplay();
}
void passiveMotion(int x,int y){
    if(firstMouse){ lastX=x; lastY=y; firstMouse=0; return; }
    float xoff=x-lastX; float yoff=lastY-y; lastX=x; lastY=y;
    float sens=0.1f; xoff*=sens; yoff*=sens;
    cameraYaw+=xoff; cameraPitch+=yoff;
    if(cameraPitch>89) cameraPitch=89; if(cameraPitch<-89) cameraPitch=-89;
    updateCameraDir();
    glutPostRedisplay();
}
void idle(){ glutPostRedisplay(); }

// ------------------ MAIN ------------------
int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280,720);
    glutCreateWindow("Skybox + Animals + Shadows");

    glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LEQUAL);
    glClearColor(0.1f,0.1f,0.1f,1.0f);
    stbi_set_flip_vertically_on_load(0);

    const char* faces[6]={"Images/pz.png","Images/nz.png","Images/py.png","Images/ny.png","Images/px.png","Images/nx.png"};
    for(int i=0;i<6;i++) textures[i]=loadTexture(faces[i]);

    initAnimals();
    initSounds();  
    updateCameraDir();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutPassiveMotionFunc(passiveMotion);
    glutIdleFunc(idle);
    glutSetCursor(GLUT_CURSOR_NONE);

    glutMainLoop();
    return 0;
}
