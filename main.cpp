#include "mbed.h"


InterruptIn center(JOYSTICK_CENTER);
LowPowerTicker lpt;
DigitalOut led1(LED1);

volatile bool pressed = false;
volatile int sleepTime = 0;
volatile bool readyToAlarm = true;
void turnOnLed() {
    led1 = 1;
}

void turnOffLed() {
    led1 = 0;
}

void center_pressed() {
    sleepTime++;
    pressed = true;
}

void initInter() {
    center.rise(&center_pressed);   
    center.mode(PullDown);
}

void alarmOn() {
    turnOnLed();
    sleepTime = 0;
    lpt.detach();
    readyToAlarm = true;
    center.enable_irq();
}


void prepareAlarm() {
    initInter();
    ThisThread::sleep_for(5000);
    while(pressed || sleepTime == 0) {
        pressed = false;
        ThisThread::sleep_for(2000);
    }
    center.disable_irq();
    turnOffLed();

    printf("sleep: %d s\n\r", sleepTime); 
    // Deep sleep for 1 second
    printf("Deep sleep allowed: %i\r\n", sleep_manager_can_deep_sleep());
    lpt.attach(&alarmOn, sleepTime); 
    sleep();
}

// main() runs in its own thread in the OS
int main()
{
  while(true) {
      if(readyToAlarm) {
          readyToAlarm = false;
          prepareAlarm();
      }
  }

}

