#pragma once

#include "ofMain.h"
#include "ofxMidi.h"

class ofApp : public ofBaseApp{
public:

  void setup();
  void update();
  void draw();

private:
  ofxMidiOut midiOut;
  const int midiChannel = 8;
};
