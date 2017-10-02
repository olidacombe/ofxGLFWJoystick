#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(22);
	ofxGLFWJoystick::one().printJoystickList();

        if(!midiOut.openVirtualPort("ps2Midi")) {
          ofLogError("ps2Midi") << "Unable to open virtual Midi port";
        }

}

//--------------------------------------------------------------
void ofApp::update(){

    if(true) { // check if it's time yet
      ofxGLFWJoystick::one().update();
      auto changes = ofxGLFWJoystick::one().getChangedValues(0);

      sendMessages(changes);
    }

}

void ofApp::sendMessages(ofxGLFWJoystick::diff& changes, bool debug) {
  if(debug) {
    for(auto& kv : changes.buttons) ofLogNotice("ps2Midi") << "button " << kv.first << " - " << static_cast<int>(kv.second);
    for(auto& kv : changes.axes) ofLogNotice("ps2Midi") << "axis " << kv.first << " - " << kv.second;
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

}
