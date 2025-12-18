#include <GLUT/glut.h>
#include "stb_image.h"
#include "../include/skybox.h"
#include <stdio.h>

static GLuint tex[6];
static float yaw=0, pitch=0;
static int lastX=640,lastY=360,firstMouse=1;

static GLuint loadTex(const char* f){
    int w,h,n;
    unsigned char* d=stbi_load(f,&w,&h,&n,0);
    if(!d){ printf("Skybox fail: %s\n",f); return 0; }
    GLuint t; glGenTextures(1,&t); glBindTexture(GL_TEXTURE_2D,t);
    GLenum fmt=(n==4)?GL_RGBA:GL_RGB;
    glTexImage2D(GL_TEXTURE_2D,0,fmt,w,h,0,fmt,GL_UNSIGNED_BYTE,d);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    stbi_image_free(d);
    return t;
}

void skyboxInit(){
    const char* f[6]={
        "Images/px.png","Images/nx.png",
        "Images/py.png","Images/ny.png",
        "Images/pz.png","Images/nz.png"
    };
    for(int i=0;i<6;i++) tex[i]=loadTex(f[i]);
}

void skyboxDraw(){
    float s=200.0f;
    glDisable(GL_LIGHTING);
    glDepthMask(GL_FALSE);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glRotatef(-pitch,1,0,0);
    glRotatef(-yaw,0,1,0);

    // +Z
    glBindTexture(GL_TEXTURE_2D,tex[4]);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0); glVertex3f(-s,-s,s);
    glTexCoord2f(1,0); glVertex3f(s,-s,s);
    glTexCoord2f(1,1); glVertex3f(s,s,s);
    glTexCoord2f(0,1); glVertex3f(-s,s,s);
    glEnd();

    // -Z
    glBindTexture(GL_TEXTURE_2D,tex[5]);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0); glVertex3f(s,-s,-s);
    glTexCoord2f(1,0); glVertex3f(-s,-s,-s);
    glTexCoord2f(1,1); glVertex3f(-s,s,-s);
    glTexCoord2f(0,1); glVertex3f(s,s,-s);
    glEnd();

    glPopMatrix();
    glDepthMask(GL_TRUE);
    glDisable(GL_TEXTURE_2D);
}

void skyboxSpecial(int key,int x,int y){
    if(key==GLUT_KEY_LEFT) yaw-=2;
    if(key==GLUT_KEY_RIGHT) yaw+=2;
    if(key==GLUT_KEY_UP) pitch+=2;
    if(key==GLUT_KEY_DOWN) pitch-=2;
}

void skyboxMouse(int x,int y){
    if(firstMouse){ lastX=x; lastY=y; firstMouse=0; return; }
    yaw+=(x-lastX)*0.1f;
    pitch+=(lastY-y)*0.1f;
    lastX=x; lastY=y;
}
