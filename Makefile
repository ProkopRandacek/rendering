GCC_WFLAGS = -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code -Wformat=2 -Winit-self -Wuninitialized
GCC_FLAGS = $(GCC_WFLAGS) -std=c11 -Ofast
GCC_LIB = -L/usr/lib/libGLEW.so -lglfw -lGL -lm -lGLEW

build: clean shaders
	mkdir build -p
	gcc src/*.c -o build/rendering $(GCC_FLAGS) $(GCC_LIB)

run: clean build
	cd build/; ./rendering

shaders: clean
	cat $(wildcard src/shaders/*frag.glsl) > fragFull.glsl
	cat $(wildcard src/shaders/*vert.glsl) > vertFull.glsl
	mono shader_minifier.exe fragFull.glsl --preserve-externals -o src/frag.h
	mono shader_minifier.exe vertFull.glsl --preserve-externals -o src/vert.h
	rm fragFull.glsl vertFull.glsl -f

clean:
	rm build/ -rf
	rm fragFull.glsl vertFull.glsl -f
