#include "AD9833C3.h"
//------------------------------
char * AD9833_mode_strings[]  = {"Off", "Sinewave", "Triangle", "Square", "Square/2" };

// ---------------
// Update the signal generator board with the parameters from the global variables
void set_signal_generator_parameters()
{
    AD.setFrequency(MD_AD9833::CHAN_0, AD9833_frequency);            // Sets the frequency on channel 0;

    AD.setMode(AD9833_mode);                                        // Sets the mode MD_AD9833::MODE_OFF,MD_AD9833::MODE_SINE,MD_AD9833::MODE_TRIANGLE,MD_AD9833::MODE_SQUARE1

    AD.setPhase(MD_AD9833::CHAN_0, AD9833_phase);                   // Sets the phase
    AD.setActivePhase(MD_AD9833::CHAN_0);                           // Sets the active phase on channel 0

    mcp41010_pot.setValue(MCP41010_value);
}

// ---------------
// Get the currnet signal generator board parameters AND update the global variables
void print_signal_generator_parameters()
{
    int str_index = 0;

    Serial.printf("\n------------------");
    Serial.printf("Frequency: %8.2f\n", AD9833_frequency);

    switch(AD9833_mode)             // Find mode string
    {
        case MD_AD9833::MODE_OFF:
            str_index = 0;
            break;
        case MD_AD9833::MODE_SINE:
            str_index = 1;
            break;
        case MD_AD9833::MODE_TRIANGLE:
            str_index = 2;
            break;
        case MD_AD9833::MODE_SQUARE1:
            str_index = 3;
            break;
        case MD_AD9833::MODE_SQUARE2:
            str_index = 4;
            break;
    }
    Serial.printf("Mode:  %s\n", AD9833_mode_strings[str_index]);
    Serial.printf("Phase: %d (degrees * 10)\n", AD9833_phase);
    Serial.printf("Potentiometer: %d\n", MCP41010_value);
}