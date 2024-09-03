#include <arduino-timer.h>

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

auto chiller = timer_create_default();
auto exhaust = timer_create_default();

unsigned long second = 1000;
unsigned long minute = 60 * second;
unsigned long hour = 60 * minute;

unsigned long exhaustDelay = 20 * second;
unsigned long chillerDelay = 15 * minute;

void setup() {
  pinMode(MACHINE_POWER, INPUT_PULLUP);
  pinMode(AIR_REQUEST, INPUT_PULLUP);
  pinMode(JOB_RUNNING, INPUT_PULLUP);
  pinMode(MANUAL_RESET, INPUT_PULLUP);
  
  pinMode(COOLER_RELAY, OUTPUT);
  pinMode(FAN_RELAY, OUTPUT);
  pinMode(AIR_RELAY, OUTPUT);
  pinMode(AUX_RELAY, OUTPUT);
  
  digitalWrite(COOLER_RELAY, OFF);
  digitalWrite(FAN_RELAY, OFF);
  digitalWrite(AIR_RELAY, OFF);
  digitalWrite(AUX_RELAY, OFF);
}


bool exhaustOff(void *){
  digitalWrite(FAN_RELAY, OFF);
  return false;
}

bool chillerOff(void *){
  digitalWrite(COOLER_RELAY, OFF);
  return false;
}

void loop() {
  chiller.tick();
  exhaust.tick();
  // Air Handling
  if (digitalRead(AIR_REQUEST) == ON && digitalRead(AIR_RELAY) == OFF){
    digitalWrite(AIR_RELAY, ON);
  }
  if (digitalRead(AIR_REQUEST) == OFF && digitalRead(AIR_RELAY) == ON){
    digitalWrite(AIR_RELAY, OFF);
  }
  // Chiller Handling
  if (digitalRead(MACHINE_POWER) == ON && digitalRead(COOLER_RELAY) == OFF){
    digitalWrite(COOLER_RELAY, ON);
  }
  if (digitalRead(MACHINE_POWER) == OFF && digitalRead(COOLER_RELAY) == ON){
  chiller.in(chillerDelay, chillerOff);
  }
  // Exhaust Handling
  if (digitalRead(JOB_RUNNING) == ON && digitalRead(FAN_RELAY) == OFF){
    digitalWrite(FAN_RELAY, ON);
  }
  if (digitalRead(JOB_RUNNING) == OFF && digitalRead(FAN_RELAY) == ON){
  exhaust.in(exhaustDelay, exhaustOff);
  }
}
