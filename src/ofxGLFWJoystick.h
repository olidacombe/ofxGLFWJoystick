//
//  ofxGLFWJoystick.h
//  emptyExample
//
//  Created by Oriol Ferrer Mesià on 07/02/15.
//
//

#ifndef __emptyExample__ofxGLFWJoystick__
#define __emptyExample__ofxGLFWJoystick__

#include "ofMain.h"

#if (_MSC_VER)
	#include <GLFW/glfw3.h>
#else
	#include "GLFW/glfw3.h"
#endif


class ofxGLFWJoystick{

public:

	void setup(){};
	void update();

	static ofxGLFWJoystick& one(){
		static ofxGLFWJoystick instance; // Instantiated on first use.
		return instance;
	}


	void printJoystickList();
	map<int,string> getJoystickList();


	int getNumJoysticksAvailable(){ return numJoysticks;};
	inline bool isJoystickAvailable(int joyID);
	string getJoystickName(int joyID);

	void drawDebug(int x, int y);

        struct diff {
          std::map<int, unsigned char> buttons;
          std::map<int, float> axes;
        };

        const diff getChangedValues(int joyID);

	//safe access
	float getAxisValue(int axisID, int joyID);
	unsigned char getButtonValue(int axisID, int joyID);

	//direct access, careful with the pointers/indexes or you will crash!
	int getNumButtons(int joyID);
	int getNumAxis(int joyID);
	const unsigned char * getButtonsForJoystick(int joyID);
	const float * getButtonsForAxis(int joyID);

//private:

	struct JoyData{
		bool available;
		string name;
		int numButtons;
		const unsigned char * buttonData;
                std::vector<unsigned char> prevButtonData;
		int numAxis;
		const float * axisData;
                std::vector<float> prevAxisData;

		JoyData(){
			buttonData = NULL; axisData = NULL;
			numButtons = numAxis = 0;
			available = false;
		}

                void clear() {
                  axisData = NULL;
                  buttonData = NULL;
                  numAxis = 0;
                  numButtons = 0;
                  prevAxisData.clear();
                  prevButtonData.clear();
                }
                
                void shift() {
                  prevButtonData.assign(buttonData, buttonData + numButtons);
                  prevAxisData.assign(axisData, axisData + numAxis);
                }

                diff changes() {
                  diff changes;
                  const int cmpAxes = min(static_cast<size_t>(numAxis), prevAxisData.size());
                  const int cmpButtons = min(static_cast<size_t>(numButtons), prevAxisData.size());
                  for(int i=0; i<cmpAxes; i++) {
                    if(axisData[i] != prevAxisData[i]) {
                      changes.axes[i] = axisData[i];
                    }
                  }
                  for(int i=0; i<cmpButtons; i++) {
                    if(axisData[i] != prevButtonData[i]) {
                      changes.buttons[i] = axisData[i];
                    }
                  }
                  return changes;
                }

                /*
                diff operator%(const JoyData& other) {
                  diff changes;
                  const int cmpAxes = min(numAxis, other.numAxis);
                  const int cmpButtons = min(numButtons, other.numButtons);

                  // this makes me uncomfortable
                  for(int i=0; i<cmpAxes; i++) {
                    if(axisData[i] != other.axisData[i]) {
                      changes.axes[i] = axisData[i];
                    }
                  }
                  for(int i=0; i<cmpButtons; i++) {
                    if(buttonData[i] != other.buttonData[i]) {
                      changes.buttons[i] = buttonData[i];
                    }
                  }

                  ofLogNotice() << this->buttonData[0];
                  return changes;
                }
                */
	};

private:

	ofxGLFWJoystick();
	void lookForJoysticks();

	int numJoysticks;
	JoyData joyData[GLFW_JOYSTICK_LAST];
        JoyData prevJoyData[GLFW_JOYSTICK_LAST];

    std::map<int, unsigned char> buttonValueQueue;
    std::map<int, float> axisValueQueue;
};

#endif /* defined(__emptyExample__ofxGLFWJoystick__) */
