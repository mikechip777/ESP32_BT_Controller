#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>

// Initialize all hardware outputs
void controlInit();

// Called every loop to apply RC state to hardware
void controlUpdate();

// Called when an EventPacket is received
void controlHandleEvent(byte eventId);

#endif