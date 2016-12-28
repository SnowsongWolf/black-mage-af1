//#include <Button.h>
#include <AlaLedRgb.h>
#include "lights.h"
#include "blm_animation_key.h"

#include <SPI.h>
#include "RF24.h"

#define REDPIN 9
#define GREENPIN 10
#define BLUEPIN 11

#define RADIO 2,3

AlaLedRgb rgbLed;
RF24 radio(RADIO);

byte addr[][6] = {"BLM50"};           // Radio transmission address, should be 5 characters
int radioCH = 110;


int curSeq = 0;

int getRadio()
{
  int data = -1;
  while (radio.available())
  {
    radio.read(&data, sizeof(data));
    Serial.println(data);
  }

  return data;
}


void setup() {
  // put your setup code here, to run once:
  rgbLed.initBP(REDPIN, GREENPIN, BLUEPIN);
  //rgbLed.setRefreshRate(1);
  //rgbLed.setBrightness(0x2d421c);
  rgbLed.setBrightness(0x292929);
  //pinMode(buttonPin, INPUT_PULLUP);

  rgbLed.setAnimation(ALA_OFF, 0, off);

  //Serial.begin(19200);
  //Serial.println("ready");
  //btnChange.pressHandler(onPress);

  radio.begin();                    // starts radio m odule
  radio.setPALevel(RF24_PA_HIGH);   // sets the power amplifier mode
  radio.setChannel(radioCH);        // sets the transceiver channel
  radio.setRetries(15,15);          // 15 retries, 4ms (250us * 15) delay
  radio.openReadingPipe(1,addr[0]);   // Open the writing pipe on the assigned address

  radio.startListening();
}

void loop() {
  //while (Serial.available())
  //{
  int ch = getRadio();
  
    //char ch = Serial.read();

    switch(ch)
    {
      case STATE_EXTINGUISH:
        rgbLed.setAnimation(seqExtinguish);
        break;

      case STATE_IGNITE:
        rgbLed.setAnimation(seqIgnite);
        break;

      case STATE_BIGCAST:
        rgbLed.setAnimation(seqCast);
        break;

      case STATE_SMALLCAST:
        rgbLed.setAnimation(seqCast);
        break;

      case STATE_LIMITCAST:
        rgbLed.setAnimation(seqLimitcast);
        break;

      case ACT_LIMITSPELL:
        rgbLed.setAnimation(seqLimitfinish);
        break;

      case ACT_BIGSPELL:
        rgbLed.setAnimation(seqBigfinish);
        break;

      case ACT_SMALLSPELL:
        rgbLed.setAnimation(seqSmallfinish);
        break;

      /*case 5:
        rgbLed.setAnimation(ALA_ON, 0, 0xffffff);
        break;*/
    }
  //}
  
  //btnChange.process();
  rgbLed.runAnimation();
}
