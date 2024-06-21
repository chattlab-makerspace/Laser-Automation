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

byte laserStatus;

const byte fullOff = 0;
const byte machineOn = 1;
const byte chillerStart = 2;
const byte chillerOn = 3;
const byte jobStart = 4;
const byte airStart = 5;
const byte airOn = 6;
const byte exhaustStart = 7;
const byte exhaustOn = 8;
const byte jobEnd = 9;
const byte airEnd = 10;
const byte airOff = 11;
const byte exhaustTimerStart = 12;
const byte exhaustWait = 13;
const byte exhaustEnd = 14;
const byte exhaustOff = 15;
const byte machineEnd = 16;
const byte chillerTimerStart = 17;
const byte chillerWait = 18;
const byte chillerEnd = 19;
const byte chillerOff = 20;
const byte machineOff = 21;
const byte machineOffAirCheck = 22;
const byte machineOffAirEnd = 23;
const byte machineOffAirOff = 24;
const byte machineOffExhaustCheck = 25;
const byte machineOffExhaustTimerStart = 26;
const byte machineOffExhaustWait = 27;
const byte machineOffExhaustEnd = 28;
const byte machineOffExhaustOff = 29;
const byte machineOffChillerCheck = 30;
const byte machineOffChillerTimerStart = 31;
const byte machineOffChillerWait = 32;
const byte machineOffChillerEnd = 33;
const byte machineOffChillerOff = 34;
const byte outOfBounds = 100;

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

void laserState() {
  switch (laserStatus){
    case fullOff:
    break;
    
    case machineOn:
    break;

    case chillerStart:
    break;

    case chillerOn:
    break;

    case jobStart:
    break;

    case airStart:
    break;

    case airOn:
    break;

    case exhaustStart:
    break;

    case exhaustOn:
    break;

    case jobEnd:
    break;

    case airEnd:
    break;

    case airOff:
    break;

    case exhaustWait:
    break;

    case exhaustEnd:
    break;

    case exhaustOff:
    break;

    case machineEnd:
    break;

    case chillerWait:
    break;

    case chillerEnd:
    break;

    case chillerOff:
    break;

    case outOfBounds:
    break;

  }
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

void loop() {
  //relayTest();
  //laserState();
}
