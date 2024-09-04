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

  public:
    int inputPin;
    int outputPin;
    unsigned long delay;

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

    void turnOn(){
      if (digitalRead(MACHINE_POWER) == ON && digitalRead(inputPin) == ON){
        Serial.println("Relay " + String(digitalRead(outputPin)));
        if (digitalRead(outputPin) == OFF){
          digitalWrite(outputPin, ON);
        } else {
          //timerClear(exhaust);
          Serial.println(timer.size());
          Serial.println(timer.empty());
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
};

  /*  
  
  bool exhaustOff(void *){
    digitalWrite(EXHAUST_RELAY, OFF);
    return false;
  }

// Exhaust Handling
  if (digitalRead(MACHINE_POWER) == ON && digitalRead(EXHAUST_REQUEST) == ON){
    Serial.println("Relay " + String(digitalRead(EXHAUST_RELAY)));
    if (digitalRead(EXHAUST_RELAY) == OFF){
      digitalWrite(EXHAUST_RELAY, ON);
    } else {
      //timerClear(exhaust);
      Serial.println(exhaust.size());
      Serial.println(exhaust.empty());
      if(!exhaust.empty()){
        exhaust.cancel();
      }
    }
  }
  if ((digitalRead(MACHINE_POWER) == OFF || (digitalRead(EXHAUST_REQUEST) == OFF) && digitalRead(EXHAUST_RELAY) == ON)){
    if (exhaust.empty()){
      exhaust.in(exhaustDelay, exhaustOff);
    }
  }*/

auto air = timer_create_default();
//auto exhaust = timer_create_default();
auto chiller = timer_create_default();

unsigned long second = 1000;
unsigned long minute = 60 * second;
unsigned long hour = 60 * minute;

unsigned long airDelay = 2 * second;
unsigned long exhaustDelay = 20 * second;
unsigned long chillerDelay = 15 * minute;

Relay exhaust(EXHAUST_REQUEST, EXHAUST_RELAY, exhaustDelay);

void setup() {
  Serial.begin(115200);
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


//bool exhaustOff(void *){
//  digitalWrite(EXHAUST_RELAY, OFF);
//  return false;
//}

bool chillerOff(void *){
  digitalWrite(CHILLER_RELAY, OFF);
  return false;
}

bool airOff(void *){
  digitalWrite(AIR_RELAY, OFF);
  return false;
}

void timerClear(Timer<> timerToClear){
    // ideally would only happen when timer is set, but library does not easily allow
    timerToClear.cancel();
}

void loop() {
  air.tick();
  //exhaust.tick();
  exhaust.loop();
  chiller.tick();
  // Air Handling
  if (digitalRead(MACHINE_POWER) == ON && digitalRead(AIR_REQUEST) == ON && digitalRead(AIR_RELAY) == OFF){
    digitalWrite(AIR_RELAY, ON);
    timerClear(air);
  }
  if ((digitalRead(MACHINE_POWER) == OFF || (digitalRead(AIR_REQUEST) == OFF) && digitalRead(AIR_RELAY) == ON)){
    air.in(airDelay, airOff);
  }
  // Chiller Handling
  if (digitalRead(MACHINE_POWER) == ON && digitalRead(CHILLER_RELAY) == OFF){
    digitalWrite(CHILLER_RELAY, ON);
    timerClear(chiller);
  }
  if (digitalRead(MACHINE_POWER) == OFF && digitalRead(CHILLER_RELAY) == ON){
    chiller.in(chillerDelay, chillerOff);
  }
  /*
  // Exhaust Handling
  if (digitalRead(MACHINE_POWER) == ON && digitalRead(EXHAUST_REQUEST) == ON){
    Serial.println("Relay " + String(digitalRead(EXHAUST_RELAY)));
    if (digitalRead(EXHAUST_RELAY) == OFF){
      digitalWrite(EXHAUST_RELAY, ON);
    } else {
      //timerClear(exhaust);
      Serial.println(exhaust.size());
      Serial.println(exhaust.empty());
      if(!exhaust.empty()){
        exhaust.cancel();
      }
    }
  }
  if ((digitalRead(MACHINE_POWER) == OFF || (digitalRead(EXHAUST_REQUEST) == OFF) && digitalRead(EXHAUST_RELAY) == ON)){
    if (exhaust.empty()){
      exhaust.in(exhaustDelay, exhaustOff);
    }
  }
  */
}
