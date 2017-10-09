#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxGLFWJoystick.h"
#include "ofxGui.h"
#include "AxisSmoother.h"

class ofApp : public ofBaseApp{
public:

  void setup();
  void update();
  void draw();

  void keyPressed(int key);
  void sendMessages(ofxGLFWJoystick::diff& changes, bool debug=false);

  void exit();

private:
  ofxMidiOut midiOut;
    const int midiChannel = 8;

  int numButtons = 0;
//    int numAxes = 0;

  ofxPanel gui;
  ofParameter<bool> showGui;
  ofParameter<bool> debug;
  ofParameter<int> midiCallbackTimeMillis;
  ofParameter<float> axisSmooth;
    

};
