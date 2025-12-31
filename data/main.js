var connection = new WebSocket('ws://' + location.hostname + '/ws', ['arduino']);

connection.onopen = function () {
  console.log('Connected: ');
  sendData(0);
};

connection.onerror = function (error) {
  console.log('WebSocket Error ', error);
};

connection.onmessage = function (e) {
  console.log('Server: ', e.data);
  processData(e.data);
};

connection.onclose = function () {
  console.log('WebSocket connection closed');
};

document.getElementById("tbParameters").click();
let command             = "";
let AD9833_frequency    = "";          
let AD9833_mode         = "";    
let AD9833_phase        = "";      
let MCP41010_value      = "";        
let startfrequency      = "";        
let endfrequency        = "";      
let stepfrequency       = "";      
let frequencystepTime   = "";          
let startAmplitude      = "";        
let stopAmplitude       = "";      
let stepAmplitude       = "";      
let amplitudeStepTime   = "";          
let sweepMode           = "";  

// --------
function openMyTab(evt, TabName) {
  var i, tabcontent, tablinks;
  tabcontent = document.getElementsByClassName("tabcontent");
  for (i = 0; i < tabcontent.length; i++) {
    tabcontent[i].style.display = "none";
  }
  tablinks = document.getElementsByClassName("tablinks");
  for (i = 0; i < tablinks.length; i++) {
    tablinks[i].className = tablinks[i].className.replace(" active", "");
  }
  document.getElementById(TabName).style.display = "block";
  evt.currentTarget.className += " active";
}
// --------
function sendData(command)
{
  switch (command) 
  {
    case 0:
      sendUpdatedParametersRequest();      
      break;
    case 1:
      sendBasicParameters();      
      break;
  
    default:
      break;
  }

}
// -------
function processData(data)
{
  console.log("Received data:");
  console.log(data);
  let json = JSON.parse(data); 

  document.getElementById('fr').value           = json["ADfreq"];
  document.getElementById('lphase').value       = json["ADphas"];
  document.getElementById('lampl').value        = json["MCPval"];

  let mode = json["ADmode"];
  switch (mode) 
  {
    case 0:
      document.getElementById('off').checked = true 
      break;
    case 1:
      document.getElementById('sine').checked = true 
      break;
    case 2:
      document.getElementById('sq1').checked = true 
      break;
    case 3:
      document.getElementById('sq2').checked = true 
      break;
    case 4:
      document.getElementById('tri').checked = true 
      break;
    default:
      break;
  }
}
// Request update from ESP
function sendUpdatedParametersRequest()
{
  const JsonValues = 
  {
  "cmd":0
  }
	var jsonData = JSON.stringify(JsonValues);
  console.log(jsonData);
  connection.send(jsonData);
}
// --- Just the basic parameters
function sendBasicParameters()
{
  console.log('In getGuiParameters');
  radiobuttonValue = document.querySelector('input[name="adMode"]:checked').value
  console.log(radiobuttonValue);
  switch (radiobuttonValue) {
    case 'off':
      AD9833_mode = 0;
      break;
    case 'sinewave':
      AD9833_mode = 1;
      break;
    case 'square1':
      AD9833_mode = 2;
      break;
    case 'square2':
      AD9833_mode = 3;
      break;
    case 'triangle':
      AD9833_mode = 4;
      break;
    default:
      break;
  }
  console.log(AD9833_mode);

  command             = 1;  // Sending basic parameters
  AD9833_frequency    = document.getElementById('fr').value;          
  console.log(AD9833_frequency);
  // let AD9833_mode         = document.getElementById('').value;    
  AD9833_phase        = document.getElementById('lphase').value;      
  console.log(AD9833_phase);
  MCP41010_value      = document.getElementById('lampl').value;        
  console.log(MCP41010_value);

  const JsonValues = {
  "cmd":command,
  "ADfreq":AD9833_frequency,
  "ADmode":AD9833_mode,
  "ADphas":AD9833_phase,
  "MCPval":MCP41010_value
  };

	var jsonData = JSON.stringify(JsonValues);
  console.log(jsonData);
  connection.send(jsonData);
}

    /* 
  startfrequency      = document.getElementById('').value;        
  endfrequency        = document.getElementById('').value;      
  stepfrequency       = document.getElementById('').value;      
  frequencystepTime   = document.getElementById('').value;          
  startAmplitude      = document.getElementById('').value;        
  stopAmplitude       = document.getElementById('').value;      
  stepAmplitude       = document.getElementById('').value;      
  amplitudeStepTime   = document.getElementById('').value;          
  sweepMode           = document.getElementById('').value;  
  "stFreq"
  "enFreq"
  "stFreq"
  "fstTim"
  "stAmpl"
  "stpAmp"
  "stepAm"
  "aStpTm"
  "swMode"




  */
// -------






