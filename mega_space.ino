#include "setup.h"
#include "mega_space_utils.h"
#include <Metro.h>
#include <wavTrigger.h>

//#define TESTMODE

//possible system statuses during the game
eStatus systemStates[] = {Off, Off, Off, Off, Off};

int levels[] = {0,1,2,3,4};
int curr_level = 0;
bool gameOver = false;
bool testMode = false;

wavTrigger wTrig;
int  gSeqState = 0;           // Main program sequencer state
int  gRateOffset = 0;         // WAV Trigger sample-rate offset



void setup() {
    Serial.begin(9600);
    InitInputs();
    
    Serial.println("Finished setup");

  // WAV Trigger startup at 57600
  wTrig.start();
  delay(10);
  
  char version[20];

  // Send a stop-all command and reset the sample-rate offset, in case we have
  //  reset while the WAV Trigger was already playing.
  wTrig.stopAllTracks();
  wTrig.masterGain(0);
  wTrig.flush();
  wTrig.samplerateOffset(0);
  wTrig.getVersion(version,20);
  DebugLog("Wav Trigger version: " + String(version));
  DebugLog("Wav Trigger tracks: " + String(wTrig.getNumTracks()));
  wTrig.setReporting(true);
  int n = wTrig.getNumTracks();
  FOR(i,n) {
      wTrig.trackGain(i,0);
  }
  if (digitalRead(buttons[2])==HIGH) testMode = true;
}

void loop() {
    if (!testMode) {
        //DebugLog("Starting loop()");

        /*TODO: intro audio 
        "You made it out! Great, we have an extraction team on the way but you're going to have to pilot this ship.
        Our scientists have provided some insights, we have a general idea  how to do it but you're going to have to figure out the controls.
        I'll be your guide"
        */
        

        /*Tutorial Missions*/
        //Mission 1 - start power
        PowerUp();
        //TogglePuzzle();
        //Mission 2 - enable flight by spinning up the engines.
        TestFlight();
    }

    else { //if !testmode
        testLeds2();
    }   
}

void InitInputs() {
    ArrayPinMode(toggles, ARRSIZE(toggles, int), INPUT);
    ArrayPinMode(buttons, ARRSIZE(toggles, int), INPUT);
}

void ArrayPinMode(int arr[], int n, int mode) {
    for (int i = 0; i < n; i++) {
        pinMode(arr[i], mode);
    }
}

void _readInput(Input *s) {
    if (s->type == DIGITAL) {
       s->digital_read = digitalRead(s->pin); 
    }
    else {
        s->analog_read = analogRead(s->pin);
    }
}

void PowerUp() {
    Serial.println("Mission start: PowerUp()");
    bool audioPlaying = false;
    int cntEngines = ARRSIZE(engines, int);
        int engineButtonStates[cntEngines];
        int actives = 0;

    /* In this mission, players need to find the power controls on each of their systems */
    while(true) {
        actives = 0;
        for (int i=0; i < cntEngines; i++) {
            int isPressed = digitalRead(engines[i]);
            engineButtonStates[i] = isPressed;
            if (isPressed == PRESSED) actives++;
            
        }

        //DebugLog("Button states: "+arrayToString(engineButtonStates, ARRSIZE(engines,int)));
        if (actives == 0) {
            digitalWrite(leds[SYS_ENG], LOW);
            wTrig.stopAllTracks();
            audioPlaying = false;
        }
        else if (actives < cntEngines) { //haven't activated everything yet
            /*if not playing Play Engine stutter noise*/
            if (!audioPlaying) {
                DebugLog("Playing track 1");
                wTrig.trackLoop(ENGINETURN, true);
                wTrig.trackPlaySolo(ENGINETURN); 
                audioPlaying = true;
            }
            
            setFlicker(leds[SYS_ENG], 25,0);
            
        }
        else {
            digitalWrite(leds[SYS_ENG], HIGH);
            wTrig.stopAllTracks();
            wTrig.trackLoop(2, true);
            DebugLog("playing track 2");
            wTrig.trackGain(ENGINEHUM, -35);
            wTrig.trackPlayPoly(ENGINEHUM);
            //wTrig.trackFade(ENGINEHUM, 0, 3000,0);
            systemStates[SYS_ENG] = On;
            Serial.println("Mission accomplished: Powerup");
            return;
        }
    }
}

void setFlicker(int ledPin, int freq, int offset) {
    int is_flicker = ((millis()+offset)/(freq+1))%20;
    if (is_flicker == 2 || is_flicker == 5 || is_flicker == 7 || is_flicker == 13 || is_flicker == 19) {
        digitalWrite(ledPin, HIGH);
    }
    else digitalWrite(ledPin, LOW);
}

void TestFlight() {

    
    Serial.println("Mission start: engine test");
    /****************************************
     * Narration: "okay team, good work. You've got the core powered up. 
     * Now we need to warm up the engines.
     * The warm up is done by setting the 4 toggles to their correct positions. 
     * Once you've done that, find the thruster controls and set them to 100%.
     * 
    //The goal of this task is to power up the flight engines to full power, 
    //and then pull them back down to 75%
    ************************************/


    
    int n = ARRSIZE(flight, int);
    int throttleStates[n];
    float throttleRatios[n];

    bool toggledOn = false;
    int toggleLeds[] = {leds[6], leds[8], leds[9]}; //we're using these to signal the power distribution is correct.
    //TODO: replace these with dedicated leds, e.g. on the switches themselves.

    int ledVals[3];

    while(!toggledOn) {
        int toggleStates[3] = {0,0,0};
        readArrayD(toggleStates, toggles, 3);

        ledVals[0] = (toggleStates[1]+toggleStates[2])%2;
        ledVals[1] = (toggleStates[0] + toggleStates[1]+1)%2;
        ledVals[2] = (toggleStates[0]+toggleStates[2])%2;

        writeArrayD(ledVals, toggleLeds, 3);

        if (ledVals[0] == 1 && ledVals[1] == 1 && ledVals[2] == 1) { //all LEDs are lit!
            Serial.println("Power routed to engines!");
            toggledOn = true;
        }

    }


        /*******
         *  TODO: Audio narration: "Great! you've routed power to the thrusters, now we're going to need you to spin them up. 
         * There should be some dials or sliders in front of you. There should be THREE controls that drive the thrusters. See if you can find them."
         * ****/

    bool thrustersAtFull = false;
    bool thrusterAudioPlaying = false;

    while (!thrustersAtFull) {
            FOR(i,n) {
                throttleStates[i] = analogRead(flight[i]);
                throttleRatios[i] = float((throttleStates[i]-MINSLIDER))/(MAXSLIDER-MINSLIDER);
            }
        
            float gainFactor = (throttleRatios[0]+throttleRatios[1]+throttleRatios[2])/3;
            wTrig.trackGain(PROPELLER2, -70 + gainFactor*80);

            if (!thrusterAudioPlaying) {
                wTrig.trackPlayPoly(PROPELLER2);
                wTrig.trackLoop(PROPELLER2, true);
                thrusterAudioPlaying = true;
            }

            if (DEBUG && millis()%1000 == 0) {
                Serial.println("Gain factor: "+ String(gainFactor));
                Serial.println("Throttles: "+arrayToString(throttleStates,n));
                Serial.println("Throttles: "+arrayToString(throttleRatios,n));
            }
            
            //if (throttleRatios[0] >= 0.8 && throttleRatios[1] >= 0.8 && throttleRatios[2] >= 0.8) thrustersAtFull = true;
          
            delay(10); //this is necessary to maintain stability of the WAV trigger and reduce load on tx commands.
        }


}

void TogglePuzzle() {
    
    
    
    //while (true) {}

}