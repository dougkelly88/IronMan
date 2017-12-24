/* The purpose of this code is to test the function of 
 *  the control button. Since geometry dictates that it
 *  should be connected to pin 1, which doubles as LED 
 *  control, and since there is no means to easily debug
 *  the trinket over USB/serial as might be the case with
 *  the Uno, we must switch between using pin 1 as an output
 *  and an input and fix some timing constraints. 
 */

 int pin = 1;
 

void setup() {
  // start up with the pin listening for button presses
  pinMode(pin, INPUT);
}

void loop() {
  // check on each loop whether button is pressed, and react accordingly
  if (digitalRead(pin) == HIGH)
  {
    onButtonPress();
  }
}

void onButtonPress() {
  pinMode(pin, OUTPUT);
  for (int idx = 0; idx <=5; idx++){
    digitalWrite(pin, HIGH);
    delay(1000);
    digitalWrite(pin, LOW);
    delay(1000);
  }
}

