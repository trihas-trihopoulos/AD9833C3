#include "AD9833C3.h"

// ---------------
// -------
// Due to the mess with the enum...
MD_AD9833::mode_t convertIntToAD9833Mode(int mode)
{
  MD_AD9833::mode_t md;
  switch (mode)
  {
  case 0:
    md= MD_AD9833::MODE_OFF;     // Enum mode_t MODE_OFF, MODE_SINE, MODE_SQUARE1, MODE_SQUARE2,  MODE_TRIANGLE  
    break;
  case 1:
    md= MD_AD9833::MODE_SINE;    // Enum mode_t MODE_OFF, MODE_SINE, MODE_SQUARE1, MODE_SQUARE2,  MODE_TRIANGLE  
    break;
  case 2:
    md= MD_AD9833::MODE_SQUARE1;     // Enum mode_t MODE_OFF, MODE_SINE, MODE_SQUARE1, MODE_SQUARE2,  MODE_TRIANGLE  
    break;
  case 3:
    md= MD_AD9833::MODE_SQUARE2;     // Enum mode_t MODE_OFF, MODE_SINE, MODE_SQUARE1, MODE_SQUARE2,  MODE_TRIANGLE  
    break;
  case 4:
    md= MD_AD9833::MODE_TRIANGLE; // Enum mode_t MODE_OFF, MODE_SINE, MODE_SQUARE1, MODE_SQUARE2,  MODE_TRIANGLE  
    break;
  default:
    md= MD_AD9833::MODE_OFF;
    break;
  }
  return(md);
}

//--------------
//--------------
int manageJoystickInputs(void)
{
  Serial.println("manageJoystickInputs of basicFrequencyStateObject");
  return(STATE__NO_CHANGE);
}
//--------------
void updateWorkingParameters(void)
{
  if(
      (workingParameters.AD9833_phase     !=  AD9833_phase     ) ||                       
      (workingParameters.AD9833_frequency !=  AD9833_frequency ) ||   
      (workingParameters.AD9833_mode      !=  AD9833_mode      ) ||                   
      (workingParameters.MCP41010_value   !=  MCP41010_value   )
    )
  {
    workingParameters.AD9833_phase                     =   AD9833_phase     ;                       
    workingParameters.AD9833_frequency                 =   AD9833_frequency ;   
    workingParameters.AD9833_mode                      =   AD9833_mode      ;                   
    workingParameters.MCP41010_value                   =   MCP41010_value   ;           
    set_signal_generator_parameters();
    prepareDataForJSONTransmission();
    print_signal_generator_parameters();        
  }

}
