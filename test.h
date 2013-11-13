#ifndef __HOPPER_MIDI_
#define __HOPPER_MIDI_

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "RtMidi.h"

std::vector<unsigned char> midi_array_passer(95, 0);

class read_midi{
public:
    read_midi();
    read_midi(unsigned int i);

    ~read_midi();

    void init();
    bool init(unsigned int i);
    bool config(unsigned int i);

    std::vector<unsigned char> get_array(){arr = midi_array_passer; return arr;}

protected: 
    //void update(double deltatime, std::vector <unsigned char> *message, void *userData);

private:
    std::vector<unsigned char> arr;
    
    RtMidiIn *midiin;
    

};

read_midi::read_midi() : midiin(0), arr(95, 0){

}

read_midi::read_midi(unsigned int i) : midiin(0), arr(95, 0){
    read_midi();
    init(i);
}

void read_midi::init(){
    midiin = new RtMidiIn();
    std::string portName;
    unsigned int i = 0, nPorts = midiin->getPortCount();
    if(nPorts == 0) {
        std::cout << "No input ports available!" << std::endl;
        return;
    }

    if(nPorts == 1){
        std::cout << "\nOpening " << midiin->getPortName() << std::endl;
    }
    else {
        for(i=0; i < nPorts; i++){
            portName = midiin->getPortName(i);
            std::cout << "  Input port #" << i << ": " << portName << '\n';
        }

        do{
            std::cout << "\nChoose a port number: ";
            std::cin >> i;
        } while (i >= nPorts);
    }
    
    std::string keyHit;
    std::getline(std::cin, keyHit);
    config(i);
}

/*void read_midi::update(double deltatime, std::vector<unsigned char> *message, void *userData){
    unsigned int nBytes = message->size();
    for(unsigned int i = 0; i < nBytes; i++){
        std::cout << "Byte " << i << " = " << (int)message->at(i) << ",  ";
    }
    if(nBytes > 0)
        std::cout << "stamp = " << deltatime << std::endl;
}*/

bool read_midi::init(unsigned int i){
    midiin = new RtMidiIn();
    return config(i);
}

void update_midi_array(double deltatime, std::vector<unsigned char> *message, void *userData){
    if(message->size() >= 3){
        //if((int)message->at(0) == 145)
        if((int)message->at(0) == 144)
            midi_array_passer[(int)message->at(1)] = (unsigned char)message->at(2);
        //if((int)message->at(0) == 129)
        if((int)message->at(0) == 128)
            midi_array_passer[(int)message->at(1)] = 0;
    }

    for(int i = 24; i < midi_array_passer.size(); i++){
        std::cout << (int)midi_array_passer[i] << ' ';
    }
    std::cout << std::endl;
    
}

bool read_midi::config(unsigned int i){
    midiin->openPort(i);
    midiin->setCallback( &update_midi_array );
    midiin->ignoreTypes(true, true, true);
    return true;
}

read_midi::~read_midi(){
    delete midiin;
}


#endif

