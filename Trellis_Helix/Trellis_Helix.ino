#include <Wire.h>
#include "Adafruit_Trellis.h"
#include <SoftwareSerial.h>
#include "MomentaryButton.h"

#define queryTime 15

SoftwareSerial midiSerial(2, 3);

unsigned long savedTime;
unsigned long updateTime;
int tempo = 10;
unsigned long flashClock;
int flashTime = 50;
int slotCount = 0;
int slotCount2 = 0;
int slotCount3 = 0;
int transSlotCount = 0;
int transSlotCount2 = 0;
int quantCount = 0;
int tempoVal = 0;
int transVal = 0;
int root = 0;
int oct = 0;
int mod = 0;
int rotCount = 0;
int noteOffVar = 0;
int polyQuantCount = 0;
int assignPos = 0;
int transGateIndex = 0;
int transTempoDivide = -1;
int transGateDivide = -1;
int transStep = 0;
int transFlashDivide = 0;
int avgVal;
boolean pitchDoor = false;
boolean transDoor = false;
boolean assignDoor = false;
boolean midiTrigger = false;
boolean chordTrigger = false;
boolean rotateFlag = true;// start as true
boolean randomFlag = false;
boolean progSwitch = true;
boolean patchFlag = false;
boolean transSwitch = false;
boolean polySwitch = false;
boolean susSwitch = false;
boolean transStepDoor = false;
boolean trellisUpdate = false;
//int polyGateIndex = 0;

const int readValLength = 10;
int rootReadVals[readValLength] = {0,0,0,0,0,0,0,0,0,0};
//byte trelMidiVal[32] = {32,33,34,36,38,40,42,44,46,68,70,72,74,76,78,60,82,83,84,86,38,40,42,44,46,68,70,72,74,76,78,60};
int pad1[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
int pad2[16] = {16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
int pad3[16] = {32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47};
int pad4[16] = {48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63};
int scale[12] = {0,1,2,3,4,5,6,7,8,9,10,11};
boolean on1[16] = {true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true};
boolean on2[16] = {true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true};
boolean on3[12] = {false,false,false,false,false,false,false,false,false,false,false,false};
boolean on4[16] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
boolean tempoGrid[16] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
boolean tempoGrid2[16] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
boolean tempoGrid3[16] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};

boolean transOn1[16] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
boolean transOn2[16] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
boolean transGrid[16] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
boolean transGrid2[16] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
boolean polyOn4[16] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};

boolean transAssign[16] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};

int transposeVal[16] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

boolean transGlow[16] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};

int randomGrid[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
int rotateGrid[10][16] = 
{{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{3,7,11,15,2,6,10,14,1,5,9,13,0,4,8,12},
{15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0},{12,8,4,0,13,9,5,1,14,10,6,2,15,11,7,3},
{1,2,3,0,5,6,7,4,9,10,11,8,13,14,15,12},{2,3,0,1,6,7,4,5,10,11,8,9,14,15,12,13},
{3,0,1,2,7,4,5,6,11,8,9,10,15,12,13,14},{0,5,10,15,4,9,14,3,8,13,2,7,12,1,6,11},
{0,1,5,4,2,3,7,6,8,9,13,12,10,11,15,14},{0,4,8,12,13,9,5,1,2,6,10,14,15,11,7,3}};
//int rotateA[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
//int rotateB[16] = {3,7,11,15,2,6,10,14,1,5,9,13,0,4,8,12};
//int rotateC[16] = {15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
//int rotateD[16] = {12,8,4,0,13,9,5,1,14,10,6,2,15,11,7,3};
int chamber[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int bucket[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int transpose[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
byte note = 0; 

//boolean polyMap[12] = 
int chord[12] = {0,0,0,0,0,0,0,0,0,0,0,0};

Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_Trellis matrix1 = Adafruit_Trellis();
Adafruit_Trellis matrix2 = Adafruit_Trellis();
Adafruit_Trellis matrix3 = Adafruit_Trellis();

Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0, &matrix1, &matrix2, &matrix3);

#define NUMTRELLIS 4

#define numKeys (NUMTRELLIS * 16)

#define INTPIN A2

#define PIN_BUTTON1 4
#define PIN_BUTTON2 6
#define PIN_BUTTON3 8

int transled = 5;
int polyled = 7;
int susled = 9;

MomentaryButton transbut(PIN_BUTTON1);
MomentaryButton polybut(PIN_BUTTON2);
MomentaryButton susbut(PIN_BUTTON3);

void setup() {

  Serial.begin(9600);
  Serial.println("Trellis Demo");
  midiSerial.begin(31250);
  flashClock = millis();
  savedTime = millis();
  updateTime = millis();
  // INT pin requires a pullup
  pinMode(INTPIN, INPUT);
  digitalWrite(INTPIN, HIGH);
  transbut.setup();
  polybut.setup();
  susbut.setup();
  pinMode(transled, OUTPUT);
  pinMode(polyled, OUTPUT);
  pinMode(susled, OUTPUT);
  
  trellis.begin(0x70, 0x71, 0x72, 0x74);

  // light up all the LEDs in order
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.setLED(i);
    trellis.writeDisplay();    
    delay(10);
  }  
}


void loop() {

  checkSwitches();
  
  int displayTime = int(millis() - flashClock);
  
  //delay(15); // 30ms delay is required, dont remove me!   <----- SUBTRACT this from tempo subdivision

  if(millis() - updateTime > queryTime){
    trellisUpdate = true;
    updateTime = millis();
  } else{
    trellisUpdate = false;
  }

  adjustClockRate(); 
  transposeRoot();
  oct = 0; //resets octave adjustment
  if(trellisUpdate){determineButtonState();} //trellis query
  advanceClockAndGate();  
  openPitchGate();
  advanceTransposeGate();
  pitchAssign();
  if(trellisUpdate){updateLEDs();} // trellis query
  //flashLEDs();
  if(displayTime > flashTime){
  for (uint8_t i=0 ; i<16 ; i++){
    if(!transSwitch){
      if(on1[i]){ // will turn back on or off at the display logic
        trellis.setLED(i);
      } else{
        trellis.clrLED(i);
      }
      if(transFlashDivide%2==0){
        if(on2[i]){ // will turn back on or off at the display logic
          trellis.setLED(pad2[i]);
        } else{
          trellis.clrLED(pad2[i]);
        }
      }
    } else{
      if(transOn1[i]){ // will turn back on or off at the display logic
        trellis.setLED(i);
      } else{
        trellis.clrLED(i);
      }
      if(transFlashDivide%2==0){
        if(transOn2[i]){
          trellis.setLED(pad2[i]);
        } else{
          trellis.clrLED(pad2[i]); 
        }

        if(transAssign[i]){
          trellis.setLED(pad4[i]);
        } else{
          trellis.clrLED(pad4[i]); 
        }
      }
    }
  }
  transFlashDivide++;
  flashClock = millis();
}

trellis.writeDisplay();

  rotateAssignments();
  midiSend();
}

void checkSwitches(){
  transbut.check();
  polybut.check();
  susbut.check();

  if(transbut.wasClicked()){
    transSwitch = !transSwitch;
    digitalWrite(transled, transSwitch);
  }
  
  if(polybut.wasClicked()){
    polySwitch = !polySwitch;
    digitalWrite(polyled, polySwitch);
  }
  
  if(susbut.wasClicked()){
    susSwitch = !susSwitch;
    digitalWrite(susled, susSwitch);
  }
}

void adjustClockRate(){
  tempoVal = analogRead(0);
  tempoVal = tempoVal/2;
  tempo = map(tempoVal, 0, 511,511,8);
}

void transposeRoot(){
  transVal = analogRead(1);
  analogReadAverage(transVal);
  transVal = avgVal/16;
  root = transVal; ////link with transpose pot
}

void analogReadAverage(int _anVal){
  for(int i = readValLength-1 ; i > 0 ; i--){
    rootReadVals[i] = rootReadVals[i-1];
  }
  rootReadVals[0] = _anVal;

  int sum;
  for(int i = 0 ; i< readValLength; i++){
    sum+=rootReadVals[i];
  }
  avgVal = sum/readValLength;
}

void determineButtonState(){
  if (trellis.readSwitches()) {
    // go through every button
    for (uint8_t i=0; i<16; i++) {
      // if it was pressed, turn it on
      if (trellis.justPressed(i)) {
        if(!transSwitch){
          if(on1[i]){
            on1[i] = false;
          }else{
            on1[i] = true; 
          }
        }else{
          if(transOn1[i]){
            transOn1[i] = false;
          }else{
            transOn1[i] = true; 
          }
        }
      }
    }
    for (uint8_t i=16; i<32; i++) {
      // if it was pressed, turn it on
      if (trellis.justPressed(i)) {
        if(!transSwitch){
          if(on2[i-16]){
        on2[i-16] = false;
          } else{
           on2[i-16] = true; 
          }
        }else{
          if(transOn2[i-16]){ 
            transOn2[i-16] = false;
          } else{
            transOn2[i-16] = true; 
          }
        }
      }
    }
          
    if (trellis.justPressed(32)){
      randomFlag = true;
      int r = 0;
      for (int i = 0 ; i < 16 ; i++){
      r = random(i,15);
      int temp = randomGrid[i];
      randomGrid[i] = randomGrid[r];
      randomGrid[r] = temp;
      }
    }
     
    if (trellis.justPressed(33)){
      randomFlag = false;
      rotCount++;
      if(rotCount>9){
        rotCount=0;
      } 
    }
      
    if (trellis.justPressed(34) && trellis.justPressed(35)){
      patchFlag = !patchFlag; ///<------ Patch flag is obsolete
    } else{
      if (trellis.justPressed(34)){
        mod -= 12;
      }
      if (trellis.justPressed(35)){
        mod += 12;
      }
    }
    
    for (uint8_t i=36; i<48; i++) {
      // if it was pressed, turn it on
      if (trellis.justPressed(i)) {
        if(!transSwitch){
          if(on3[i-36]){
        on3[i-36] = false;
          } else{
           on3[i-36] = true; 
          }
        }else{
  
          if(transposeVal[assignPos] != i-36){ //if you pick anything that is not currently assigned, then it's still true and reassign
            transAssign[assignPos] = true;
            transposeVal[assignPos] = i-36;
          } else if(transAssign[assignPos]) { ///if transAssign is true and we are hitting the same value as before, then make false clear it out
            transAssign[assignPos] = false;
            transposeVal[assignPos] = -1; ////jumps to -1 where nothing will see it!
          }
        }
      }
    }
    
    for (uint8_t i=48; i<64; i++) {
      // if it was pressed, turn it on
      if (trellis.justPressed(i)) {
        if(!transSwitch){
          if(on4[i-48]){
            on4[i-48] = false;
          } else{
            on4[i-48] = true; 
          }
        } else{
          assignPos=i-48; //can keep this for transpose
        }
      }
    }
  }
}

void advanceClockAndGate(){
  int passedTime = int(millis() - savedTime);
  if(passedTime > tempo){
    transTempoDivide++;
    if(slotCount>15){
      tempoGrid[15] = false;    
      slotCount = 0;  
    }
    
    tempoGrid[slotCount] = true;
    
    if(slotCount-1>=0){
      tempoGrid[slotCount-1] = false; 
    }
    
    slotCount++;
    
    if(!susSwitch){
      noteOff(0x80, noteOffVar, 0x40); //trigger!
    }
    
    savedTime=millis();
    
    if(transTempoDivide%2==0){
      if(transSlotCount>15){
         transGrid[15] = false;    
       transSlotCount = 0;  
      }

      transGrid[transSlotCount] = true;
    
      if(transSlotCount-1>=0){
        transGrid[transSlotCount-1] = false; 
      }
    
      transSlotCount++;
    
    }
  ///GATE Clock LOGIC///
    for (uint8_t i=0 ; i<16 ; i++){
      if(on1[i] && tempoGrid[i]){
        if(slotCount2>15){
          tempoGrid2[15] = false;    
          slotCount2 = 0;  
        }
      
        tempoGrid2[slotCount2] = true;
          
        if(slotCount2-1>=0){
          tempoGrid2[slotCount2-1] = false; 
        }

        slotCount2++;
        pitchDoor=true;
      }
    }
    
    if(transTempoDivide%2==0){  // could this be wrapped into the first modulo if statement?
      for (uint8_t i=0 ; i<16 ; i++){
        if(transOn1[i] && transGrid[i]){
          if(transSlotCount2>15){
            transGrid2[15] = false;    
            transSlotCount2 = 0;  
          }
        
          transGrid2[transSlotCount2] = true;
        
          if(transSlotCount2-1>=0){
            transGrid2[transSlotCount2-1] = false; 
          }
        transSlotCount2++;
        transDoor=true;
        }
      }
    }
  }
}

void openPitchGate(){
  int boop=0;
  if(pitchDoor){
    for (uint8_t i=0 ; i<16 ; i++){
      if(on2[i] && tempoGrid2[i]){   //if position i has the clock and is toggled on
        if(susSwitch){
          noteOff(0x80, noteOffVar, 0x40);
        }
        while(!on4[slotCount3%16]){ //checks to see if first position is on, if it's not, then....
          tempoGrid3[slotCount3%16] = false;
          slotCount3++;
          boop++;

          if(boop>15){
            boop=0;
            break;
          }
        }

        if(on4[slotCount3%16]){
          for(uint8_t i=0; i<16;i++){
            if(slotCount3%16 == i){ 
              tempoGrid3[i] = true;
              midiTrigger = true;     ///sets flag to play 1 midi note
            } else{
              tempoGrid3[i] = false;
            }
          }
        }
        slotCount3++;
      }
    }
  pitchDoor = false;
  }
}

void advanceTransposeGate(){
  if(transDoor){
    for (uint8_t i=0 ; i<16 ; i++){
      if(transOn2[i] && transGrid2[i] && transAssign[i]){ 
        transGateIndex = i;
        transStepDoor = true;
      }
    }
    transDoor = false;
  } 
}

void pitchAssign(){
  int x = 0;
  int n = 0;
  int ceiling = 0;
  
  if (transStepDoor){
   transStep = transposeVal[transGateIndex];
   transStepDoor = false;
  }

  while (x < 16){    /// fill up pitch assignment buckets
    if(on3[n]){    /// check current quant location to see if it's toggled, if it is...
      bucket[x] = root+scale[n]+oct+mod+transStep;  /// assign bucket val to these values 
      if(bucket[x] > 127){  /// checks to see if we are over 127 (max midi val), resets octave
        oct=0;
      }
      x++;  /// go to next bucket location
      ceiling = 0;  /// resets quant location counter 
    }
    n++;
    if(n>11){ ///if we have arrived at the end of the quant locations but still need to fill buckets
      n=0;
      oct+=12; 
    }
    ceiling++;  /// counts the quant locations, if 12 have been checked and none were true then quit
    if (ceiling>12){
      x=16; 
      for( uint8_t j = 0; j<16; j++){ /////////////////// <---THIS IS CLEARING OUT ALL the pitch assignments!!
        bucket[j] = 0;  
      }
    }  
  }
}

void updateLEDs(){
  //PAD1//
  if(!transSwitch){
    for (uint8_t i=0 ; i<16 ; i++){
      if (on1[i]){
        if(tempoGrid[i]){
          trellis.clrLED(pad1[i]); 
        } else{
          trellis.setLED(pad1[i]);
        }
      } 
      if (!on1[i]){
        if(tempoGrid[i]){
          trellis.setLED(pad1[i]);
        } else {
          trellis.clrLED(pad1[i]);
        }
      }
    }
  }else{
    for (uint8_t i=0 ; i<16 ; i++){
      if (transOn1[i]){
        if(transGrid[i]){
          trellis.clrLED(pad1[i]);  
        } else{
          trellis.setLED(pad1[i]);
        }
      }
      if (!transOn1[i]){
        if(transGrid[i]){
          trellis.setLED(pad1[i]);
        } else {
          trellis.clrLED(pad1[i]);
        }
      }
    }
  }
   //PAD2//
   
   if(!transSwitch){
   
       for (uint8_t i=0 ; i<16 ; i++){
         
       if (on2[i]){
         if(tempoGrid2[i]){
           trellis.clrLED(pad2[i]);
           
         } else{
           trellis.setLED(pad2[i]);
    
         }
       }
       if (!on2[i]){
         if(tempoGrid2[i]){
           trellis.setLED(pad2[i]);
     
         } else {
           trellis.clrLED(pad2[i]);
           }
         }
        }
   } else {
     for (uint8_t i=0 ; i<16 ; i++){
      if (transOn2[i]){
        if(transGrid2[i]){
           trellis.clrLED(pad2[i]);
           
         } else{
           trellis.setLED(pad2[i]);
    
         }
       }
       if (!transOn2[i]){
         if(transGrid2[i]){
           trellis.setLED(pad2[i]);
     
         } else {
           trellis.clrLED(pad2[i]);
           }
         }
     }
   }
   
   //PAD3//
   
   for (uint8_t i=0 ; i<12 ; i++){
     if(!transSwitch){
   if(on3[i]){
     trellis.setLED(pad3[i+4]);
   }  else{
     trellis.clrLED(pad3[i+4]);
   }
     } else {
       if((i==0) && !transAssign[assignPos]){ //if we're checking 1st position (index 0) and assign is false for that first position?
         trellis.clrLED(pad3[i+4]);
       } else if(transposeVal[assignPos] == i){

         trellis.setLED(pad3[i+4]);
         } else {
         trellis.clrLED(pad3[i+4]);
       }
    }
 }
   
   //PAD4//
   
    for (uint8_t i=0 ; i<16 ; i++){
        if(!transSwitch){
           if (on4[i]){
             if(tempoGrid3[i]){
               trellis.clrLED(pad4[i]);
               
             } else{
               
               trellis.setLED(pad4[i]);
        
             }
           }
           if (!on4[i]){
             if(tempoGrid3[i]){
               trellis.setLED(pad4[i]);
         
             } else {
               trellis.clrLED(pad4[i]);
             }
           }
    } else {
     for (uint8_t i=0 ; i<16 ; i++){
      if (transAssign[i]){
        if(transGrid2[i]){
           trellis.clrLED(pad4[i]);
           
         } else{
           trellis.setLED(pad4[i]);
    
         }
       }
       if (!transAssign[i]){
         if(transGrid2[i]){
           trellis.setLED(pad4[i]);
         } else {
           trellis.clrLED(pad4[i]);
           }
         }
      }
    }
  }
   
   
  trellis.writeDisplay();
}

void rotateAssignments(){
  for (uint8_t i=0 ; i<16 ; i++){
    if(randomFlag){
      chamber[i]= bucket[randomGrid[i]];
    }else if(rotateFlag){
      chamber[i]= bucket[rotateGrid[rotCount][i]];
    }
  }
}

void midiSend(){
    if(midiTrigger){
    for (uint8_t i=0 ; i<16 ; i++){
      if(on4[i] && tempoGrid3[i]){
        noteOn(0x90, chamber[i], 0x4F);
        noteOffVar = chamber[i];
      }
    }
    midiTrigger = false;
  }
}
  
void noteOn(byte cmd, byte data1, byte  data2) {
 midiSerial.write(cmd);
 midiSerial.write(data1);
 midiSerial.write(data2);
}

void noteOff(byte cmd, byte data1, byte  data2) {
 midiSerial.write(cmd);
 midiSerial.write(data1);
 midiSerial.write(data2);    
}

void progChange(byte cmd, byte data1) {
midiSerial.write(cmd);
midiSerial.write(data1);
}

