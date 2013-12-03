#ifndef __HOPPER_MIDI_
#define __HOPPER_MIDI_

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "RtMidi.h"

//This will be a global variable that will keep track of the keys pressed
//and thier velocities
std::vector<unsigned char> midi_array_passer(97, 0);


/*
 * THis class will serve as a wrapper for the RtMidi library, to grab
 * the keys which are currently pressed and their velocities
 */

class read_midi{
public:
    //constructors
    read_midi();
    read_midi(unsigned int i);

    //destructor
    ~read_midi();

    //initialization functions
    void init();
    bool init(unsigned int i);
    bool config(unsigned int i);

    //function to return a vector containing a vector of keys which are on, and
    //their velocities
    std::vector<std::vector<unsigned char> > get_on_keys();

    //function to return the array containing all of the keys currently on
    std::vector<unsigned char> get_array(){arr = midi_array_passer; return arr;}

private:
    //vector containing the velocities of keys located at the index
    std::vector<unsigned char> arr;
    
    //RtMidiIn object
    RtMidiIn *midiin;
    

};

//constructors
read_midi::read_midi() : midiin(0), arr(95, 0){

}

read_midi::read_midi(unsigned int i) : midiin(0), arr(95, 0){
    read_midi();
    init(i);
}

//initialization where ports are printed out to choose from
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

//initialization when we know the port already
bool read_midi::init(unsigned int i){
    midiin = new RtMidiIn();
    return config(i);
}

//returns the on keys
std::vector<std::vector<unsigned char> > read_midi::get_on_keys(){
    std::vector<std::vector<unsigned char> > returner;
    std::vector<unsigned char> adder;
    for(unsigned int i = 0; i < midi_array_passer.size(); i++){
        if(midi_array_passer[i] != 0){
            adder.clear();
            adder.push_back(i);
            adder.push_back(midi_array_passer[i]);
            returner.push_back(adder);
        }
    }
    return returner;
}

//update the data that we have, this will be a callback function that we give
//the midi object, which will be called every time we get a MIDI event
void update_midi_array(double deltatime, std::vector<unsigned char> *message, void *userData){
    if(message->size() >= 3){
        if((int)message->at(0) == 144)
            midi_array_passer[(int)message->at(1)] = (unsigned char)message->at(2);
        if((int)message->at(0) == 128)
            midi_array_passer[(int)message->at(1)] = 0;
    }
}

//set all the configurations for the RtiMidi object
bool read_midi::config(unsigned int i){
    midiin->openPort(i);
    midiin->setCallback( &update_midi_array );
    midiin->ignoreTypes(true, true, true);
    return true;
}

//destructor; don't like memory leaks
read_midi::~read_midi(){
    delete midiin;
}


#endif

