#!/bin/bash

g++ -o main main.cpp Bunny.cpp ShaderSource.cpp CompiledShader.cpp AttachedShader.cpp -lm -lGL -lGLEW -lglut
