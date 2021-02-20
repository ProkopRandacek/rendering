build: clean
	mkdir build
	cp src/glsl/* build/
	gcc -L/usr/lib/libGLEW.so -Wall -o build/rendering src/*.c -lglfw -lGL -lm -lGLEW

run: clean build
	cd build/; ./rendering

clean:
	rm build/ -rf
