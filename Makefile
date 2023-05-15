.PHONY: clean build package all
.DEFAULT_GOAL=all

clean::
	rm -rfv build

build::
	g++ -o OpenGL-scene *.c *.cpp */*.cpp */*/*.cpp -static-libgcc -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive,-Bdynamic -lopengl32 -lglfw3 -I./external-dependencies/include

package:: build
	mkdir -vp build/Release
	mv -fv OpenGL-scene.exe build/Release/
	cp -av media shader build/Release/
	cp -av /mingw64/bin/glfw3.dll build/Release/

all:: package
