

all: library
	
build:
	g++ -O3 -Wall -Irtmidi-2.0.1 -Irtmidi-2.0.1/include -D__LINUX_ALSA__ -o main project.cpp rtmidi-2.0.1/tests/Release/RtMidi.o -lpthread -lasound -lGL -lglut -lGLU

build_mac:
	g++ -O3 -Wall -Irtmidi-2.0.1 -Irtmidi-2.0.1/include -D__LINUX_ALSA__ -o main project.cpp rtmidi-2.0.1/tests/Release/RtMidi.o -lpthread -framework CoreMIDI -framework CoreAudio -framework CoreFoundation -framework GL -framework glut 

library:
	./make_library.sh

