#pragma once

class Joystick {
public:
    Joystick(int id=0);
    ~Joystick();
    bool isAvailable() { return available; }
private:
    bool available;
    int ID; 
    string name;

};
