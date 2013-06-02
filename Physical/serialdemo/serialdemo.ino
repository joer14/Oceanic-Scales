
#include <Adafruit_WS2801.h>
#include "SPI.h"

#define DATA_PIN (2)
#define CLOCK_PIN (3)

#define LIGHT_COUNT (72)
#define SB_BUFFER_SIZE (64)
#define DEVICE_ID (1)
#define PARAMETER_MAX (255)

Adafruit_WS2801 strip = Adafruit_WS2801(LIGHT_COUNT, DATA_PIN, CLOCK_PIN);

uint32_t pixels[LIGHT_COUNT];

struct led
{
  int x;
  int y;
  //char r;
  //char g;
  //char b;
  char id;
  char type;
};

//#define pinModeFast(x, y) pinMode(x, y)
//#define digitalWriteFast(x, y) digitalWrite(x, y)
//SerialCommand SCmd;   // The demo SerialCommand object

//pseudo led class field values
/*
enum fields{
  X,
  Y,
  R,
  G,
  B,
  T   //0 for plankton, 1 for element
};
*/
//state values
enum states{
  RESTING,
  GAME,
  CALIBRATE
};

enum types{
  PLANKTON,
  TEMPERATURE,
  PH,
  NITROGEN
};

float windowY = 48;
float windowX = 26;

char sb [SB_BUFFER_SIZE]; //sb = serialbuffer
int sbpos = 0;

int state = 0;

led leds [LIGHT_COUNT];
int top = -1;
int buffer [LIGHT_COUNT];

int nitrogen = 0;
int ph = 0;
int temperature = 0;

int TIME = 0;

void setup() {
  
  Serial.begin(9600);

  //int leds[LIGHT_COUNT]; // (x,y,r,g,b)
  init_leds();
      
//  Serial.write("Set up!!!");
  //pinModeFast(DATA_PIN, OUTPUT);
  //pinModeFast(CLOCK_PIN, OUTPUT);
  //digitalWriteFast(DATA_PIN, LOW);
  //digitalWriteFast(CLOCK_PIN, LOW);
  Serial.println("Ready"); 
  
  strip.begin();

  // Update LED contents, to start they are all 'off'
  strip.show();

}


void loop() {
  //delay(1000);
  //Serial.println("hello");
  readNextCommand();
  //Serial.println(state);
  TIME = millis(); //will be changed to take time from dispatch arduino
  switch(state)
  {
    case RESTING: solid();
                  solid();
                  break;
    case    GAME: game();
                  break;
    case CALIBRATE: calibrate();
                  break;
    default     : tyker(1);
                  tyker(1);
  }
  
  /*
  while(!isEmpty())
  {
     //int popped = pop();
     //set_pixel_rgb(popped, leds[popped][B], leds[popped][G], leds[popped][R]);
     //strip.setPixelColor(popped, leds[popped].r, leds[popped].g, leds[popped].b);
  }
  */
  //show();
  //Serial.println("showing");
  strip.show();
}

void game()
{
   char balance = 0;
   for(int i = 0; i < LIGHT_COUNT; ++i) {
       switch (leds[i].type) {
           case TEMPERATURE:
               int d = abs(127 - TEMPERATURE);
               if (TEMPERATURE >= 127) {
                   double rTmpCalc = (double)(d / 128) * 15;
                   int rTmp = 255 - (int)rTmpCalc;
                   double gTmpCalc = (double)(d / 128) * 255;
                   int gTmp = 255 - (int)gTmpCalc;
                   double bTmpCalc = (double)(d / 128) * 200;
                   int bTmp = 255 - (int)bTmpCalc;
                   strip.setPixelColor(i, rTmp, gTmp, bTmp);
               }
               else if (TEMPERATURE < 127) {
                   double rTmpCalc = (double)(d / 128) * 255;
                   int rTmp = 255 - (int)rTmpCalc;
                   double gTmpCalc = (double)(d / 128) * 155;
                   int gTmp = 255 - (int)gTmpCalc;
                   double bTmpCalc = (double)(d / 128) * 55;
                   int bTmp = 255 - (int)bTmpCalc;
                   strip.setPixelColor(i, rTmp, gTmp, bTmp);
               }
               break;
           case NITROGEN:
               d = abs(127 - NITROGEN);
               if (NITROGEN >= 127) {
                   double rNCalc = (double)(d / 128) * 100;
                   int rN = 255 - (int)rNCalc;
                   double gNCalc = (double)(d / 128) * 155;
                   int gN = 255 - (int)gNCalc;
                   double bNCalc = (double)(d / 128) * 225;
                   int bN = 255 - (int)bNCalc;
                   strip.setPixelColor(i, rN, gN, bN);
               }
               else if (NITROGEN < 127) {
                   double rNCalc = (double)(d / 128) * 170;
                   int rN = 255 - (int)rNCalc;
                   double gNCalc = (double)(d / 128) * 100;
                   int gN = 255 - (int)gNCalc;
                   double bNCalc = (double)(d / 128) * 225;
                   int bN = 255 - (int)bNCalc;
                   strip.setPixelColor(i, rN, gN, bN);
               }
           case PH:
               d = abs(127 - PH);
               if (PH >= 127) {
                   strip.setPixelColor(i, 0, 255, 255);
               }
               else if (PH < 127) {
                   strip.setPixelColor(i, 0, 95, 95);
               }
           case PLANKTON:
               break;
           default:
               break;
       }
   }
}

void wipe()
{
  for(int i = 0; i < LIGHT_COUNT; ++i)
  {
   strip.setPixelColor(i, 0, 0, 0); 
  }
}

//shows BLUE
void solid()
{
  
   for(int i = 0; i < LIGHT_COUNT; ++i)
   {
     //if(leds[i][X] == 0 || leds[i][T] != type) continue;
     //leds[i].r = 0;
     //leds[i].g = 0;
     //leds[i].b = 255;
     //push(i);
     strip.setPixelColor(i, 0, 255, 0);
   }
   
}

//float counter = (millis() / 5000) % 400;
void tyker(int type)
{      
     delay(10);
     float control =  (TIME / 2000.0) ;
     float center = sin((PI / 2) * control) * windowY * 2 - (windowY / 2);
     for(int i = 0; i < LIGHT_COUNT; ++i)
     {
        if (leds[i].y == 0 || leds[i].type != type) continue;
        float distance = abs(leds[i].y - center);
        //Serial.println(distance);
        //int v = (int) ((distance / (windowX * 2)) * 255) + (int) (20 * (float) ((i % 7) - 3));
        int v = (int) ((distance / (windowY * 2)) * 127);
//       Serial.println(v);
        //leds[i].r = v;
        push(i);
     }
     
}

//waves on the beach
void tyker2(int type)
{      
//     Serial.println("Tyker");
     delay(10);
     float control =  (TIME / 5000.0) ;
     //Serial.println(counter);
     float center = sin(TWO_PI * control) * windowY;
     //Serial.print("center = ");
     //Serial.println(center);
     for(int i = 0; i < LIGHT_COUNT; ++i)
     {
        if (leds[i].y == 0 || leds[i].type != type) continue;
        float distance = abs(leds[i].y - center);
        //Serial.println(distance);
        //int v = (int) ((distance / (windowX * 2)) * 255) + (int) (20 * (float) ((i % 7) - 3));
        int v = (int) ((distance / (windowY * 2)) * 255);
//       Serial.println(v);
        //leds[i].r = v;
        //leds[i].g = v;
        //leds[i].b = v;
        //push(i);
     }
     
}

void calibrate(){
  wipe();
  for(int i = 0; i < LIGHT_COUNT; )
  {
   if(Serial.read() != -1){
     wipe();
     strip.setPixelColor(i, 255,255,255);
     ++i;
   }
   strip.show();
   delay(15); 
  }
  Serial.write("calibration over");
  state = 0;
}

////////////////////

/*
a machine that uses this method will receive serial commands
in this form, minus the quotation marks.
    
  ------number of bytes in this message  
  | --- device ID number (0 means all, if first character isn't 0 this string
  | |   is parsed to determine if command is designated for this machine.  
  | | ---- state ID
  | | |  ------- additonal arguments (optional, determined by state ID)      
  | | |  |  |  |           
"15 0 1 45 36 64"

The individual arguments here are given in order of preference.  If a buffer is
in danger of overflowing, the byte size allows that machine to trash those
bytes if it needs to.  This would result in a loss of resolution, but it avoids
having to develop a handshake system which could be fatally time consuming.  

For questions ask Tyler.

*/
void readNextCommand_arduino()
{
 char term = '\n';
 if(Serial.available() == 0) return;
 char msgSize = Serial.read();
 char targetid = Serial.read();
 if(targetid != 0 && !isCommandForMe_arduino()) return;
 state = Serial.read();
 parseExtraArgs_arduino(state);
}

boolean isCommandForMe_arduino()
{
 char inByte = Serial.read();
 while(inByte != ':')
 {
   if (inByte == DEVICE_ID) return true; 
 }
 return false;
}

void parseExtraArgs_arduino(int state)
{
  switch(state)
  {
    case GAME: nitrogen = Serial.read();
               ph = Serial.read();
               temperature = Serial.read();
               break;
    default  : return;
  } 
}

void readNextCommand()
{
  //Serial.println(sb);
  char term = '\n';
  if(Serial.available() > 0)
  {
    char inChar = Serial.read();
    if (inChar == term)
    {
      //Serial.println("Received newline");
      //Serial.println(sb);
      char* cmdbuf;
      cmdbuf = strtok(sb, " ");
      //Serial.println(1);
      int cmdsize = atoi(cmdbuf);
      cmdbuf = strtok(NULL, " ");
      //Serial.println(2);
      if (!isCommandForMe(cmdbuf)) return;
      cmdbuf = strtok(NULL, " ");
      //Serial.println(3);
      state = atoi(cmdbuf);
      Serial.print("State =");
      Serial.println(state);
      clearsb();
    }
    else 
    {
     if (sbpos < SB_BUFFER_SIZE)
     {
       sb[sbpos++] = inChar;
       sb[sbpos] = '\0';
     }
     else 
     {
       Serial.println("Buffer overflow"); 
     }
    }
  }
}

void clearsb()
{
  sb[0] = '\0';
  sbpos = 0; 
}

boolean isCommandForMe(char* str)
{
  //Serial.print("icfm");
  //Serial.println(str);
  if (str[0] == '0') return true; 
  int i = 0;
  while(str[i] != NULL)
  {
    //Serial.println(str[i]);
    if(str[i] == DEVICE_ID) return true;
    ++i; 
  }
  return false;
}

void parseExtraArgs(int state)
{
  char* argbuf;
  switch(state)
  {
   case GAME: argbuf = strtok(NULL , " ");
              nitrogen = atoi(argbuf);
              argbuf = strtok(NULL, " ");
              ph = atoi(argbuf);
              argbuf = strtok(NULL, " ");
              temperature = atoi(argbuf);
   default:   return; 
  }
  
}

boolean isEmpty()
{
   return top == -1; 
}
 
void push(int led)
{
  buffer[++top] = led;
}
 
int pop()
{
  return buffer[top--]; 
}

void init_leds()
{
  
  
  for(int i = 0; i < LIGHT_COUNT; ++i)
  {
    leds[i].id = i;
    switch(i){
      case 0: leds[i].x = 4;
              leds[i].y = 16;
              leds[i].type = NITROGEN;
              break;
      case 1: leds[i].x = 6;
              leds[i].y = 10;
              leds[i].type = PLANKTON;
              break;
      case 2: leds[i].x = 4;
              leds[i].y = 4;
              leds[i].type = PH;
              break;
      case 3: leds[i].x = 10;
              leds[i].y = 8;
              leds[i].type = TEMPERATURE;
              break;
      case 4: leds[i].x = 16;
              leds[i].y = 7;
              leds[i].type = PH;
              break;
      case 5: leds[i].x = 23;
              leds[i].y = 5;
              leds[i].type = TEMPERATURE;
              break;
      case 6: leds[i].x = 21;
              leds[i].y = 10;
              leds[i].type = PLANKTON;
              break;
      case 7: leds[i].x = 23;
              leds[i].y = 17;
              leds[i].type = NITROGEN;
              break;
      case 8: leds[i].x = 22;
              leds[i].y = 23;
              leds[i].type = PH;
              break; 
      case 9: leds[i].x = 18;
              leds[i].y = 29;
              leds[i].type = TEMPERATURE;
              break;
      case 10: leds[i].x = 23;
               leds[i].y = 39;
               leds[i].type = PH;
               break;
      case 11: leds[i].x = 19;
               leds[i].y = 36;
               leds[i].type = PLANKTON;
               break;
      case 12: leds[i].x = 15;
               leds[i].y = 34;
               leds[i].type = NITROGEN;
               break;
      case 13: leds[i].x = 14;
               leds[i].y = 31;
               leds[i].type = TEMPERATURE;
               break;
      case 14: leds[i].x = 10;
               leds[i].y = 33;
               leds[i].type = PLANKTON;
               break;
      case 15: leds[i].x = 12;
               leds[i].y = 38;
               leds[i].type = PH;
               break;
      case 16: leds[i].x = 15;
               leds[i].y = 45;
               leds[i].type = NITROGEN;
               break;
      case 17: leds[i].x = 10;
               leds[i].y = 43;
               leds[i].type = PLANKTON;
               break;
      case 18: leds[i].x = 4;
               leds[i].y = 39;
               leds[i].type = TEMPERATURE;
               break;
      case 19: leds[i].x = 5;
               leds[i].y = 32;
               leds[i].type = NITROGEN;
               break;
      case 20: leds[i].x = 11;
               leds[i].y = 25;
               leds[i].type = NITROGEN;
               break;
      case 21: leds[i].x = 16;
               leds[i].y = 20;
               leds[i].type = PLANKTON;
               break;
      case 22: leds[i].x = 13;
               leds[i].y = 17;
               leds[i].type = TEMPERATURE;
               break;
      case 23: leds[i].x = 5;
               leds[i].y = 23;
               leds[i].type = PH;
               break;
      case 24: leds[i].x = 22;
               leds[i].y = 10;
               leds[i].type = PH;
               break;
      case 25: leds[i].x = 20;
               leds[i].y = 14;
               leds[i].type = PLANKTON;
               break;
      case 26: leds[i].x = 21;
               leds[i].y = 18;
               leds[i].type = TEMPERATURE;
               break;
      case 27: leds[i].x = 22;
               leds[i].y = 23;
               leds[i].type = PH;
               break;
      case 28: leds[i].x = 19;
               leds[i].y = 27;
               leds[i].type = PLANKTON;
               break;
      case 29: leds[i].x = 19;
               leds[i].y = 32;
               leds[i].type = NITROGEN;
               break;
      case 30: leds[i].x = 14;
               leds[i].y = 35;
               leds[i].type = PH;
               break;
      case 31: leds[i].x = 19;
               leds[i].y = 39;
               leds[i].type = TEMPERATURE;
               break;
      case 32: leds[i].x = 15;
               leds[i].y = 42;
               leds[i].type = PLANKTON;
               break;
      case 33: leds[i].x = 11;
               leds[i].y = 44;
               leds[i].type = NITROGEN;
               break;
      case 34: leds[i].x = 5;
               leds[i].y = 38;
               leds[i].type = TEMPERATURE;
               break;
      case 35: leds[i].x = 6;
               leds[i].y = 33;
               leds[i].type = PLANKTON;
               break;
      case 36: leds[i].x = 4;
               leds[i].y = 27;
               leds[i].type = PH;
               break;
      case 37: leds[i].x = 11;
               leds[i].y = 28;
               leds[i].type = NITROGEN;
               break;
      case 38: leds[i].x = 14;
               leds[i].y = 24;
               leds[i].type = TEMPERATURE;
               break;
      case 39: leds[i].x = 13;
               leds[i].y = 19;
               leds[i].type = NITROGEN;
               break;
      case 40: leds[i].x = 14;
               leds[i].y = 13;
               leds[i].type = NITROGEN;
               break;
      case 41: leds[i].x = 15;
               leds[i].y = 7;
               leds[i].type = TEMPERATURE;
               break;
      case 42: leds[i].x = 8;
               leds[i].y = 6;
               leds[i].type = PLANKTON;
               break;
      case 43: leds[i].x = 8;
               leds[i].y = 3;
               leds[i].type = PH;
               break;
      case 44: leds[i].x = 3;
               leds[i].y = 8;
               leds[i].type = NITROGEN;
               break;
      case 45: leds[i].x = 3;
               leds[i].y = 16;
               leds[i].type = TEMPERATURE;
               break;
      case 46: leds[i].x = 7;
               leds[i].y = 19;
               leds[i].type = PLANKTON;
               break;
      case 47: leds[i].x = 9;
               leds[i].y = 14;
               leds[i].type = PH;
               break;
      case 48: leds[i].x = 23;
               leds[i].y = 26;
               leds[i].type = PH;
               break;
      case 49: leds[i].x = 21;
               leds[i].y = 32;
               leds[i].type = NITROGEN;
               break;
      case 50: leds[i].x = 18;
               leds[i].y = 37;
               leds[i].type = PH;
               break;
      case 51: leds[i].x = 12;
               leds[i].y = 40;
               leds[i].type = PLANKTON;
               break;
      case 52: leds[i].x = 13;
               leds[i].y = 44;
               leds[i].type = TEMPERATURE;
               break;
      case 53: leds[i].x = 7;
               leds[i].y = 38;
               leds[i].type = NITROGEN;
               break;
      case 54: leds[i].x = 12;
               leds[i].y = 32;
               leds[i].type = TEMPERATURE;
               break;
      case 55: leds[i].x = 6;
               leds[i].y = 30;
               leds[i].type = PLANKTON;
               break;
      case 56: leds[i].x = 3;
               leds[i].y = 26;
               leds[i].type = PH;
               break;
      case 57: leds[i].x = 9;
               leds[i].y = 25;
               leds[i].type = NITROGEN;
               break;
      case 58: leds[i].x = 12;
               leds[i].y = 19;
               leds[i].type = PLANKTON;
               break;
      case 59: leds[i].x = 6;
               leds[i].y = 19;
               leds[i].type = TEMPERATURE;
               break;
      case 60: leds[i].x = 4;
               leds[i].y = 14;
               leds[i].type = NITROGEN;
               break;
      case 61: leds[i].x = 6;
               leds[i].y = 10;
               leds[i].type = PLANKTON;
               break;
      case 62: leds[i].x = 4;
               leds[i].y = 6;
               leds[i].type = TEMPERATURE;
               break;
      case 63: leds[i].x = 11;
               leds[i].y = 7;
               leds[i].type = PH;
               break;
      case 64: leds[i].x = 11;
               leds[i].y = 14;
               leds[i].type = NITROGEN;
               break;
      case 65: leds[i].x = 17;
               leds[i].y = 21;
               leds[i].type = TEMPERATURE;
               break;
      case 66: leds[i].x = 18;
               leds[i].y = 28;
               leds[i].type = PLANKTON;
               break;
      case 67: leds[i].x = 17;
               leds[i].y = 16;
               leds[i].type = PH;
               break;
      case 68: leds[i].x = 17;
               leds[i].y = 5;
               leds[i].type = PLANKTON;
               break;
      case 69: leds[i].x = 22;
               leds[i].y = 4;
               leds[i].type = NITROGEN;
               break;
      case 70: leds[i].x = 22;
               leds[i].y = 14;
               leds[i].type = PH;
               break;
      default: break;
    }    
    
  }
  //}
  
  /*
  0: n
  1: pl
  2: ph
  3: t
  4: 16, 7 ph 
  5: 23, 5 t
  6: 21, 10 pl
  7: 23, 17 n
  8: 22, 23 ph
  9: 18, 29 t
  10: 23, 39 ph 
  11: 19, 36 pl
  12: 15, 34 n
  13: 14, 31 t
  14: 10, 33 pl
  15: 12, 38 ph
  16: 15, 45 n
  17: 10, 43 pl
  18: 4, 39 t
  19: 5, 32 n
  20: 11, 25 n
  21: 16, 20 pl
  22: 13, 17 t
  23: 5, 23 ph
  24: 22, 10  ph
  25: 20, 14 pl
  26: 21, 18 t
  27: 22, 23 ph
  28: 19, 27 pl
  29: 19, 32 n
  30: 14, 35 ph
  31: 19, 39 t
  32: 15, 42 pl
  33: 11, 44 n
  34: 5, 38 t
  35: 6, 33 pl
  36: 4, 27 ph
  37: 11, 28 n
  38: 14, 24 t
  39: 13, 19 n
  40: 14, 13 n
  41: 15, 7 t
  42: 8, 6pl 
  43: 8, 3 ph
  44: 3, 8 n
  45: 3, 16 temp
  46: 7, 19 pl 
  47: 9, 14 ph
  48: 23, 26 ph
  49: 21, 32 n
  50: 18, 37 ph
  51: 12, 40 pl
  52: 13, 44 t
  53: 7, 38 n
  54: 12, 32 t
  55: 6, 30 pl
  56: 3, 26 ph
  57: 9, 25 n
  58: 12, 19 pl
  59: 6, 19 t
  60: 4, 14 n
  61: 6, 10 pl
  62: 4, 6 t
  63: 11, 7 ph
  64: 11, 14 n
  65: 17, 21 t
  66: 18, 28 pl
  67: 17, 16 ph
  68: 20, 8 pl
  69: 17, 5 n
  70: 22, 4 ph
  */
  
}
