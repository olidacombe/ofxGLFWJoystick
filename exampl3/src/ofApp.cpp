#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(22);
	ofxGLFWJoystick::one().printJoystickList();

        if(!midiOut.openVirtualPort("ps2Midi")) {
          ofLogError("ps2Midi") << "Unable to open virtual Midi port";
        }

        gui.setup("params");
        gui.add(showGui.set("showGui", false));
        gui.add(debug.set("debug", false));
        gui.add(midiCallbackTimeMillis.set("midiTime", 30, 3, 500));
        gui.loadFromFile("settings.xml");

}

//--------------------------------------------------------------
void ofApp::update(){

    int time = ofGetElapsedTimeMillis();

    if(time > midiCallbackTimeMillis) { // check if it's time yet

      ofxGLFWJoystick::one().update();
      numButtons = ofxGLFWJoystick::one().getNumButtons(0);

      auto changes = ofxGLFWJoystick::one().getChangedValues(0);

      sendMessages(changes, debug);
      ofResetElapsedTimeCounter();
    }

}

void ofApp::sendMessages(ofxGLFWJoystick::diff& changes, bool debug) {
  const int baseCC = 0;

  if(debug) {
    for(auto& kv : changes.buttons) ofLogNotice("ps2Midi") << "button " << kv.first << " - " << static_cast<int>(kv.second);
    for(auto& kv : changes.axes) ofLogNotice("ps2Midi") << "axis " << kv.first << " - " << kv.second;
  }

  for(auto& kv : changes.buttons) {
    midiOut.sendControlChange(midiChannel, baseCC + kv.first, kv.second);
  }

  for(auto& kv : changes.axes) {
    midiOut.sendControlChange(midiChannel, baseCC + numButtons + kv.first, ofMap(kv.second, -1., 1., 0, 127, true));
  }

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofxGLFWJoystick::one().drawDebug(100,100);

	//safe access o joystick - slower
	int joystickID = 0;
	float joyX = ofxGLFWJoystick::one().getAxisValue(0, joystickID);
	float joyY = ofxGLFWJoystick::one().getAxisValue(1, joystickID);
	//getAxisValue() always returns a value in the [-1, 1] range


	//lets map the joystick to our window size
	float mappedX = ofMap(joyX, -1, 1, 0, ofGetWidth());
	float mappedY = ofMap(joyY, -1, 1, 0, ofGetHeight());

	ofSetColor(255);
	ofCircle(mappedX, mappedY, 3);

        if(showGui) gui.draw();

}

void ofApp::keyPressed(int key){
  switch(key) {
    case 'h':
      showGui = !showGui;
      break;
    case 'd':
      debug = !debug;
      break;
    default:
      break;
  }
}


void ofApp::exit() {
  midiOut.closePort();
  gui.saveToFile("settings.xml");
}
