#include <GL/gl.h>
#include "my_glu.h"
#include <GL/glut.h>



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <stdbool.h>


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GLuint textures[6]; // 6 faces as separate 2D textures

// Camera
float cameraYaw = 0.0f;
float cameraPitch = 0.0f;
int lastX = 640, lastY = 360;
int firstMouse = 1;



GLuint loadTexture(const char* filename) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        stbi_image_free(data);
        printf("✓ Loaded texture: %s (%dx%d, %d channels)\n", filename, width, height, nrChannels);
        return textureID;
    } else {
        printf("✗ Failed to load texture: %s\n", filename);
        printf("  Error: %s\n", stbi_failure_reason());
        return 0;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, 1280.0 / 720.0, 0.1, 5000.0);  
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Apply camera rotation
    glRotatef(-cameraPitch, 1.0f, 0.0f, 0.0f);
    glRotatef(-cameraYaw, 0.0f, 1.0f, 0.0f);
    
    glDisable(GL_LIGHTING);

    float size = 100.0f;  
    
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);
    
    // Front face (+Z)
    glBindTexture(GL_TEXTURE_2D, textures[4]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, size);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, size);
    glEnd();
    
    // Back face (-Z)
    glBindTexture(GL_TEXTURE_2D, textures[5]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, -size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, -size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, -size);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size, -size);
    glEnd();
    
    // Right face (+X)
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, -size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, -size);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size, size);
    glEnd();
    
    // Left face (-X)
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, -size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, size);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);
    glEnd();
    
    // Top face (+Y)
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size, size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size, size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, -size);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);
    glEnd();
    
    // Bottom face (-Y)
    glBindTexture(GL_TEXTURE_2D, textures[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, -size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, -size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, -size, size);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, -size, size);
    glEnd();
    
    glDepthMask(GL_TRUE);
    glDisable(GL_TEXTURE_2D);
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ESC key
        exit(0);
    }
}

void specialKeys(int key, int x, int y) {
    float rotationSpeed = 2.0f;
    
    switch(key) {
        case GLUT_KEY_LEFT:
            cameraYaw -= rotationSpeed;
            break;
        case GLUT_KEY_RIGHT:
            cameraYaw += rotationSpeed;
            break;
        case GLUT_KEY_UP:
            cameraPitch += rotationSpeed;
            if (cameraPitch > 89.0f) cameraPitch = 89.0f;
            break;
        case GLUT_KEY_DOWN:
            cameraPitch -= rotationSpeed;
            if (cameraPitch < -89.0f) cameraPitch = -89.0f;
            break;
    }
    
    glutPostRedisplay();
}

void passiveMotion(int x, int y) {
    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = 0;
        return;
    }
    
    float xoffset = x - lastX;
    float yoffset = lastY - y;
    lastX = x;
    lastY = y;
    
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    cameraYaw += xoffset;
    cameraPitch += yoffset;
    
    if (cameraPitch > 89.0f) cameraPitch = 89.0f;
    if (cameraPitch < -89.0f) cameraPitch = -89.0f;
    
    glutPostRedisplay();
}

void idle() {
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    // Print current working directory
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    }
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("Skybox ");
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    stbi_set_flip_vertically_on_load(0);

    // Load textures for each face
    printf("Loading skybox textures...\n");
    const char* faces[6] = {
        "pz.png", //+x 
        "nz.png",  //-z
        "py.png",   //+y 
        "ny.png",  //-y 
        "px.png",  //+x
        "nx.png"   //-x
    };

    
    int allLoaded = 1;
    for (int i = 0; i < 6; i++) {
        textures[i] = loadTexture(faces[i]);
        if (textures[i] == 0) {
            allLoaded = 0;
        }
    }
    
    if (!allLoaded) {
        printf(" textures failed to load!\n");
    } else {
        printf("All textures loaded successfully!\n");
    }
    
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