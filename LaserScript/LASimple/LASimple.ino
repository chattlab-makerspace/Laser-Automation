#include <arduino-timer.h>

#define ON 0
#define OFF 1

//outputs
#define CHILLER_RELAY 9
#define EXHAUST_RELAY 10
#define AIR_RELAY 11
#define AUX_RELAY 12

//inputs
#define MACHINE_POWER 4
#define AIR_REQUEST 5
#define JOB_RUNNING 6
#define MANUAL_RESET 7

#define CHILLER_REQUEST MACHINE_POWER
#define EXHAUST_REQUEST JOB_RUNNING

class Relay {
  private:
    Timer<> timer;
    int inputPin;
    int outputPin;
    unsigned long delay;

    void turnOn(){
      if (digitalRead(MACHINE_POWER) == ON && digitalRead(inputPin) == ON){
        if (digitalRead(outputPin) == OFF){
          digitalWrite(outputPin, ON);
        } else {
          if(!timer.empty()){
            timer.cancel();
          }
        }
      }
    }

    bool relayOff(){
      digitalWrite(outputPin, OFF);
      return false;      
    }

    void turnOff(){
      if ((digitalRead(MACHINE_POWER) == OFF || (digitalRead(inputPin) == OFF) && digitalRead(outputPin) == ON)){
        if (timer.empty()){
          timer.in(delay, [](void *argument) -> bool { return ((Relay*)argument)->relayOff(); }, this);
        }
      }
    }

  public:
    Relay(int inputPin, int outputPin, unsigned long delay){
      this->inputPin = inputPin;
      this->outputPin = outputPin;
      this->timer = timer_create_default();
      this->delay = delay;
    }

    void loop(){
      timer.tick();
      turnOn();
      turnOff();
    }
};

unsigned long second = 1000;
unsigned long minute = 60 * second;
unsigned long hour = 60 * minute;

unsigned long airDelay = 2 * second;
unsigned long exhaustDelay = 20 * second;
unsigned long chillerDelay = 15 * minute;

Relay exhaust(EXHAUST_REQUEST, EXHAUST_RELAY, exhaustDelay);
Relay air(AIR_REQUEST, AIR_RELAY, airDelay);
Relay chiller(CHILLER_REQUEST, CHILLER_RELAY, chillerDelay);

void setup() {
  //Serial.begin(115200);
  pinMode(MACHINE_POWER, INPUT_PULLUP);
  pinMode(AIR_REQUEST, INPUT_PULLUP);
  pinMode(JOB_RUNNING, INPUT_PULLUP);
  pinMode(MANUAL_RESET, INPUT_PULLUP);
  
  pinMode(CHILLER_RELAY, OUTPUT);
  pinMode(EXHAUST_RELAY, OUTPUT);
  pinMode(AIR_RELAY, OUTPUT);
  pinMode(AUX_RELAY, OUTPUT);
  
  digitalWrite(CHILLER_RELAY, OFF);
  digitalWrite(EXHAUST_RELAY, OFF);
  digitalWrite(AIR_RELAY, OFF);
  digitalWrite(AUX_RELAY, OFF);
}

void loop (){
  air.loop();
  exhaust.loop();
  chiller.loop();
} 
