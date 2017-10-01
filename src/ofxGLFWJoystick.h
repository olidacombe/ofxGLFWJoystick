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

private:

	ofxGLFWJoystick();
	void lookForJoysticks();

	struct JoyData{
		bool available;
		string name;
		int numButtons;
		const unsigned char * buttonData;
		int numAxis;
		const float * axisData;

		JoyData(){
			buttonData = NULL; axisData = NULL;
			numButtons = numAxis = 0;
			available = false;
		}

                diff operator%(const JoyData& other) {
                  diff changes;
                  // populate it...

                  // return it...
                  return changes;
                }
	};

	int numJoysticks;
	JoyData joyData[GLFW_JOYSTICK_LAST];
        JoyData prevJoyData[GLFW_JOYSTICK_LAST];
    std::map<int, unsigned char> buttonValueQueue;
    std::map<int, float> axisValueQueue;
};

#endif /* defined(__emptyExample__ofxGLFWJoystick__) */
