/* Control a pair of Neopixels for a NYE costume. 
 *  Short press of the button cycles brightness and 
 *  colour; long (>3 s) press toggles disco mode
 */

#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
 #include <avr/power.h>
#endif

#define PIN 0

int pin = 1;
int mode = 0;
unsigned long start_time;
unsigned long duration;
unsigned long disco_start;
unsigned long disco_prev_t;
unsigned long flash_duration = 200;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(2, PIN); 
uint32_t color;
uint32_t l_eye_array[6] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFF0000, 0x00FF00, 0x0000FF};
uint32_t r_eye_array[6] = {0xFF0000, 0x00FF00, 0x0000FF, 0x00FF00, 0x0000FF, 0xFF0000};

void setup() {
  // start up with the pin listening for button presses
  pinMode(pin, INPUT);
  // set up neopixels
  #ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
    if(F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  pixels.begin();
}

void loop() {
  // check on each loop whether button is pressed, and react accordingly
  if (digitalRead(pin) == HIGH)
  {
    onButtonPress();
  }
  // in order to make disco mode interruptable, implement in main loop
  if (mode == 7){
    discoMode();
  }
}

void onButtonPress() {
  // time the duration of a button press, and react accordingly
  start_time = millis();
  while (digitalRead(pin) == HIGH)
  {
    // do nothing, just wait
  }
  duration = millis() - start_time;
  if (duration < 3000){
    incrementMode();
  }
  else {
    mode = 7;
    disco_start = millis();
    disco_prev_t = 0;
  }
}

void incrementMode(){
  // cycle through modes
  mode++;
  if (mode > 6){
    mode = 0;
  }
  changeMode();
}

void changeMode(){
  // implement newly selected standard mode
  switch (mode) {
    case 0:
      // turn everything off
      color = pixels.Color(0, 0, 0);
      break;
    case 1:
      // turn on 1/3 brightness, white
      color = pixels.Color(85, 85, 85);
      break;
    case 2: 
      // turn on 2/3 brightness, white
      color = pixels.Color(170, 170, 170);
      break;
    case 3:
      // turn on full brightness, white
      color = pixels.Color(255, 255, 255);
      break;
    case 4: 
      // turn on 2/3 brightness, red
      color = pixels.Color(170, 0, 0);
      break;
    case 5: 
      // turn on 2/3 brightnes, green
      color = pixels.Color(0, 170, 0);
      break;
    case 6: 
      // turn on 2/3 brightness, blue
      color = pixels.Color(0, 0, 170);
      break;
  }
  testFn(mode);
  for (int eidx = 0; eidx < 2; eidx++) pixels.setPixelColor(eidx, color);
  pixels.show();
}

void discoMode(){
  unsigned long t = millis() - disco_start;
  if ((t - disco_prev_t) > flash_duration){
    disco_prev_t = t;
    // round to nearest flash_duration do mods to choose the value of the left and right eye arrays to set colors to 
    int cidx = floor((t % (6 * flash_duration))/flash_duration);
    pixels.setPixelColor(0, l_eye_array[cidx]);
    pixels.setPixelColor(1, r_eye_array[cidx]);
    pixels.show();
  }
}

void testFn(int md){
  // function to test incrementing mode
  pinMode(pin, OUTPUT);
  for (int idx = 0; idx < md; idx++){
    digitalWrite(pin, LOW);
    delay(flash_duration);
    digitalWrite(pin, HIGH);
    delay(flash_duration);
    digitalWrite(pin, LOW);
  }
  pinMode(pin, INPUT);
}

