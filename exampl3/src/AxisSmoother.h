#pragma once

#include "ofxGLFWJoystick.h"

using JoyData = ofxGLFWJoystick::JoyData;
using axisMapType = JoyData::axisMapType;
using Processor = JoyData::Processor<axisMapType>;

class AxisSmoother : public Processor {
public:

	//typedef std::map<int, float> mapType;
    
    AxisSmoother(float amt) {
        setAmount(amt);
    }

    void process(std::unordered_map<int, float>& changes) {
        //target.merge(changes); // nope

        for(auto& kv : changes) {
            target[kv.first] = kv.second;
        }
        step();
        for(auto& kv: current) {
            changes[kv.first] = kv.second;
        }
    }


    void setAmount(float amt) {
        amount = ofClamp(amt, 0.0, 1.0);
    }


    const axisMapType& getValues() {
        return current;
    }


private:
    float amount;

    /* modify current according to target and strategy */
    void step() {
        const float p = amount;
        const float q = 1-p;

        std::list<int> toDelete;
        for(auto& kv : target) {
            try {
                const float& oldVal = current.at(kv.first);
                if(oldVal == kv.second) {
                    toDelete.push_back(kv.first);
                    /*
                    target.erase(kv.first); // can I do this in the loop?
                    current.erase(kv.first);
                    */
                } else {
                    current[kv.first] = oldVal * p + kv.second * q;
                }
            } catch(std::out_of_range e) {
            //current[kv.first] = kv.second;
                current[kv.first] = 0.0;
            }
            
            // wipe converged values
            for(auto& i : toDelete) {
                target.erase(i);
                current.erase(i);
            }
        }
    }


    // unordered_map?
    axisMapType current;
    axisMapType  target;
};
