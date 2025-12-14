#!/bin/bash

clang \
    src/main.c \
    -I/Users/odmaabaysgalan/Documents/OPENGLproject/include \
    -I/opt/homebrew/include \
    -L/opt/homebrew/lib \
    -framework GLUT \
    -framework OpenGL \
    -o app
