#!/usr/bin/bash

#src/*.h

gcc -Wall -Wextra -I/lib/raylib -o main src/*.c  -L/lib/raylib -lraylib -lm

