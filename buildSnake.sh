#!/bin/bash

cc snake.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o snake
cc snake.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o snake.exe
./snake