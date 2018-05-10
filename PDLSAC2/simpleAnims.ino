void SolidColor()
{
  for (byte i=0; i<NUM_LEDS_TORRE; i++)
  {
    ledsTorre_1[i]=CHSV(CENTELLEO,ENFRIAMIENTO,brillo);
    ledsTorre_2[i]=CHSV(CENTELLEO,ENFRIAMIENTO,brillo);
    ledsTorre_3[i]=CHSV(CENTELLEO,ENFRIAMIENTO,brillo);
    ledsTorre_4[i]=CHSV(CENTELLEO,ENFRIAMIENTO,brillo);
  }
}

