build: clean shaders
	mkdir build -p
	gcc -L/usr/lib/libGLEW.so -Wall -Wextra -std=c11 -Ofast -o build/rendering src/*.c -lglfw -lGL -lm -lGLEW

run: clean build
	cd build/; ./rendering

shaders: clean
	cat $(wildcard src/glsl/*frag.glsl) > fragFull.glsl
	cat $(wildcard src/glsl/*vert.glsl) > vertFull.glsl
	mono shader_minifier.exe fragFull.glsl --preserve-externals -o src/frag.h
	mono shader_minifier.exe vertFull.glsl --preserve-externals -o src/vert.h
	rm fragFull.glsl vertFull.glsl -f

clean:
	rm build/ -rf
	rm fragFull.glsl vertFull.glsl -f
