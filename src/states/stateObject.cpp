#include "stateObject.h"

stateObject::stateObject()
{
    running = false;
    startTime = 0;
};


// ---
// Dummy startup object
void stateObject::startupObject()
{

}

// ---
int stateObject::loopObject()
{
    stateChange = inputFunction();  
    drawFunction();
    return(stateChange);
};
// ---
// Dummy redraw object function
void stateObject::redraw()
{

}
