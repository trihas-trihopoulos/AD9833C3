#ifndef UI_FUNCTIONS_H
#define UI_FUNCTIONS_H

#pragma once
// --------------------------
#include "AD9833C3.h"

typedef void (screensFunction)(void);               // function pointer to screen handling function
typedef void (inputsFunction)(void);                // function pointer to inputs (buttons etc) handling function

#define MAX_UI_SCREENS 10                           // Maximum number of screens
// -------------------------------------------
// Specifies an OLED screen and inputs handling functions  
class uiScreenObject
{
  private:

  public:
    bool        enabled    = false;
    long        timeToStayOnScreen;

    uiScreenObject();
    uiScreenObject(screensFunction sfunc, inputsFunction ifunc, long st, bool en);

    screensFunction *drawFunction=NULL;                  // Called for screen drawing
    inputsFunction  *inputFunction=NULL;
    screensFunction *overlayFunction=NULL;               // possible overlay function
};
// --------------------------
class uiScreensManager
{
  private:

  public:
    int NoOfScreens=0;
    uiScreenObject screens[MAX_UI_SCREENS];
    uiScreensManager();
};
// --------------------------
extern uiScreenObject uobj;

#endif //UI_FUNCTIONS_H
