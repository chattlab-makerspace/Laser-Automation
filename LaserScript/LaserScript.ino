#define ON 0
#define OFF 1

//outputs
#define COOLER_RELAY 9
#define FAN_RELAY 10
#define AIR_RELAY 11
#define AUX_RELAY 12

//inputs
#define MACHINE_POWER 4
#define AIR_REQUEST 5
#define SAFETY_SWITCH 6
#define MANUAL_RESET 7

unsigned long seconds = 1000;
unsigned long minutes = 60 * seconds;

unsigned long exhaustDelay = 30 * seconds;
unsigned long chillerDelay = 15 * minutes;

unsigned long exhaustTimer = 0;
unsigned long chillerTimer = 0;

bool exhaustTimerOn = 0;
bool chillerTimerOn = 0;

byte laserStatus;

const byte machineOff = 0;
const byte chillerToggle = 1;
const byte machineIdle = 2;
const byte exhaustToggle = 3;
const byte jobIdle = 4;
const byte airToggle = 5;
const byte exhaustWait = 6;
const byte chillerWait = 7;
const byte err = 100;


bool coolerStatus = !digitalRead(COOLER_RELAY);
bool exhaustStatus = !digitalRead(FAN_RELAY);
bool airStatus = !digitalRead(AIR_RELAY);


bool machineOn = digitalRead(MACHINE_POWER);
bool airRequested = digitalRead(AIR_REQUEST);
bool estopped = digitalRead(SAFETY_SWITCH);
bool manualReset = digitalRead(MANUAL_RESET);

void (* resetFunc) (void) = 0;

void setup() {
  Serial.begin(115200);
  pinMode(COOLER_RELAY, OUTPUT);
  pinMode(FAN_RELAY, OUTPUT);
  pinMode(AIR_RELAY, OUTPUT);
  pinMode(AUX_RELAY, OUTPUT);
  digitalWrite(COOLER_RELAY, OFF);
  digitalWrite(FAN_RELAY, OFF);
  digitalWrite(AIR_RELAY, OFF);
  digitalWrite(AUX_RELAY, OFF);
}

void relayTest(){  
  digitalWrite(AUX_RELAY, OFF);
  digitalWrite(COOLER_RELAY, ON);
  delay(500);
  digitalWrite(COOLER_RELAY, OFF);
  digitalWrite(FAN_RELAY, ON);
  delay(500);
  digitalWrite(FAN_RELAY, OFF);
  digitalWrite(AIR_RELAY, ON);
  delay(500);
  digitalWrite(AIR_RELAY, OFF);
  digitalWrite(AUX_RELAY, ON);
  delay(500);
}

void inputCheck(){
  machineOn = digitalRead(MACHINE_POWER);
  airRequested = digitalRead(AIR_REQUEST);
  estopped = digitalRead(SAFETY_SWITCH);
  manualReset = digitalRead(MANUAL_RESET);
}

void outputCheck(){
  coolerStatus = !digitalRead(COOLER_RELAY);
  exhaustStatus = !digitalRead(FAN_RELAY);
  airStatus = !digitalRead(AIR_RELAY);
}

void exhaustTimerSet(){
  exhaustTimerOn = 1;
  exhaustTimer = millis();
}

void exhaustTimerReset(){
  exhaustTimerOn = 0;
  exhaustTimer = 0;
}

bool exhaustTimerCheck(){
  if(exhaustTimerOn == 1){
    if(millis()-exhaustTimer<=exhaustDelay){
      return 0;
    } else {
      return 1;
    }
  } else {
    return 0;
  }
}

void chillerTimerSet(){
  chillerTimerOn = 1;
  chillerTimer = millis();
}

void chillerTimerReset(){
  chillerTimerOn = 0;
  chillerTimer = 0;
}

bool chillerTimerCheck(){
  if(chillerTimerOn == 1){
    if(millis()-chillerTimer<=chillerDelay){
      return 0;
    } else {
      return 1;
    }
  } else {
    return 0;
  }
}

void machineOffState(){
  delay(5000);
  return;
}

void chillerToggleState(){
  if(machineOn){
    digitalWrite(COOLER_RELAY, ON);
    laserStatus=machineIdle;
    return;
  } else if(!machineOn){
    digitalWrite(COOLER_RELAY, OFF);
    laserStatus=machineOff;
    return;
  } else {
    laserStatus=err;
    return;
  }
}

void machineIdleState(){
  return;
}

void exhaustToggleState(){
  return;
}

void jobIdleState(){
  return;
}

void airToggleState(){
  digitalWrite(AIR_RELAY,!digitalRead(AIR_REQUEST));
  laserStatus = jobIdle;
  return;
}

void exhaustWaitState(){
  return;
}

void chillerWaitState(){
  if(machineOn){
    chillerTimerReset();
    exhaustTimerReset();
    laserStatus = machineIdle;
  }
  if(chillerTimerCheck()){
    laserStatus = chillerToggle;
    return;
  }
  if(exhaustStatus == ON){
    if (!exhaustTimerCheck){
      exhaustTimerSet();
      return;
    } else {
      laserStatus = exhaustToggle;
      return;
    }
  }
}

void errState(){
  return;
}

void laserState() {
  switch (laserStatus){
    case machineOff:
      machineOffState();
    break;

    case chillerToggle:
      chillerToggleState();
    break;

    case machineIdle:
      machineIdleState();
    break;

    case exhaustToggle:
      exhaustToggleState();
    break;

    case jobIdle:
      jobIdleState();
    break;

    case airToggle:
      airToggleState();
    break;

    case exhaustWait:
      exhaustWaitState();
    break;

    case chillerWait:
      chillerWaitState();
    break;

    case err:
      errState();
    break;    
  }
}

void loop() {
  //relayTest();
  inputCheck();
  outputCheck();
  laserState();
}
