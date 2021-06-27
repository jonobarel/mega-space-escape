#ifndef SETUP_INCLUDED
#define SETUP_INCLUDED
#define DEBUG true
#define BUTTONPIN 32

#define ANALOG 0
#define DIGITAL 1

#define SYS_POWER 0
#define SYS_ENG 1
#define SYS_SHIELDS 2
#define SYS_IMPULSE 3
#define SYS_FTL 4
#define PRESSED HIGH
#define NOTPRESSED LOW
#define FOR(i,x) for(int i=0; i<x; i++)
#define ARRSIZE(arrName, arrType) sizeof(arrName)/sizeof(arrType)
#define MAXSLIDER 1000
#define MINSLIDER 20

/******************
 * Audio tracks
 * ******************/

#define ENGINETURN 1
#define ENGINEHUM 2
#define PEW 3
#define PROPELLER 4
#define PROPELLER2 5


enum eStatus {Off,On,Damaged,Disabled};

struct Input {
    int pin;
    int type;
    float analog_read;
    int digital_read;
};

int buttons[] = {31,32,33};
int toggles[] = {37,38,39};
int sliders[] = {A0, A1, A2};

int thrusterToggles[] = {41,42,43,44};
int thrusterToggleLights[] = {45,46,47,48};

int potent[] = {A8, A9, A10, A11};
int engines[] = {buttons[0], buttons[2]};
int flight[] = {sliders[2], sliders[3], potent[2]};


/*
A8 - 10-1000
A9 - ~30-1000
A10 - ~50- 1000
A11 -~60 - 1000


*/
int leds[] = {49,50,51,52,53,43,44,45,46,47}; //size = 10

#endif //ifndef SETUP_INCLUDED
