void apagarAros() {
  apagar60();
  apagar24();
  apagar16();
}


void apagar60()
{
  for (byte i=0; i<NUM_LEDS_60; i++)
  leds60[i] = CRGB(0,0,0);
}


void apagar24()
{
  for (byte i=0; i<NUM_LEDS_24; i++)
  leds24[i] = CRGB(0,0,0);
}


void apagar16()
{
  for (byte i=0; i<NUM_LEDS_16; i++)
  leds16[i] = CRGB(0,0,0);
}

