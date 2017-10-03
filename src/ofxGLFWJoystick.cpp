//
//  ofxGLFWJoystick.cpp
//  emptyExample
//
//  Created by Oriol Ferrer Mesià on 07/02/15.
//
//

#include "ofxGLFWJoystick.h"

ofxGLFWJoystick::ofxGLFWJoystick(){

	numJoysticks = 0;
	printJoystickList();
	lookForJoysticks();
	//this triggers a deadlock on OSX... sad! we'll need to manually update this...
	//ofAddListener(ofEvents().update, &one(), &ofxGLFWJoystick::update, OF_EVENT_ORDER_BEFORE_APP);
}


void ofxGLFWJoystick::printJoystickList(){

	map<int,string> list = getJoystickList();
	if(list.size() == 0){
		ofLogNotice("ofxGLFWJoystick") << "No Joysticks Found!";
	}
	map<int,string>::iterator it = list.begin();
	while(it != list.end()){
		ofLogNotice("ofxGLFWJoystick") << it->second << " >> ID " << it->first;
		++it;
	}
}


map<int,string> ofxGLFWJoystick::getJoystickList(){

	map<int,string> list;
	for(int i = 0; i < GLFW_JOYSTICK_LAST; i++){
		if(glfwJoystickPresent(i)){
			list[i] = string(glfwGetJoystickName(i));
		}
	}
	return list;
}


void ofxGLFWJoystick::lookForJoysticks(){

	int n = 0;
	for(int i = 0; i < GLFW_JOYSTICK_LAST; i++){
        JoyData& jd = joyData[i];
		if(glfwJoystickPresent(i)){
			jd.available = true;
            jd.ID = i;
			string name = string(glfwGetJoystickName(i));
			if(name != jd.name){
				jd.name = name;
				ofLogNotice("ofxGLFWJoystick") << "Joystick Found at index " << i << ": '" << name << "'";
                jd.update();
				//jd.axisData = glfwGetJoystickAxes(i, &jd.numAxis);
				//jd.buttonData = glfwGetJoystickButtons(i, &jd.numButtons);
			}
			n++;
		}else{
			if (jd.name.size()){
				ofLogNotice("ofxGLFWJoystick") << "Joystick Lost at index " << i << ": '" << jd.name << "'";
			}
			jd.available = false;
			jd.name = "";
		}
	}
	numJoysticks = n;
}


void ofxGLFWJoystick::update(){

	if(ofGetFrameNum()%600 == 1){ //update joystick info every now and then
		lookForJoysticks();
	}

	//vector<string> joys;

//        std::swap(joyData, prevJoyData);

	for(int j = 0; j < numJoysticks; j++) {
        JoyData& jd = joyData[j];
		if(!jd.available){
            jd.clear();
		} else {
            // joyData should have own identity and own update funciton really
            /*
            joyData[j].shift();
			joyData[j].buttonData = glfwGetJoystickButtons(j, &joyData[j].numButtons);
			joyData[j].axisData = glfwGetJoystickAxes(j, &joyData[j].numAxis);
            joyData[j].updateAggregates();
            */
            jd.shift();
            jd.update();
		}
	}
}


float ofxGLFWJoystick::getAxisValue(int axisID, int joyID){

	if(isJoystickAvailable(joyID)){
		if(axisID < joyData[joyID].numAxis && axisID >= 0){
			return joyData[joyID].axisData[axisID];
		}
	}
	return 0.0f;
}

const ofxGLFWJoystick::diff ofxGLFWJoystick::getChangedValues(int joyID) {
  return joyData[joyID].changes();
}

unsigned char ofxGLFWJoystick::getButtonValue(int buttonID, int joyID){

	if(isJoystickAvailable(joyID)){
		if(buttonID < joyData[joyID].numButtons && buttonID >= 0){
			return joyData[joyID].buttonData[buttonID];
		}
	}
	return 0;
}


void ofxGLFWJoystick::drawDebug(int x, int y){

	ofPushMatrix();
	ofPushStyle();
	ofTranslate(x, y);
	int joyRad = 25;
	int buttonSize = 20;
	int padding = 10;
	float yOffset = 0;

	for(int j = 0; j < numJoysticks; j++){


		ofTranslate(0 , yOffset);

		int xOffset = joyRad * 0.5f;
		bool joyOK = glfwJoystickPresent(j);

		if(joyOK){
			string msg = "'" + joyData[j].name + "' numAxis: " + ofToString(joyData[j].numAxis) +
			"  numButtons: " + ofToString(joyData[j].numButtons);

			ofSetColor(255);
			ofDrawBitmapString(msg, 0, - 22);

			for(int i = 0; i < joyData[j].numAxis; i+=2){
				ofSetColor(255, 128);
				float xx = xOffset + joyRad * 0.5f;
				float yy = joyRad * 0.5f;
				ofCircle(xx, yy, joyRad);
				float x = joyData[j].axisData[i] * joyRad;
				float y = 0;
				if(i + 1 < joyData[j].numAxis ){
					y = joyData[j].axisData[i + 1] * joyRad;
				}
				ofColor c; c.setHsb((i * 16)%255, 255, 255);
				ofSetColor(c);
                ofDrawArrow(glm::vec3(xx, yy, 0), glm::vec3(xx + x, yy + y, 0), 4);
				xOffset += joyRad * 2 + padding;
			}

			ofSetRectMode(OF_RECTMODE_CENTER);
			for(int i = 0; i < joyData[j].numButtons; i ++ ){
				bool pressed = joyData[j].buttonData[i] > 0; //mmm not sure why its a char if its only 0/1
				ofSetColor( pressed ? 0:255, pressed ? 255:0, 0 );
				ofRect(xOffset, joyRad *  0.5, joyRad * 0.7, joyRad * 0.7);
				ofSetColor(255);
				ofDrawBitmapString(ofToString(i), xOffset - 7, joyRad * 0.5 + 22);
				xOffset += joyRad + padding * 0.5;
			}
			ofSetRectMode(OF_RECTMODE_CORNER);
			yOffset = joyRad * 2 + 4 * padding;
		}
	}
	ofPopStyle();
	ofPopMatrix();
}


bool ofxGLFWJoystick::isJoystickAvailable(int joyID){
	if(joyID < GLFW_JOYSTICK_LAST){
		if(joyData[joyID].available){
			return true;
		}
	}
	return false;
}


string ofxGLFWJoystick::getJoystickName(int joyID){
	if(isJoystickAvailable(joyID)){
		return joyData[joyID].name;
	}
	return "";
}


int ofxGLFWJoystick::getNumButtons(int joyID){
	if(isJoystickAvailable(joyID)){
		return joyData[joyID].numButtons;
	}
	return 0;
}


int ofxGLFWJoystick::getNumAxis(int joyID){
	if(isJoystickAvailable(joyID)){
		return joyData[joyID].numAxis;
	}
	return 0;
}


const unsigned char * ofxGLFWJoystick::getButtonsForJoystick(int joyID){
	if(isJoystickAvailable(joyID)){
		return joyData[joyID].buttonData;
	}
	return NULL;
}


const float * ofxGLFWJoystick::getButtonsForAxis(int joyID){
	if(isJoystickAvailable(joyID)){
		return joyData[joyID].axisData;
	}
	return NULL;
}








