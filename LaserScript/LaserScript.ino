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
#define JOB_RUNNING 6
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


bool machineOn = digitalRead(MACHINE_POWER);

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
  if(machineOn){
    laserStatus = chillerToggle;
  }
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
  }
}

void machineIdleState(){
  if (!machineOn) {
    laserStatus = chillerWait;
    return;
  } else if (digitalRead(JOB_RUNNING)){
    laserStatus = exhaustToggle;
    return;
  } else if(!digitalRead(FAN_RELAY)) {
    laserStatus = exhaustTimer;
    return;
  }
  return;
}

void exhaustToggleState(){
  if(!digitalRead(JOB_RUNNING)){
    digitalWrite(FAN_RELAY,OFF);
    laserStatus = machineIdle;
    return;
  } else {
    digitalWrite(FAN_RELAY,ON);
    laserStatus = jobIdle;
    return;
  }
  return;
}

void jobIdleState(){
  if(digitalRead(AIR_REQUEST)){
    laserStatus = airToggle;
    return;
  } else if (!machineOn){
    laserStatus = chillerWait;
    return;
  } else if (!digitalRead(JOB_RUNNING)){
    laserStatus = machineIdle;
    return;
  }
  return;
}

void airToggleState(){
  digitalWrite(AIR_RELAY,!digitalRead(AIR_REQUEST));
  laserStatus = jobIdle;
  return;
}

void exhaustWaitState(){
  if(digitalRead(FAN_RELAY) == OFF){
    laserStatus = chillerWait;
    return;
  } else if (digitalRead(JOB_RUNNING)){
    laserStatus = jobIdle;
    return;
  } else if (!exhaustTimerOn){
    exhaustTimerSet();
    return;
  } else if (exhaustTimerCheck()){
    exhaustTimerReset();
    laserStatus = exhaustToggle;
    return;
  }
  return;
}

void chillerWaitState(){
  if(machineOn){
    chillerTimerReset();
    exhaustTimerReset();
    laserStatus = machineIdle;
    return;
  } else if(digitalRead(FAN_RELAY) == ON){
    laserStatus = exhaustWait;
    return;
  } else if (!chillerTimerOn) {
    chillerTimerSet();
    return;
  } else if(chillerTimerCheck()){
    laserStatus = chillerToggle;
    return;
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
  laserState();
}
