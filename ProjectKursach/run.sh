#!/bin/sh

g++ Mario2.cpp -o Mario.out -ISFML/include/ -LSFML/lib/  -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGLEW && ./Mario.out
