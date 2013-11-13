

all:
	g++ -O3 -Wall -Irtmidi-2.0.1 -Irtmidi-2.0.1/include -D__LINUX_ALSA__ -o main main.cpp rtmidi-2.0.1/tests/Release/RtMidi.o -lpthread -lasound

