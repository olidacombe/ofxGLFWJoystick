#include "ofApp.h"
#include "ofxGLFWJoystick.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(22);
	ofxGLFWJoystick::one().printJoystickList();

}

//--------------------------------------------------------------
void ofApp::update(){

	ofxGLFWJoystick::one().update();
    
    auto changes = ofxGLFWJoystick::one().getChangedValues(0);
    
    /*
    for(auto& kv : changes.buttons) {
        ofLogNotice("exampl3") << "button " << kv.first << " = " << static_cast<int>(kv.second);
    }
    for(auto& kv : changes.axes) {
        ofLogNotice("exampl3") << "axis " << kv.first << " = " << kv.second;
    }
    */
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