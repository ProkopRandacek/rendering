build: clean shaders
	mkdir build -p
	gcc -L/usr/lib/libGLEW.so -Wall -o build/rendering src/*.c -lglfw -lGL -lm -lGLEW

run: clean build
	cd build/; ./rendering

shaders: clean
	mkdir build -p
	#the url does not work
	#if test -f "shader_minifier.exe"; then curl https://github.com/laurentlb/Shader_Minifier/releases/download/1.1.6/shader_minifier.exe -o shader_minifier.exe; fi
	cat src/glsl/SDFs.glsl src/glsl/fragmentShader.glsl > fragFull.glsl
	cat src/glsl/vertexShader.glsl                      > vertFull.glsl
	mono shader_minifier.exe fragFull.glsl --preserve-externals -o src/frag.h
	mono shader_minifier.exe vertFull.glsl --preserve-externals -o src/vert.h
	rm fragFull.glsl vertFull.glsl -f
	dos2unix -q src/frag.h
	dos2unix -q src/vert.h

clean:
	rm build/ -rf
	rm fragFull.glsl vertFull.glsl -f
	rm src/frag.h src/vert.h -f
