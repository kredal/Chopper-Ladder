#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h> 
#define N_LEDS 7
#define PIN     6
SoftwareSerial mySerial(4, 5); //rx, tx
//const int buttonPin = 2;     // the number of the pushbutton pin

// variables will change:
//int buttonState = 0;         // variable for reading the pushbutton status
//int lastButtonState = LOW;
//unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
//unsigned long debounceDelay = 100;  

uint32_t outputStyle = 4; //defines the starting style


uint32_t Wheel(uint16_t WheelPos);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
 
void setup() {
  strip.begin();

  Serial.begin(9600);
//    pinMode(buttonPin, INPUT);

}
 
int pos = 0, dir = 1; // Position, direction of "eye"
 
void loop() {
if (mySerial.available() > 0) {
byte incomingByte = mySerial.read();
Serial.println(incomingByte);
    // if it's a capital H (ASCII 72), turn on the LED:

    if (incomingByte == 'N') { //next
      outputStyle++;
      Serial.println(outputStyle); 
    }
 }
/*  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
      outputStyle++;
      Serial.println(outputStyle);
      }
    }
  } */
//       Serial.println(outputStyle); 
switch (outputStyle) { 
  case 0:
    LarsonScanner(255,0,0,150); //G, R, B, delay, frames
  break;
  case 1:
    LarsonScanner(0,255,0,150); //G, R, B, delay, frames
  break;
  case 2:
    LarsonScanner(0,255,255,150); //G, R, B, delay, frames
  break;
  case 3:
    LarsonScanner(255,255,255,60); //G, R, B, delay, frames
  break;
  case 4:
    rainbowCycle(20);
  break;
  case 5:             //If you add more cases, make sure the bottom one always resets outputStyle to 0.
    outputStyle = 0;
  break;
}  

}
 
  
void LarsonScanner(uint8_t g, uint8_t r, uint8_t b,uint8_t wait) {  
//uint8_t n = 0;  //counts frames so far
for(uint8_t s=0; s <= strip.numPixels(); s++) strip.setPixelColor(s, g/4, r/4, b/4);  //set everything to dark red to start
strip.show(); //show the dark red I just set
//  while (n < frames){
    int j;
    strip.setPixelColor(pos, g, r, b); // Center pixel is brightest
 
    strip.show();
    delay(wait);
 
    // Rather than being sneaky and erasing just the tail pixel,
    // it's easier to erase it all and draw a new one next time.
  for(uint8_t s=0; s <= strip.numPixels(); s++) strip.setPixelColor(s, g/4, r/4, b/4);
 
    // Bounce off ends of strip
    pos += dir;
    if(pos < 0) {
      pos = 1;
      dir = -dir;
    } else if(pos >= strip.numPixels()) {
      pos = strip.numPixels() - 2;
      dir = -dir;
    }
//  n++; //add frame to the count
//  }
}
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j=0; j < 384 *1 ; j++) {     // 1 cycles of all 384 colors in the wheel
    for (i=0; i < (strip.numPixels()*10); i++) {
      // tricky math! we use each pixel as a fraction of the full 384-color
      // wheel (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 384 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel(((i * 384 / (strip.numPixels()*10)) + j) % 384));
    }
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

void changeOutputStyle() {
  outputStyle++;
}

uint32_t Wheel(uint16_t WheelPos)
{
  byte r, g, b;
  switch(WheelPos / 128)
  {
    case 0:
      r = 127 - WheelPos % 128; // red down
      g = WheelPos % 128;       // green up
      b = 0;                    // blue off
      break;
    case 1:
      g = 127 - WheelPos % 128; // green down
      b = WheelPos % 128;       // blue up
      r = 0;                    // red off
      break;
    case 2:
      b = 127 - WheelPos % 128; // blue down
      r = WheelPos % 128;       // red up
      g = 0;                    // green off
      break;
  }
  return(strip.Color(r,g,b));
}
