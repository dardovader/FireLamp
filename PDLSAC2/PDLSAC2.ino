#include <FastLED.h>
#include "DHT.h"
#include <Button.h>
#include <EEPROM.h>

#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#define MP3_SOUND_HOGUERA 1
#define MP3_SOUND_AGUA    2
#define MP3_SOUND_MAR     3



#define USE_TEMP_SENSOR false
#define USE_POT_FOR_PARAMS true
#define USAR_SONIDO false
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
#define BUTTON_PIN        2
#define SOFT_SERIALM_RX    6
#define SOFT_SERIALM_TX    7

SoftwareSerial miSerial(SOFT_SERIALM_RX, SOFT_SERIALM_TX); // RX, TX
DFRobotDFPlayerMini mp3;


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
#define FRAMES_PER_SECOND 100

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

#define  num_modos 11

bool gReverseDirection = false;

int t = 0;

unsigned long previousMillisTemp = 0;

byte melodia, nueva_melodia = 0;
void setup()
{
  delay(2000);
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
  Serial.println(F("It's showtime!"));
  modo = EEPROM.read(MEM_ADDRESS);
  if (USAR_SONIDO)
  {
    Serial.print("Iniciado sistema de sonido");
    if (!mp3.begin(miSerial))
    {
      Serial.println(F("Unable to begin:"));
      Serial.println(F("1.Please recheck the connection!"));
      Serial.println(F("2.Please insert the SD card!"));
      while(true);
    }
  }
  
}

void loop() {
  //Serial.print(F("modo "));Serial.println(modo);
  random16_add_entropy(random());
if (botoncete.released())
  {
    modo++;
    if (modo >= num_modos)
    {
      modo=0;
    }
    EEPROM.update(MEM_ADDRESS, modo);
  }

  unsigned long currentMillisTemp = millis();
  static uint8_t tono = 0;
  switch (modo) {
    case 0:  // Fuego normal
      //Serial.println("dentro de modo 0");
      paleta = HeatColors_p;
      FireRing60();
      FireRing24();
      FireRing16();
      TORRES_IGUALES = true;
      pintarTorres();
      if (USAR_SONIDO)
      {
        tocarSonido(MP3_SOUND_HOGUERA);
      }
      break; 
    case 1:  // Fuego asimétrico
      //Serial.println("dentro de modo 1");
      paleta = HeatColors_p;
      FireRing60();
      FireRing24();
      FireRing16();
      TORRES_IGUALES = false;
      pintarTorres();
      if (USAR_SONIDO)
      {
        tocarSonido(MP3_SOUND_HOGUERA);
      }
      break;
    case 2:  // Fuego arcoiris
      //Serial.println("dentro de modo 2");
      tono++;
      colorOscuro  = CHSV(tono, 255, 192);
      colorClaro = CHSV(tono, 128, 255);if (USAR_SONIDO)
      paleta = CRGBPalette16( CRGB::Black, colorOscuro, colorClaro, CRGB::White);
      FireRing60();
      FireRing24();
      FireRing16();
      TORRES_IGUALES = true;
      pintarTorres();
      if (USAR_SONIDO)
      {
        tocarSonido(MP3_SOUND_HOGUERA);
      }
      break;
    case 3:  // Fuego segun temperatura
      //Serial.println("dentro de modo 3");
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
        if (USAR_SONIDO)
        {
          tocarSonido(MP3_SOUND_HOGUERA);
        }
      }
      else 
      {
        modo++;
      }
 
      break;
     case 4: // Static color
        TORRES_IGUALES = true;
        apagarAros();
        SolidColor();
        if (USAR_SONIDO)
        {
          tocarSonido(MP3_SOUND_HOGUERA);
        }
        break;
     case 5: //faro
       tono = CENTELLEO;
       TORRES_IGUALES = true;
       faro(tono);
       if (USAR_SONIDO)
       {
        tocarSonido(MP3_SOUND_MAR);
       }
       break;
     case 6: // faro arcoiris
       tono = t++;
       TORRES_IGUALES = true;
       faro(tono);
       if (USAR_SONIDO)
      {
        tocarSonido(MP3_SOUND_MAR);
      }
       break;
     case 7: // magia
       TORRES_IGUALES=false;
       magia();
       break;
     case 8: // larson
       TORRES_IGUALES=true;
       tono=CENTELLEO;
       larson(tono);
       break;
     case 9: // larson multicolor
       TORRES_IGUALES=true;
       tono++;
       larson(tono);
       break;
     case 10: // larson con desfase de color
       TORRES_IGUALES=false;
       tono++;
       larsonDesfasdo(tono);
       break;
  }

 
  
  if (USE_TEMP_SENSOR)
  {
    if (currentMillisTemp - previousMillisTemp >= DHT_REFRESH)
    {
      previousMillisTemp = currentMillisTemp;
      t = dht.readTemperature();
    }
    else t = 20;
    
  }

  if (USE_POT_FOR_PARAMS)
  {
    CENTELLEO = map(analogRead(PIN_CHISPORROTEO), 0, 1023, 0, 255);
    ENFRIAMIENTO = map(analogRead(PIN_FRIO_AMBIENTE), 0, 1023, 0, 255);
    brillo = map(analogRead(PIN_BRILLO_TOTAL), 0, 1023, 0, 255);
  }

  FastLED.show(brillo);
  FastLED.delay(1000 / FRAMES_PER_SECOND);

}

void tocarSonido(byte nueva)
{
  if (melodia!=nueva)
  {
    mp3.play(nueva);
    melodia=nueva;
  }
}

