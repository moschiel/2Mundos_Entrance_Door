//// == WATCHDOG
void IRAM_ATTR resetModule() {
  Serial.println(" ******* ");
  Serial.println(" ***  A L A R M  **** ");
  Serial.println(" Timer Overflow on 5 seconds ... ");
  Serial.println(" ***  A L A R M  **** ");
  ESP.restart();
}


void IRAM_ATTR doorOff(){
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(DOOR_PIN, LOW);
  timerEnd(DoorTimer);
}

void WDT_setup()
{
   //https://techtutorialsx.com/2017/10/07/esp32-arduino-timer-interrupts/  
  //we will initialize our timer with a call to the timerBegin function, which will return a pointer to a structure of type hw_timer_t, which is the one of the timer global variable we declared(velTimer).
  //As input, this function receives the number of the timer we want to use (from 0 to 3, since we have 4 hardware timers), the value of the prescaler and a flag indicating if the counter should count up (true) or down (false)
  //Regarding the prescaler, typically the frequency of the base signal used by the ESP32 counters is 80 MHz, which means the signal would make the timer counter increment 80 000 000 times per second.
  //Although we could make the calculations with this value to set the counter number for generating the interrupt, we will take advantage of the prescaler to simplify it. Thus, if we divide this value by 80 (using 80 as the prescaler value), we will get a signal with a 1 MHz frequency that will increment the timer counter 1 000 000 times per second.
  WDTtimer = timerBegin(0, 80, true);   
  //before enabling the timer, we need to bind it to a handling function(velDisplay), which will be executed when the interrupt is generated. This is done with a call to the timerAttachInterrupt function.
  //This function receives as input a pointer to the initialized timer, which we stored in our global variable(velTimer), the address of the function(velDisplay) that will handle the interrupt and a flag indicating if the interrupt to be generated is edge (true) or level (false).
  timerAttachInterrupt(WDTtimer, &resetModule, true);
  //Next we will use the timerAlarmWrite function to specify the counter value in which the timer interrupt will be generated. So, this function receives as first input the pointer to the timer, as second the value of the counter in which the interrupt should be generated, and as third a flag indicating if the timer should automatically reload upon generating the interrupt.
  timerAlarmWrite(WDTtimer, 6000000, false);
  //We finish our setup function by enabling the timer with a call to the timerAlarmEnable function, passing as input our timer variable.
  timerAlarmEnable(WDTtimer);
}

void DoorTimer_setup()
{
   //https://techtutorialsx.com/2017/10/07/esp32-arduino-timer-interrupts/  
  //we will initialize our timer with a call to the timerBegin function, which will return a pointer to a structure of type hw_timer_t, which is the one of the timer global variable we declared(velTimer).
  //As input, this function receives the number of the timer we want to use (from 0 to 3, since we have 4 hardware timers), the value of the prescaler and a flag indicating if the counter should count up (true) or down (false)
  //Regarding the prescaler, typically the frequency of the base signal used by the ESP32 counters is 80 MHz, which means the signal would make the timer counter increment 80 000 000 times per second.
  //Although we could make the calculations with this value to set the counter number for generating the interrupt, we will take advantage of the prescaler to simplify it. Thus, if we divide this value by 80 (using 80 as the prescaler value), we will get a signal with a 1 MHz frequency that will increment the timer counter 1 000 000 times per second.
  DoorTimer = timerBegin(0, 80, true);   
  //before enabling the timer, we need to bind it to a handling function(velDisplay), which will be executed when the interrupt is generated. This is done with a call to the timerAttachInterrupt function.
  //This function receives as input a pointer to the initialized timer, which we stored in our global variable(velTimer), the address of the function(velDisplay) that will handle the interrupt and a flag indicating if the interrupt to be generated is edge (true) or level (false).
  timerAttachInterrupt(DoorTimer, &doorOff, true);
  //Next we will use the timerAlarmWrite function to specify the counter value in which the timer interrupt will be generated. So, this function receives as first input the pointer to the timer, as second the value of the counter in which the interrupt should be generated, and as third a flag indicating if the timer should automatically reload upon generating the interrupt.
  timerAlarmWrite(DoorTimer, 800000, false); //0,8s
  //We finish our setup function by enabling the timer with a call to the timerAlarmEnable function, passing as input our timer variable.
  timerAlarmEnable(DoorTimer);
}
