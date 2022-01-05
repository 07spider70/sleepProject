#include "mbed.h"


InterruptIn center(JOYSTICK_CENTER);

LowPowerTicker lpt;

DigitalOut led1(LED1);

volatile int sleepTime = 0;
volatile bool canSleep = false;
void centerPressed();
void handlerTimeOut();
void init();
void butStart();

void init() {
    center.rise(&centerPressed);   
    center.mode(PullDown);
    sleepTime = 0;

}

void butStart() {
    led1 = 1;
    canSleep = false;
    lpt.detach();
    center.enable_irq();
}

void handlerTimeOut() {
    led1 = 0;
    center.disable_irq();
    lpt.attach(&butStart, sleepTime);
    sleepTime = 0;
    canSleep = true;

}

void centerPressed() {
    sleepTime++;
    lpt.attach(&handlerTimeOut,1);
}

// main() runs in its own thread in the OS
int main()
{
    init();
    butStart();
  while(true) {
    if (canSleep) {
            sleep();
    }
    
  }

}

