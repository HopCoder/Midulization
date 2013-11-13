

all:
	g++ -O3 -Wall -I~/Downloads/rtmidi-2.0.1 -I~/Downloads/rtmidi-2.0.1/include -D__LINUX_ALSA__ -o main project.cpp ~/Downloads/rtmidi-2.0.1/tests/Release/RtMidi.o -lpthread -lasound -lGL -lglut 



