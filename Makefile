build:
	mkdir build
	cp src/glsl/* build/
	gcc -L/usr/lib/libGLEW.so -Wall -o build/rendering src/*.c -lglfw -lGL -lm -lGLEW

run: build
	cd build/; ./rendering
	clear

clean:
	rm build/ -rf
