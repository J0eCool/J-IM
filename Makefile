buildDir = build

all: | $(buildDir)
	g++ src/main.cpp -o $(buildDir)/jil.exe

clean:
	rmdir -rf $(buildDir)

$(buildDir):
	mkdir $(buildDir)