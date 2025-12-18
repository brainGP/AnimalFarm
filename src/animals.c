#define GL_SILENCE_DEPRECATION 1
#include "animals.h"

// ---------- GLOBALS ----------
int totalAnimals = 5;
Animal animals[5];
float realSizeScale[5] = {1.0f, 0.9f, 0.65f, 1.2f, 0.6f};

// camera
float camX = 0, camZ = -50.0f;
float camAngle = 180.0f;
float camHeight = 8.0f;
float moveSpeed = 2.0f, rotSpeed = 10.0f;

// ---------- AUDIO ----------
void playSound(char* file) {
    SystemSoundID id;
    CFURLRef url = CFBundleCopyResourceURL(CFBundleGetMainBundle(),
                                           CFStringCreateWithCString(NULL, file, kCFStringEncodingUTF8),
                                           CFSTR("wav"), NULL);
    if (!url) return;
    AudioServicesCreateSystemSoundID(url, &id);
    AudioServicesPlaySystemSound(id);
}

// ---------- TEXTURE ----------
GLuint loadTexture(const char* file) {
    int w, h, n;
    unsigned char* data = stbi_load(file, &w, &h, &n, 3);
    if (!data) { printf("Texture FAILED: %s\n", file); return 0; }
    GLuint tex; glGenTextures(1, &tex); glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
    return tex;
}

// ---------- LOAD OBJ ----------
void loadOBJ(Animal* A) {
    FILE* f = fopen(A->modelFile, "r");
    if (!f) { printf("OBJ not found: %s\n", A->modelFile); return; }
    char line[512];
    A->posCount = A->uvCount = A->faceCount = 0;
    while (fgets(line, sizeof(line), f)) {
        if (!strncmp(line, "v ", 2)) A->posCount++;
        else if (!strncmp(line, "vt ", 3)) A->uvCount++;
        else if (!strncmp(line, "f ", 2)) A->faceCount++;
    }
    rewind(f);
    A->posList = malloc(sizeof(VPos) * A->posCount);
    A->uvList = malloc(sizeof(VUV) * A->uvCount);
    A->faces = malloc(sizeof(Face) * A->faceCount);

    int pi = 0, ui = 0, fi = 0;
    float minX = 99999, minY = 99999, minZ = 99999;
    float maxX = -99999, maxY = -99999, maxZ = -99999;

    while (fgets(line, sizeof(line), f)) {
        if (!strncmp(line, "v ", 2)) {
            float x, y, z; sscanf(line, "v %f %f %f", &x, &y, &z);
            A->posList[pi].x = x; A->posList[pi].y = y; A->posList[pi].z = z;
            if (x < minX) minX = x; if (y < minY) minY = y; if (z < minZ) minZ = z;
            if (x > maxX) maxX = x; if (y > maxY) maxY = y; if (z > maxZ) maxZ = z;
            pi++;
        } else if (!strncmp(line, "vt ", 3)) {
            float u, v; sscanf(line, "vt %f %f", &u, &v);
            A->uvList[ui].u = u; A->uvList[ui].v = 1.0f - v;
            ui++;
        } else if (!strncmp(line, "f ", 2)) {
            int v[3], t[3]; sscanf(line, "f %d/%d %d/%d %d/%d", &v[0], &t[0], &v[1], &t[1], &v[2], &t[2]);
            for (int k = 0; k < 3; k++) { A->faces[fi].v[k] = v[k] - 1; A->faces[fi].uv[k] = t[k] - 1; }
            fi++;
        }
    }
    fclose(f);

    A->drawCount = A->faceCount * 3;
    A->drawVerts = malloc(sizeof(DrawVertex) * A->drawCount);
    int di = 0;
    for (int i = 0; i < A->faceCount; i++) {
        for (int k = 0; k < 3; k++) {
            int vi = A->faces[i].v[k]; int ti = A->faces[i].uv[k];
            A->drawVerts[di].x = A->posList[vi].x;
            A->drawVerts[di].y = A->posList[vi].y;
            A->drawVerts[di].z = A->posList[vi].z;
            A->drawVerts[di].u = A->uvList[ti].u;
            A->drawVerts[di].v = A->uvList[ti].v;
            di++;
        }
    }

    A->centerX = (minX + maxX) / 2; A->centerY = (minY + maxY) / 2; A->centerZ = (minZ + maxZ) / 2;
    float maxDim = fmax(fmax(maxX - minX, maxY - minY), maxZ - minZ);
    A->scaleMul = 25.0f / maxDim;
    A->heightOffset = -minY * A->scaleMul;
    A->minY = minY; A->maxY = maxY;
}

// ---------- DRAW ANIMAL ----------
void drawAnimal(Animal* A, int index) {
    glPushMatrix();
    glTranslatef(A->xPos, 0, A->zPos); // move X/Z
    glRotatef(90.0f, 0, 1, 0); // rotate Y
    glScalef(A->scaleMul * realSizeScale[index],
             A->scaleMul * realSizeScale[index],
             A->scaleMul * realSizeScale[index]);
    glTranslatef(-A->centerX, -A->minY, -A->centerZ); // center
    glBindTexture(GL_TEXTURE_2D, A->textureID);
    glBegin(GL_TRIANGLES);
    for (int j = 0; j < A->drawCount; j++) {
        glTexCoord2f(A->drawVerts[j].u, A->drawVerts[j].v);
        glVertex3f(A->drawVerts[j].x, A->drawVerts[j].y, A->drawVerts[j].z);
    }
    glEnd();
    glPopMatrix();
}

// ---------- INIT ANIMALS ----------
void initAnimals() {
    animals[0].modelFile = "Models/Horse/Horse.obj"; animals[0].textureFile = "Models/Horse/Horse.jpeg";
    animals[1].modelFile = "Models/Cow/Cow.obj";     animals[1].textureFile = "Models/Cow/Cow.jpeg";
    animals[2].modelFile = "Models/Sheep/Sheep.obj"; animals[2].textureFile = "Models/Sheep/Sheep.jpeg";
    animals[3].modelFile = "Models/Camel/Camel.obj"; animals[3].textureFile = "Models/Camel/Camel.jpeg";
    animals[4].modelFile = "Models/Goat/Goat.obj";   animals[4].textureFile = "Models/Goat/Goat.jpeg";

    float startX = -20.0f;
    float spacing = 10.0f;

    for (int i = 0; i < totalAnimals; i++) {
        loadOBJ(&animals[i]);
        animals[i].textureID = loadTexture(animals[i].textureFile);
        animals[i].xPos = startX + i * spacing;
        animals[i].zPos = 0;
    }
}
