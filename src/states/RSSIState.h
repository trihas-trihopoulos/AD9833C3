#ifndef RSSI_STATE_H
#define RSSI_STATE_H
// ------------
#include "Arduino.h"

// ----------
// Axis coordinates
#define     X_AXIS_STARTX       28              
#define     X_AXIS_STARTY       51               
#define     X_AXIS_ENDX         127   
#define     X_AXIS_ENDY         X_AXIS_STARTY

#define     X_AXIS_TICK_STEP    5
#define     X_AXIS_TICK_LENGTH  2

#define     Y_AXIS_STARTX       X_AXIS_STARTX
#define     Y_AXIS_STARTY       0
#define     Y_AXIS_ENDX         X_AXIS_STARTX
#define     Y_AXIS_ENDY         X_AXIS_STARTY

#define     Y_AXIS_TICK_STEP    5
#define     Y_AXIS_TICK_LENGTH  2

// -------
// data array 
#define ARR_LEN  X_AXIS_ENDX - X_AXIS_STARTX - 1      // Data array LENGTH - matches diagram width
#define RSSI_MEASURMENT_PERIOD   1000                 // How often to update diagram - read RSSI, in milliseconds
// -------
class RSSIStateObject : public stateObject 
{
  private:

    int data[ARR_LEN];
    int last_data_index;    // Points to the last entry of the array
    ulong last_measurement_mills;

    int last_rssi;
    bool error;

    int max_rssi;
    int min_rssi;

    int graph_max_rssi;
    int graph_min_rssi;
    float yscale_factor;

    char strRSSI[5];

  public:

    // Parameters 

    RSSIStateObject();
    virtual void startupObject();
    virtual int loopObject();
    virtual void redraw();

    int updateRSSI();       // Updates data array, min and max

    void drawAxis();        // Draw axis only
    void drawRSSI();
};

#endif  //RSSI_STATE_H