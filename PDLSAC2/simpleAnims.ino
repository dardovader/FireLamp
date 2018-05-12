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



void faro(byte tono)
{
  //difuminarTorres();
  static byte t = 0;
  static byte b = 0;
  for (byte i=1; i<NUM_LEDS_TORRE; i++)
  {
     switch (t)
     {
        case 0:
          ledsTorre_1[i]=CHSV(tono,255,b);
          ledsTorre_4[i]=CHSV(tono,255,255-b);
          break;
        case 1:
          ledsTorre_2[i]=CHSV(tono,255,b);
          ledsTorre_1[i]=CHSV(tono,255,255-b);
          break;
        case 2:
          ledsTorre_3[i]=CHSV(tono,255,b);
          ledsTorre_2[i]=CHSV(tono,255,255-b);
          break;
        case 3:
          ledsTorre_4[i]=CHSV(tono,255,b);
          ledsTorre_3[i]=CHSV(tono,255,255-b);
          break;
        default:
          t=0;
          break;  
     }
  }
  //static byte v=map(CENTELLEO,0,255,1,4);
  //b=b+v;
  b++;
  if (b>=255)
  {
    b=0;
    t++;
    if (t>=4)
    {
      t=0;
    }
  } 
}

void difuminarTorres(byte n)
{
  for (byte i=0; i<NUM_LEDS_TORRE;i++)
  {
    ledsTorre_1[i].nscale8(n);
    ledsTorre_2[i].nscale8(n);
    ledsTorre_3[i].nscale8(n);
    ledsTorre_4[i].nscale8(n);
  }
}


