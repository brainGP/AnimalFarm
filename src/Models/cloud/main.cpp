#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

/* ================= FULLSCREEN QUAD ================= */
float quadVertices[] = {
    -1,-1,0,  1,-1,0,
    -1, 1,0,  1, 1,0
};

/* ================= CAMERA ================= */
float camX = 0.0f, camY = 3.0f, camZ = 30.0f;
float speed = 0.3f;

void processInput(GLFWwindow* w){
    if(glfwGetKey(w,GLFW_KEY_W)==GLFW_PRESS) camZ -= speed;
    if(glfwGetKey(w,GLFW_KEY_S)==GLFW_PRESS) camZ += speed;
    if(glfwGetKey(w,GLFW_KEY_A)==GLFW_PRESS) camX -= speed;
    if(glfwGetKey(w,GLFW_KEY_D)==GLFW_PRESS) camX += speed;
    if(glfwGetKey(w,GLFW_KEY_Q)==GLFW_PRESS) camY -= speed;
    if(glfwGetKey(w,GLFW_KEY_E)==GLFW_PRESS) camY += speed;
}

/* ================= SHADER COMPILE ================= */
GLuint compileShader(GLenum type,const char* src){
    GLuint s=glCreateShader(type);
    glShaderSource(s,1,&src,nullptr);
    glCompileShader(s);
    return s;
}

/* ================= VERTEX ================= */
const char* vertexShader = R"glsl(
#version 330 core
layout(location=0) in vec3 aPos;
out vec2 uv;
void main(){
    uv = aPos.xy*0.5+0.5;
    gl_Position = vec4(aPos,1);
}
)glsl";

/* ================= FRAGMENT ================= */
const char* fragmentShader = R"glsl(
#version 330 core
out vec4 FragColor;
in vec2 uv;

uniform float time;
uniform vec3 camPos;

/* -------- Noise -------- */
float hash(vec3 p){
    return fract(sin(dot(p,vec3(12.9,78.2,37.7)))*43758.5);
}
float noise(vec3 p){
    vec3 i=floor(p), f=fract(p);
    f=f*f*(3.-2.*f);
    return mix(
        mix(mix(hash(i),hash(i+vec3(1,0,0)),f.x),
            mix(hash(i+vec3(0,1,0)),hash(i+vec3(1,1,0)),f.x),f.y),
        mix(mix(hash(i+vec3(0,0,1)),hash(i+vec3(1,0,1)),f.x),
            mix(hash(i+vec3(0,1,1)),hash(i+vec3(1,1,1)),f.x),f.y),
        f.z);
}

void main(){
    /* ===== PERSPECTIVE RAY ===== */
    vec2 ndc = uv*2.0-1.0;
    float fov = radians(60.0);
    vec3 rayDir = normalize(vec3(
        ndc.x * tan(fov*0.5),
        ndc.y * tan(fov*0.5),
        -1.0
    ));

    float density = 0.0;
    float stepSize = 0.03;

    for(float t=0.; t<1.3; t+=stepSize){
        vec3 p = camPos + rayDir * t * 45.0;

        // сунгасан, сийрэг үүл
        p.xz *= 0.08;
        p.y  *= 0.35;

        float n = noise(p + vec3(time*0.02,0,0));
        n = smoothstep(0.65,0.85,n);

        density += n * stepSize;
    }

    density = clamp(density,0.,1.);

    vec3 skyTop = vec3(0.55,0.7,0.95);
    vec3 skyBot = vec3(1.0,0.88,0.7);
    vec3 sky = mix(skyBot, skyTop, uv.y);

    vec3 cloud = vec3(0.97,0.98,0.99);
    vec3 color = mix(sky, cloud, density);

    FragColor = vec4(color,1.0);
}
)glsl";

int main(){
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(900,600,"Perspective Moving Camera Clouds",0,0);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    GLuint VAO,VBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(quadVertices),quadVertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,0,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    GLuint prog=glCreateProgram();
    glAttachShader(prog,compileShader(GL_VERTEX_SHADER,vertexShader));
    glAttachShader(prog,compileShader(GL_FRAGMENT_SHADER,fragmentShader));
    glLinkProgram(prog);

    float time=0;

    while(!glfwWindowShouldClose(window)){
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);
        time += 0.5f;

        glUseProgram(prog);
        glUniform1f(glGetUniformLocation(prog,"time"),time);
        glUniform3f(glGetUniformLocation(prog,"camPos"),camX,camY,camZ);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
