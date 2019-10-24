#include "Arduino.h"
#include "FastLED.h"
#define NUM_LEDS 144
CRGBArray<NUM_LEDS> leds;

//FIRE2012 defines and other stuff
bool gReverseDirection = false;
#define COOLING 55
#define SPARKING 120

#define POLICE_BLINK_DELAY 65

void setup() {
  FastLED.addLeds<NEOPIXEL,6>(leds, NUM_LEDS);
}

void policeBlink(){
  for(int i = 0; i< NUM_LEDS; i=i+8){
    leds[i] = CRGB::Red;
    leds[i+1] = CRGB::Red;
    leds[i+2] = CRGB::Red;
    leds[i+3] = CRGB::Red;
    leds[i+4] = CRGB::Blue;
    leds[i+5] = CRGB::Blue;
    leds[i+6] = CRGB::Blue;
    leds[i+7] = CRGB::Blue;
    
  }
  FastLED.delay(POLICE_BLINK_DELAY);
  leds = CRGB::Black;
  FastLED.delay(POLICE_BLINK_DELAY);

  for(int i = 0; i< NUM_LEDS; i=i+8){
    leds[i] = CRGB::Blue;
    leds[i+1] = CRGB::Blue;
    leds[i+2] = CRGB::Blue;
    leds[i+3] = CRGB::Blue;
    leds[i+4] = CRGB::Red;
    leds[i+5] = CRGB::Red;
    leds[i+6] = CRGB::Red;
    leds[i+7] = CRGB::Red;   
  }

  FastLED.delay(POLICE_BLINK_DELAY);
  leds = CRGB::Black;
  FastLED.delay(POLICE_BLINK_DELAY);
}

void policeBlinkAlternate(){
  leds = CRGB::Black;

  // flash right red
  for(int flashes = 0; flashes < 3; flashes++){
    // red on
    for(int i = 0; i< NUM_LEDS/2; i=i+1){
      leds[i] = CRGB::Red;
    }

    FastLED.delay(POLICE_BLINK_DELAY);

    //all off
    leds = CRGB::Black;

    FastLED.delay(POLICE_BLINK_DELAY);

  }

  // flash left blue
  for(int flashes = 0; flashes < 3; flashes++){
    // blue on
    for(int i = NUM_LEDS/2; i< NUM_LEDS; i=i+1){
      leds[i] = CRGB::Blue;
    }

    FastLED.delay(POLICE_BLINK_DELAY);

    //all off
    leds = CRGB::Black;

    FastLED.delay(POLICE_BLINK_DELAY);
  }
}

void blinker(){
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int hue = beatsin16(10,0,255);
  int pos = beatsin16(30, 0,NUM_LEDS-1);
  // leds = CHSV(30, 255, max(brightness,10));
  leds[pos] += CHSV(hue, 255, 255);
  FastLED.delay(5);
}

void hueCycle(){
  for(int baseHue = 0; baseHue < 255; baseHue ++){
    for(int led = 0; led < NUM_LEDS; led ++){
      leds[led] = CHSV(baseHue + (255/NUM_LEDS)*led, 255, 255);
    }
    FastLED.delay(20);
  }
}

// from here: https://github.com/FastLED/FastLED/blob/master/examples/Fire2012/Fire2012.ino
void Fire2012()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS/2];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS/2; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS/2) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS/2 - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS/2; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS/2 -1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
      // and other side
      leds[NUM_LEDS - pixelnumber] = color;
    }
}


// https://github.com/FastLED/FastLED/blob/master/examples/Cylon/Cylon.ino

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void cylon() { 
	static uint8_t hue = 0;
	Serial.print("x");
	// First slide the led in one direction
	for(int i = 0; i < NUM_LEDS; i++) {
		// Set the i'th led to red 
		leds[i] = CHSV(hue++, 255, 255);
		// Show the leds
		FastLED.show(); 
		// now that we've shown the leds, reset the i'th led to black
		// leds[i] = CRGB::Black;
		fadeall();
		// Wait a little bit before we loop around and do it again
		delay(10);
	}
	Serial.print("x");

	// Now go in the other direction.  
	for(int i = (NUM_LEDS)-1; i >= 0; i--) {
		// Set the i'th led to red 
		leds[i] = CHSV(hue++, 255, 255);
		// Show the leds
		FastLED.show();
		// now that we've shown the leds, reset the i'th led to black
		// leds[i] = CRGB::Black;
		fadeall();
		// Wait a little bit before we loop around and do it again
		delay(10);
	}
}

void randomNoise(){
  for(int led = 0; led < NUM_LEDS; led++){
    leds[led] = CHSV(random8(),255,80);
  }
  FastLED.delay(100);
}

void solidColorCycle(){
  for(int hue = 0; hue < 255; hue++){
    leds = CHSV(hue,0,255);
    FastLED.delay(40);
  }
}

void loop(){
  hueCycle();
}
