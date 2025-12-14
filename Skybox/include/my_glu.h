#ifndef MY_GLU_H
#define MY_GLU_H

#include <math.h>
#include <string.h>
#include <GL/gl.h>

static void gluPerspective(double fovY, double aspect, double zNear, double zFar) {
    double f = 1.0 / tan(fovY * 0.5 * M_PI / 180.0);
    double m[16] = {
        f/aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (zFar+zNear)/(zNear-zFar), -1,
        0, 0, (2*zFar*zNear)/(zNear-zFar), 0
    };
    glMultMatrixd(m);
}

static void gluLookAt(double ex, double ey, double ez,
                      double cx, double cy, double cz,
                      double ux, double uy, double uz) {
    double fx = cx - ex, fy = cy - ey, fz = cz - ez;
    double rl = 1.0 / sqrt(fx*fx + fy*fy + fz*fz);
    fx*=rl; fy*=rl; fz*=rl;

    double sx = fy*uz - fz*uy;
    double sy = fz*ux - fx*uz;
    double sz = fx*uy - fy*ux;

    rl = 1.0 / sqrt(sx*sx + sy*sy + sz*sz);
    sx*=rl; sy*=rl; sz*=rl;

    double ux2 = sy*fz - sz*fy;
    double uy2 = sz*fx - sx*fz;
    double uz2 = sx*fy - sy*fx;

    double m[16] = {
        sx,  ux2,  -fx,  0,
        sy,  uy2,  -fy,  0,
        sz,  uz2,  -fz,  0,
        0,    0,    0,   1
    };

    glMultMatrixd(m);
    glTranslated(-ex, -ey, -ez);
}

#endif
