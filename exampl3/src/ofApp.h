#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxGLFWJoystick.h"

class ofApp : public ofBaseApp{
public:

  void setup();
  void update();
  void draw();

  void sendMessages(ofxGLFWJoystick::diff& changes, bool debug=false);

private:
  ofxMidiOut midiOut;
  const int midiChannel = 8;

};
