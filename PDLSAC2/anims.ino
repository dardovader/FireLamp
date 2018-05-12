void FireRing60()
{
  static byte heat[NUM_LEDS_60];

  for ( byte i = 0; i < NUM_LEDS_60; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((ENFRIAMIENTO * 10) / NUM_LEDS_60) + 2));
  }

  if ( random8() < CENTELLEO ) {
    byte y = random8(NUM_LEDS_60 - 1);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  for ( byte j = 0; j < NUM_LEDS_60; j++) {
    byte colorindex = scale8( heat[j], 240);
    CRGB color = ColorFromPalette( paleta, colorindex);
    byte pixelnumber;
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


  for ( byte i = 0; i < NUM_LEDS_16; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((ENFRIAMIENTO * 10) / NUM_LEDS_24) + 2));
  }


  if ( random8() < CENTELLEO ) {
    byte y = random8(NUM_LEDS_16 - 1);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  for ( byte j = 0; j < NUM_LEDS_16; j++) {
    byte colorindex = scale8( heat[j], 240);
    CRGB color = ColorFromPalette( paleta, colorindex);
    byte pixelnumber;
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

  for ( byte i = 0; i < NUM_LEDS_TORRE; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((ENFRIAMIENTO * 10) / NUM_LEDS_TORRE) + 2));
  }

  for ( byte k = NUM_LEDS_TORRE - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }

  if ( random8() < CENTELLEO ) {
    byte y = random8(7);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  for ( byte j = 0; j < NUM_LEDS_TORRE; j++) {
    byte colorindex = scale8( heat[j], 240);
    CRGB color = ColorFromPalette( paleta, colorindex);
    byte pixelnumber;
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

void magia()
{
/*  for (byte i=0; i<NUM_LEDS_TORRE; i++)
  {
    ledsTorre_1[i]=qsub8(ledsTorre_1[i],random8(0,ENFRIAMIENTO));
    ledsTorre_2[i]=qsub8(ledsTorre_2[i],random8(0,ENFRIAMIENTO));
    ledsTorre_3[i]=qsub8(ledsTorre_3[i],random8(0,ENFRIAMIENTO));
    ledsTorre_4[i]=qsub8(ledsTorre_4[i],random8(0,ENFRIAMIENTO));
  }
*/
  difuminarTorres(ENFRIAMIENTO);
  if (random8() <CENTELLEO)
  {
    byte n= random8(0,4);
    switch(n)
    {
      case 0: ledsTorre_1[random8(0,NUM_LEDS_TORRE)]=CHSV(random8(),255,255); break;
      case 1: ledsTorre_2[random8(0,NUM_LEDS_TORRE)]=CHSV(random8(),255,255); break;
      case 2: ledsTorre_3[random8(0,NUM_LEDS_TORRE)]=CHSV(random8(),255,255); break;
      case 3: ledsTorre_4[random8(0,NUM_LEDS_TORRE)]=CHSV(random8(),255,255); break;
    }
  }
}

void larson(byte t)
{
  difuminarTorres(ENFRIAMIENTO);

  static byte i=0;
  static boolean subida = true;
    ledsTorre_1[i]=CHSV(t,255,255);
    ledsTorre_2[i]=CHSV(t,255,255);
    ledsTorre_3[i]=CHSV(t,255,255);
    ledsTorre_4[i]=CHSV(t,255,255);

 if (subida)
 {
  if (i<NUM_LEDS_TORRE-1)
  {
    i++;
  }
  else
  {
    subida=false;
    i--;
  }
 }
 else
 {
  if (i>0)
  {
    i--;
  }
  else
  {
    subida=true;
    i++;
  }
 }
}


void larsonDesfasdo(byte t)
{
  difuminarTorres(ENFRIAMIENTO);

  static byte i=0;
  static boolean subida = true;
    ledsTorre_1[i]=CHSV(t,255,255);
    ledsTorre_2[i]=CHSV(t+64,255,255);
    ledsTorre_3[i]=CHSV(t+128,255,255);
    ledsTorre_4[i]=CHSV(t+196,255,255);

 if (subida)
 {
  if (i<NUM_LEDS_TORRE-1)
  {
    i++;
  }
  else
  {
    subida=false;
    i--;
  }
 }
 else
 {
  if (i>0)
  {
    i--;
  }
  else
  {
    subida=true;
    i++;
  }
 }
}
