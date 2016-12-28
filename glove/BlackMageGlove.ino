#include <AlaLed.h>
#include <Button.h>
#include "blm_animation_key.h"

#include <SPI.h>
#include "RF24.h"

#define BTNEXEC 4                 // Execute Action button
#define BTN1 5                    // Button 1 (small spell)
#define BTN2 6                    // Button 2 (big spell)
#define BTN3 7                    // Button 3 (limit break)

#define HAP 3                     // Haptic vibration motoro pin
#define EYE 10                    // Eye LED pin
#define RADIO 8,9                 // Radio control pins

int nextAction = ACT_NULL;
int lastAction = ACT_NULL;
int nextState = STATE_NULL;
int lastState = STATE_NULL;
int curState = STATE_OFF;
bool eyePrime = false;             // If true, turn the eye on when the staff turns on
bool eyeState = false;            // Current state of the eye, on or off

const int eyePrimeBtn = BTN3;           // Sets the button to use for priming the eye
const int eyeToggleBtn = BTN2;          // Sets teh button to use for toggling the eye on and off

int btnHold = 500;                // Time in ms a button needs to be held to trigger the hold action

int xmit;                         // Data transmission variable

int radioCH = 110;                // radio transmission channel.  Channels under 106 can experience WiFi interference.

int igniteSpeed = 300;            // Ignition animation speed

AlaLed hap;
AlaLed eye;

Button btnexec = Button(BTNEXEC, BUTTON_PULLUP_INTERNAL, true, 50);
Button btn1 = Button(BTN1, BUTTON_PULLUP_INTERNAL, true, 50);
Button btn2 = Button(BTN2, BUTTON_PULLUP_INTERNAL, true, 50);
Button btn3 = Button(BTN3, BUTTON_PULLUP_INTERNAL, true, 50);

AlaSeq seq1[] = {
  { ALA_ON, 0, 100 },
  { ALA_OFF, 0, 0},
  { ALA_ENDSEQ }
};

AlaSeq seq2[] = {
  { ALA_ON, 0, 100 },
  { ALA_OFF, 0, 100 },
  { ALA_ON, 0, 100 },
  { ALA_OFF, 0, 0 },
  { ALA_ENDSEQ }
};

AlaSeq seq3[] = {
  { ALA_ON, 0, 50 },
  { ALA_OFF, 0, 50 },
  { ALA_ON, 0, 50 },
  { ALA_OFF, 0, 50 },
  { ALA_ON, 0, 50 },
  { ALA_OFF, 0, 0 },
  { ALA_ENDSEQ }
};

AlaSeq seq4[] = {
  { ALA_ON, 0, 200 },
  { ALA_OFF, 0, 0},
  { ALA_ENDSEQ }
};

AlaSeq seqIgnite[] =
{
  { ALA_OFF, 0, 50 },
  { ALA_FADEIN, igniteSpeed, igniteSpeed },
  { ALA_ON, 0, 0 },
  { ALA_ENDSEQ }
};

AlaSeq seqExtinguish[]=
{
  { ALA_ON, 0, 50 },
  { ALA_FADEOUT, igniteSpeed, igniteSpeed },
  { ALA_OFF, 0, 0 },
  { ALA_ENDSEQ }
};

AlaSeq seqLimitExtinguish[]=
{
  { ALA_ON, 0, 5000 },
  { ALA_FADEOUT, igniteSpeed, igniteSpeed },
  { ALA_OFF, 0, 0 },
  { ALA_ENDSEQ }
};

RF24 radio(RADIO);

byte addr[][6] = {"BLM50"};           // Radio transmission address, should be 5 characters

//------------------------------------------------------------

void radioSend(int data)
{
  radio.write(&data, sizeof(data));
}

void onPress(Button& b)
{
  switch(b.pin)                                                         // Run code for which button was pressed
  {
    case BTNEXEC:
      switch (nextState)
      {
        case STATE_NULL:                          // No queued state
          if ((curState == STATE_EXTINGUISH) || (curState == STATE_OFF))  // If staff is off, turn on
          {
            curState = STATE_IGNITE;                // Current state is on
            nextState = STATE_NULL;                 // No queued state
            nextAction = ACT_NULL;                  // No queued action
            
            if ((eyePrime) && (!eyeState))             // If eye is set to follow staff and eye is off, turn on
            {
              eye.setAnimation(seqIgnite);
              eyeState = true;
              Serial.println("Eye follows on.");
            }

            Serial.print("State Ignite: ");
            Serial.println(STATE_IGNITE);
            radioSend(STATE_IGNITE);
          }else                                     // Staff is on, turn it off
          {
            curState = STATE_EXTINGUISH;            // Current state is off
            nextState = STATE_NULL;                 // No queued state
            nextAction = ACT_NULL;                  // No queued action

            if ((eyePrime) && (eyeState))              // If eye is set to follow staff and eye is on, turn off
            {
              Serial.println("Eye follows off.");
              eye.setAnimation(seqExtinguish);
              eyeState = false;
            }

            Serial.print("State Extinguish: ");
            Serial.println(STATE_EXTINGUISH);
            radioSend(STATE_EXTINGUISH);
          }
          break;

        case STATE_SMALLCAST:                      // Small spell selector
          curState = STATE_IDLE;
          nextState = STATE_NULL;
          nextAction = ACT_SMALLSPELL;
          Serial.println("State small spell cast.");
          radioSend(STATE_SMALLCAST);
          break;

        case STATE_BIGCAST:
          curState = STATE_IDLE;
          nextState = STATE_NULL;
          nextAction = ACT_BIGSPELL;
          Serial.println("State big spell cast.");
          radioSend(STATE_BIGCAST);
          break;

        case STATE_LIMITCAST:
          curState = STATE_EXTINGUISH;
          nextState = STATE_NULL;
          nextAction = ACT_LIMITSPELL;
          Serial.println("State limit break cast.");
          radioSend(STATE_LIMITCAST);
          break;
      }
      break;

    case BTN1:
      lastState = nextState;                        // Save the current settings in case a long press is performed instead.
      lastAction = nextAction;
        
      if (nextState == STATE_SMALLCAST)
      {
        nextAction = ACT_NULL;
        nextState = STATE_IDLE;
        hap.setAnimation(seq2);
      }else
      {
        nextState = STATE_SMALLCAST;
        nextAction = ACT_SMALLSPELL;
        hap.setAnimation(seq1);
      }
      break;

    case BTN2:
      lastState = nextState;                        // Save the current settings in case a long press is performed instead.
      lastAction = nextAction;
        
      if (nextState == STATE_BIGCAST)
      {
        nextAction = ACT_NULL;
        nextState = STATE_IDLE;
        hap.setAnimation(seq2);
      }else
      {
        nextState = STATE_BIGCAST;
        nextAction = ACT_BIGSPELL;
        hap.setAnimation(seq1);
      }
      break;

    case BTN3:
      lastState = nextState;                        // Save the current settings in case a long press is performed instead.
      lastAction = nextAction;
        
      if (nextState == STATE_LIMITCAST)
      {
        nextAction = ACT_NULL;
        nextState = STATE_IDLE;
        hap.setAnimation(seq2);
      }else
      {
        nextState = STATE_LIMITCAST; 
        nextAction = ACT_LIMITSPELL;
        hap.setAnimation(seq1);
      }
      break;
  }
}

void onRelease(Button& b)
{
  switch(nextAction)
  {
    case ACT_SMALLSPELL:
      Serial.println("Small Spell finale.");
      nextAction = ACT_NULL;
      curState = STATE_IDLE;
      nextState = STATE_NULL;
      hap.setAnimation(seq4);
      radioSend(ACT_SMALLSPELL);
      break;

    case ACT_BIGSPELL:
      Serial.println("Big Spell finale.");
      nextAction = ACT_NULL;
      curState = STATE_IDLE;
      nextState = STATE_NULL;
      hap.setAnimation(seq4);
      radioSend(ACT_BIGSPELL);
      break;

    case ACT_LIMITSPELL:
      Serial.println("Limit Break finale.");
      nextAction = ACT_NULL;
      curState = STATE_EXTINGUISH;
      nextState = STATE_NULL;
      hap.setAnimation(seq4);
      radioSend(ACT_LIMITSPELL);
      
      if (eyePrime && eyeState)
      {
        eye.setAnimation(seqLimitExtinguish);
        eyeState = false;
      }
      break;
  }
  
}

void onHold(Button& b)
{
  nextState = lastState;              // Reset the State and Action variables since Press will have been triggered.
  nextAction = lastAction;
  
  switch (b.pin)
  {
    case eyePrimeBtn:                 // The Eye Prime button was held
      eyePrime = !eyePrime;           // Invert the eyePrime variable
      Serial.print("Eye Prime set to ");
      if (eyePrime)
      {
        Serial.println("TRUE.");
        // set the haptic notifier
        hap.setAnimation(seq3);       // 3 short ticks to indicate the primed eye
      }else
      {
        Serial.println("FALSE.");
        // set the haptic notifier
        hap.setAnimation(seq2);       // 2 short ticks to indicate prime clear
      }
      
      break;

    case eyeToggleBtn:
      eyeState = !eyeState;           // Invert the eyeState variable
      Serial.print("Eye State set to ");
      if (eyeState)                   // If the eye is on, turn on
      {
        eye.setAnimation(seqIgnite);      // Turn on the eye
        hap.setAnimation(seq4);           // One long tick to indicate eye on
        Serial.println ("TRUE.");
      }else                               // If the eye is off, turn off
      {
        eye.setAnimation(seqExtinguish);  // Turn off the eye
        hap.setAnimation(seq2);           // 2 short ticks to indicate eye off
        Serial.println("FALSE.");
      }
      break;
  }
}

void setup() {
  // Pin assignment
  pinMode(BTNEXEC, INPUT_PULLUP);
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);

  pinMode (HAP, OUTPUT);
  pinMode (EYE, OUTPUT);

  // haptic and eye initialization
  hap.initPWM(HAP);
  hap.setAnimation(seq3);

  eye.initPWM(EYE);
  eye.setAnimation(ALA_OFF,0);

  

  nextAction = ACT_NULL;
  nextState = STATE_NULL;
  lastState = STATE_NULL;
  curState = STATE_OFF;

  // button event handlers
  btnexec.pressHandler(onPress);
  btnexec.releaseHandler(onRelease);

  btn1.pressHandler(onPress);
  btn2.pressHandler(onPress);
  btn3.pressHandler(onPress);

  btn2.holdHandler(onHold, btnHold);
  btn3.holdHandler(onHold, btnHold);

  Serial.begin(19200);              // Output to the serial console for debug purposes.

  // Radio init
  radio.begin();                    // starts radio m odule
  radio.setPALevel(RF24_PA_HIGH);   // sets the power amplifier mode
  radio.setChannel(radioCH);        // sets the transceiver channel
  radio.setRetries(15,15);          // 15 retries, 4ms (250us * 15) delay
  radio.openWritingPipe(addr[0]);   // Open the writing pipe on the assigned address
}

void loop() {
  // put your main code here, to run repeatedly:
  hap.runAnimation();
  eye.runAnimation();
  btnexec.process();
  btn1.process();
  btn2.process();
  btn3.process();
}
