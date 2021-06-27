#include "mega_space_utils.h"

void testToggles() {
    int CNT = sizeof(toggles)/sizeof(int);
    int states[CNT];

    for (int i=0; i < CNT; i++) {
        states[i] = digitalRead(toggles[i]);
    }
    for (int i=0; i < CNT; i++) {
            int curr = digitalRead(toggles[i]);
            if (curr != states[i]) {
                Serial.println("Flipped toggle "+String(toggles[i]) + " to "+ String(curr));
                states[i] = curr;
            }

            digitalWrite(leds[i], curr);
    }


}

void testSliders() {
    int CNT = sizeof(sliders)/sizeof(int);
    
    float reads[CNT];

    String retStr = "Slider states:";

    for (int i=0; i < CNT; i++) {
        reads[i] = analogRead(sliders[i]);
        retStr+=" ["+String(sliders[i]) + "] " + String(reads[i]);
    }

    Serial.println(retStr);
}

void testPotents() {
  int CNT = sizeof(potent)/sizeof(int);
    
    float reads[CNT];

    String retStr = "Pot states:";

    for (int i=0; i < CNT; i++) {
        reads[i] = analogRead(potent[i]);
        retStr+=" ["+String(potent[i]) + "] " + String(reads[i]);
    }

    Serial.println(retStr);

}

void testLeds() {
    for (int i=0; i<sizeof(leds)/sizeof(int); i++) {
        digitalWrite(leds[i],HIGH);
        waitForButtonPressAndRelease();
        digitalWrite(leds[i],LOW);
    }
}

void testLeds2() {
    FOR(i,10) {
        digitalWrite(leds[i], HIGH);
        delay(500);
        digitalWrite(leds[i], LOW);
    }
}

void waitForButtonPressAndRelease() {
    while(digitalRead(BUTTONPIN)==HIGH) {
        continue;
    }
    while(digitalRead(BUTTONPIN)==LOW) {
        continue;
    }
    return;
}

String arrayToString(int arr[], int n) {
    String s = "[";
    FOR(i,n) {
        s+=String(arr[i]);
        if (i < n-1) s+=", ";
    }       
    s+="]";
    return s;
}

String arrayToString(float arr[], int n) {
    String s = "[";
    FOR(i,n) {
        s+=String(arr[i],2);
        if (i < n-1) s+=", ";
    }       
    s+="]";
    return s;
}

void DebugLog(String str) {
  #ifdef DEBUG
  Serial.println("DEBUG: "+str+"("+String(millis()/1000)+" seconds");
  #endif //ifdef DEBUG
}

void readArrayD(int outArr[], int pins[], int n) {
    FOR(i,n) {
        outArr[i] = digitalRead(pins[i]);
    }
}

void readArrayA(int outArr[], int pins[], int n){
    FOR(i,n) {
        outArr[i] = analogRead(pins[i]);
    }
}

void writeArrayD(int vals[], int arr[], int n) {
    FOR(i,n) {
        digitalWrite(arr[i], vals[i]);
    }
}