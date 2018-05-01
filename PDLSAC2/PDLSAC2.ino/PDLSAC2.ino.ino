#include <FastLED.h>
#include "DHT.h"
#include <Button.h>
#include <EEPROM.h>

#define USE_TEMP_SENSOR false
#define USE_POT_FOR_PARAMS false
boolean TORRES_IGUALES  = true;

// Pines digitales

#define LED_16_PIN        3
#define LED_24_PIN        4
#define LED_60_PIN        5
#define DHTPIN            8
#define LED_TORRE_PIN_4   9
#define LED_TORRE_PIN_3   10
#define LED_TORRE_PIN_2   11
#define LED_TORRE_PIN_1   12
#define BUTTON_PIN        13


// Pines Analógicos

#define PIN_CHISPORROTEO A0
#define PIN_FRIO_AMBIENTE A1
#define PIN_BRILLO_TOTAL  A2


#define NUM_LEDS_16    16
#define NUM_LEDS_24    24
#define NUM_LEDS_60    60
#define NUM_LEDS_TORRE 48

#define DHTTYPE DHT22
#define MIN_TEMP -10
#define MAX_TEMP 40

#define MEM_ADDRESS 0

byte brillo = 64;
#define FRAMES_PER_SECOND 60

#define DHT_REFRESH 300000 

Button botoncete(BUTTON_PIN);

CRGB colorOscuro;
CRGB colorClaro;


CRGB leds60[NUM_LEDS_60];
CRGB leds24[NUM_LEDS_24];
CRGB leds16[NUM_LEDS_16];
CRGB ledsTorre_1[NUM_LEDS_TORRE];
CRGB ledsTorre_2[NUM_LEDS_TORRE];
CRGB ledsTorre_3[NUM_LEDS_TORRE];
CRGB ledsTorre_4[NUM_LEDS_TORRE];
CRGBPalette16 paleta;
DHT dht(DHTPIN, DHTTYPE);

byte CENTELLEO = 200;
byte ENFRIAMIENTO = 55;
byte modo = 0;

#define  num_modos 4

bool gReverseDirection = false;

int t = 0;

unsigned long previousMillisTemp = 0;


void setup() {
  delay(3000);
  FastLED.addLeds<WS2812B, LED_60_PIN, GRB>(leds60, NUM_LEDS_60).setCorrection(TypicalSMD5050);
  FastLED.addLeds<WS2812B, LED_24_PIN, GRB>(leds24, NUM_LEDS_24).setCorrection(TypicalSMD5050);
  FastLED.addLeds<WS2812B, LED_16_PIN, GRB>(leds16, NUM_LEDS_16).setCorrection(TypicalSMD5050);
  FastLED.addLeds<WS2812B, LED_TORRE_PIN_1, GRB>(ledsTorre_1, NUM_LEDS_TORRE).setCorrection(TypicalSMD5050);
  FastLED.addLeds<WS2812B, LED_TORRE_PIN_2, GRB>(ledsTorre_2, NUM_LEDS_TORRE).setCorrection(TypicalSMD5050);
  FastLED.addLeds<WS2812B, LED_TORRE_PIN_3, GRB>(ledsTorre_3, NUM_LEDS_TORRE).setCorrection(TypicalSMD5050);
  FastLED.addLeds<WS2812B, LED_TORRE_PIN_4, GRB>(ledsTorre_4, NUM_LEDS_TORRE).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( brillo );
  t = dht.readTemperature();
  paleta = HeatColors_p;
  botoncete.begin();
  dht.begin();
  Serial.begin(115200);
  Serial.println("It's showtime!");
  modo = EEPROM.read(MEM_ADDRESS);
}

void loop() {
  random16_add_entropy(random());
  if (botoncete.released())
  {
    modo++;
    if (modo >= num_modos)
    {
      modo=0;
    }
    EEPROM.write(MEM_ADDRESS, modo);
  }
  
  unsigned long currentMillisTemp = millis();
  static uint8_t tono = 0;
  switch (modo) {
    case 0:  // Fuego normal
      paleta = HeatColors_p;
      FireRing60();
      FireRing24();
      FireRing16();
      TORRES_IGUALES = true;
      pintarTorres();
      break; 
    case 1:  // Fuego asimétrico
      paleta = HeatColors_p;
      FireRing60();
      FireRing24();
      FireRing16();
      TORRES_IGUALES = false;
      pintarTorres();
      break;
    case 2:  // Fuego arcoiris
      tono++;
      colorOscuro  = CHSV(tono, 255, 192);
      colorClaro = CHSV(tono, 128, 255);
      paleta = CRGBPalette16( CRGB::Black, colorOscuro, colorClaro, CRGB::White);
      FireRing60();
      FireRing24();
      FireRing16();
      TORRES_IGUALES = true;
      pintarTorres();
      break;
    case 3:  // Fuego segun temperatura
      if (USE_TEMP_SENSOR) {
        tono = map(t, MIN_TEMP, MAX_TEMP, 160, 0);
        colorOscuro  = CHSV(tono, 255, 192);
        colorClaro = CHSV(tono, 128, 255);
        paleta = CRGBPalette16( CRGB::Black, colorOscuro, colorClaro, CRGB::White);
        FireRing60();
        FireRing24();
        FireRing16();
        TORRES_IGUALES = true;
        pintarTorres();
      }
      break;
  }
  
  if (USE_TEMP_SENSOR)
  {
    if (currentMillisTemp - previousMillisTemp >= DHT_REFRESH) {
      previousMillisTemp = currentMillisTemp;
      t = dht.readTemperature();
    }
    else t = 20;
    
  }

  if (USE_POT_FOR_PARAMS) {
    CENTELLEO = map(analogRead(PIN_CHISPORROTEO), 0, 1023, 0, 255);
    ENFRIAMIENTO = map(analogRead(PIN_FRIO_AMBIENTE), 0, 1023, 0, 255);
    brillo = map(analogRead(PIN_BRILLO_TOTAL), 0, 1023, 0, 255);
  }

  FastLED.show(brillo);
  FastLED.delay(1000 / FRAMES_PER_SECOND);

}

void FireRing60()
{
  static byte heat[NUM_LEDS_60];

  for ( int i = 0; i < NUM_LEDS_60; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((ENFRIAMIENTO * 10) / NUM_LEDS_60) + 2));
  }

  if ( random8() < CENTELLEO ) {
    int y = random8(NUM_LEDS_60 - 1);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  for ( int j = 0; j < NUM_LEDS_60; j++) {
    byte colorindex = scale8( heat[j], 240);
    CRGB color = ColorFromPalette( paleta, colorindex);
    int pixelnumber;
    if ( gReverseDirection ) {
      pixelnumber = (NUM_LEDS_60 - 1) - j;
    } else {
      pixelnumber = j;
    }
    leds60[pixelnumber] = color;
  }
}

void FireRing24()
{
  static byte heat[NUM_LEDS_24];

  for ( byte i = 0; i < NUM_LEDS_24; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((ENFRIAMIENTO * 10) / NUM_LEDS_24) + 2));
  }

  if ( random8() < CENTELLEO ) {
    byte y = random8(NUM_LEDS_24 - 1);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  for ( int j = 0; j < NUM_LEDS_24; j++) {
    byte colorindex = scale8( heat[j], 240);
    CRGB color = ColorFromPalette( paleta, colorindex);
    byte pixelnumber;
    if ( gReverseDirection ) {
      pixelnumber = (NUM_LEDS_24 - 1) - j;
    } else {
      pixelnumber = j;
    }
    leds24[pixelnumber] = color;
  }
}
void FireRing16()
{
  static byte heat[NUM_LEDS_16];


  for ( int i = 0; i < NUM_LEDS_16; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((ENFRIAMIENTO * 10) / NUM_LEDS_24) + 2));
  }


  if ( random8() < CENTELLEO ) {
    int y = random8(NUM_LEDS_16 - 1);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  for ( int j = 0; j < NUM_LEDS_16; j++) {
    byte colorindex = scale8( heat[j], 240);
    CRGB color = ColorFromPalette( paleta, colorindex);
    int pixelnumber;
    if ( gReverseDirection ) {
      pixelnumber = (NUM_LEDS_16 - 1) - j;
    } else {
      pixelnumber = j;
    }
    leds16[pixelnumber] = color;
  }
}

void FireBar(byte nt)  // Mark Kriegsman's Fire2012 algorithm
{
  static byte heat[NUM_LEDS_TORRE];

  for ( int i = 0; i < NUM_LEDS_TORRE; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((ENFRIAMIENTO * 10) / NUM_LEDS_TORRE) + 2));
  }

  for ( int k = NUM_LEDS_TORRE - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }

  if ( random8() < CENTELLEO ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  for ( int j = 0; j < NUM_LEDS_TORRE; j++) {
    byte colorindex = scale8( heat[j], 240);
    CRGB color = ColorFromPalette( paleta, colorindex);
    int pixelnumber;
    if ( gReverseDirection ) {
      pixelnumber = (NUM_LEDS_TORRE - 1) - j;
    } else {
      pixelnumber = j;
    }

    switch (nt){
      case 0:
        ledsTorre_1[pixelnumber] = color;
        ledsTorre_2[pixelnumber] = color;
        ledsTorre_3[pixelnumber] = color;
        ledsTorre_4[pixelnumber] = color;      
        break;
      case 1:
        ledsTorre_1[pixelnumber] = color;
        break;
      case 2:
        ledsTorre_2[pixelnumber] = color;
        break;
      case 3:
        ledsTorre_3[pixelnumber] = color;
        break;
      case 4:
        ledsTorre_4[pixelnumber] = color;
      break;     
    }
  }
}

void pintarTorres()
{
  if (TORRES_IGUALES) 
  {
    FireBar(0);
  }
  else {
    FireBar(1);
    FireBar(2);
    FireBar(3);
    FireBar(4);
  }
}
