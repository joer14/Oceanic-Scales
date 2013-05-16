/*
 * An Arduino controller for LED strings based on the SM16716 controller.
 *
 * By Mike Tsao (github.com/sowbug). Copyright 2012, all rights reserved.
 *
 * You might see these being sold as WS2801 strands, and you're wondering why
 * your off-the-shelf WS2801 code isn't working. This sketch will restore
 * your confidence that your lights aren't broken.
 *
 * This code isn't the fastest, but it is just about the simplest you'll
 * find. For performance, inv estigate code that uses hardware SPI.
 *
 * Beware that everyone seems to use different wiring color schemes. I
 * found the order of the wires is more reliable. Looking at the IC side
 * of the board, you'll find from left to right: V+, Data, Clock, Ground.
 */
 
//#include <SerialCommand.h>
#define DATA_PIN (11)
#define CLOCK_PIN (13)

#define LIGHT_COUNT (50)
#define SB_BUFFER_SIZE (64)
#define DEVICE_ID (1)

uint32_t pixels[LIGHT_COUNT];

#define pinModeFast(x, y) pinMode(x, y)
#define digitalWriteFast(x, y) digitalWrite(x, y)
//SerialCommand SCmd;   // The demo SerialCommand object

//pseudo led class field values
enum fields{
  X,
  Y,
  R,
  G,
  B,
  T   //0 for plankton, 1 for element
};

//state values
enum states{
  RESTING,
  GAME
};

enum types{
  PHYTOS,
  CHEMS 
};

float windowY = 48;
float windowX = 26;

char sb [SB_BUFFER_SIZE]; //sb = serialbuffer
int sbpos = 0;

int state = 0;

int leds [LIGHT_COUNT][6];
int top = -1;
int buffer [LIGHT_COUNT];

int nitrogen = 0;
int ph = 0;
int temperature = 0;

void setup() {
  
  Serial.begin(9600);

  int leds[LIGHT_COUNT][5]; // (x,y,r,g,b)
  init_leds();
      
//  Serial.write("Set up!!!");
  pinModeFast(DATA_PIN, OUTPUT);
  pinModeFast(CLOCK_PIN, OUTPUT);
  digitalWriteFast(DATA_PIN, LOW);
  digitalWriteFast(CLOCK_PIN, LOW);
  Serial.println("Ready"); 
}


void loop() {
  readNextCommand();
  switch(state)
  {
    case RESTING: solid(CHEMS);
                  solid(PHYTOS);
                  break;
    case    GAME: tyker(CHEMS);
                  break;
    default     : tyker(PHYTOS);
  }
  
  
  while(!isEmpty())
  {
     int popped = pop();
     set_pixel_rgb(popped, leds[popped][B], leds[popped][G], leds[popped][R]);
     //THESE ARE SWAPPED. RESWAP THEM WHEN WE HAVE A NON-JANKY LIGHT STRIP.
  }

  show();

}

static void set_pixel(uint8_t index, uint32_t color) {
  pixels[index] = color & 0x00FFFFFF;
}

static void set_pixel_rgb(uint8_t index, uint32_t r, uint32_t g, uint32_t b) {
  set_pixel(index, r | (g << 8) | (b << 16));
}

static void toggle_clock() {
  digitalWriteFast(CLOCK_PIN, HIGH);
  digitalWriteFast(CLOCK_PIN, LOW);
}

static void write_pixel(uint8_t i) {
  const uint32_t MASK = ((uint32_t)(1) << 24);
  uint32_t p = pixels[i] | MASK;
  int j = 25;
  while (j--) {
    digitalWriteFast(DATA_PIN, (p & MASK) ? HIGH : LOW);
    toggle_clock();
    p <<= 1;
  }
}

static void write_blank_pixel() {
  int j = 25;
  while (j--) {
    digitalWriteFast(DATA_PIN, 0);
    toggle_clock();
  }
}

static void show() {
  digitalWriteFast(DATA_PIN, LOW);
  for (int i = 0; i < LIGHT_COUNT; i++) {
    toggle_clock();
  }
  for (int i = 0; i < LIGHT_COUNT; ++i) {
    write_pixel(i);
  }
  write_blank_pixel();
  delay(1);
}

static uint32_t simple_color(uint8_t color) {
  switch (color) {
  case 0:
    return 0x00ff0000;
  case 1:
    return 0x0000ff00;
  case 2:
    return 0x000000ff;
  case 3:
    return 0x00ffff00;
  case 4:
    return 0x00ff00ff;
  case 5:
    return 0x0000ffff;
  case 6:
    return 0x00ffffff;
  default:
    return 0x00330000;
  }
}

static uint32_t blue_color(uint8_t color) {
  switch (color) {
  case 0:
    return 0x004682B4;
  case 1:
    return 0x004682B4;
  case 2:
    return 0x00B0C4DE;
  case 3:
    return 0x00B0E0E6;
  case 4:
    return 0x00ff00ff;
  case 5:
    return 0x0087CEEB;
  case 6:
    return 0x0087CEFA;
  default:
    return 0x0000BFFF;
  }
}

static uint32_t get_dimmer_mask(uint32_t value) {
  return value | (value << 8) | (value << 16);
}

static void scroll(int8_t delta) {
  size_t bytes_to_move = (LIGHT_COUNT - delta) * sizeof(uint32_t);
  if (delta > 0) {
    memmove(&pixels[delta], &pixels[0], bytes_to_move);
  } else {
    memmove(&pixels[0], &pixels[-delta], bytes_to_move);
  }
}

static void wipe(uint8_t delay_msec) {
  for (int i = 0; i < LIGHT_COUNT; ++i) {
    scroll(1);
    set_pixel(0, 0);
    show();
    delay(delay_msec);
  }
}

void solid(int type)
{
   for(int i = 0; i < LIGHT_COUNT; ++i)
   {
     if(leds[i][X] == 0 || leds[i][T] != type) continue;
     leds[i][R] = leds[i][G] = 0;
     leds[i][B] = 255;
     push(i);
   }
   
}

//float counter = (millis() / 5000) % 400;
void tyker(int type)
{      
     delay(10);
     float counter =  (millis() / 2000.0) ;
     float center = sin((PI / 2) * counter) * windowY * 2 - (windowY / 2);
     for(int i = 0; i < LIGHT_COUNT; ++i)
     {
        if (leds[i][Y] == 0 || leds[i][T] != type) continue;
        float distance = abs(leds[i][Y] - center);
        //Serial.println(distance);
        //int v = (int) ((distance / (windowX * 2)) * 255) + (int) (20 * (float) ((i % 7) - 3));
        int v = (int) ((distance / (windowY * 2)) * 127);
//       Serial.println(v);
        leds[i][R] = v;
        push(i);
     }
     
}

//waves on the beach
void tyker2(int type)
{      
//     Serial.println("Tyker");
     delay(10);
     float counter =  (millis() / 5000.0) ;
     //Serial.println(counter);
     float center = sin(TWO_PI * counter) * windowY;
     //Serial.print("center = ");
     //Serial.println(center);
     for(int i = 0; i < LIGHT_COUNT; ++i)
     {
        if (leds[i][Y] == 0 || leds[i][T] != type) continue;
        float distance = abs(leds[i][Y] - center);
        //Serial.println(distance);
        //int v = (int) ((distance / (windowX * 2)) * 255) + (int) (20 * (float) ((i % 7) - 3));
        int v = (int) ((distance / (windowY * 2)) * 255);
//       Serial.println(v);
        leds[i][R] = leds[i][G] = leds[i][B] = v;
        push(i);
     }
     
}

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
void readNextCommand()
{
  //Serial.println(sb);
  char term = '\n';
  while(Serial.available() > 0)
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
    leds[i][T] = 0;
    switch(i){
      case 49:   leds[i][X] = 17;
                 leds[i][Y] = 5;
                 break;
      case 46:   leds[i][X] = 22;
                 leds[i][Y] = 10;
                 leds[i][T] = 1;
                 break;
      case 43:   leds[i][X] = 17;
                 leds[i][Y] = 15;
                 break;
      case 39:   leds[i][X] = 7;
                 leds[i][Y] = 19;
                 break;
      case 36:   leds[i][X] = 4;
                 leds[i][Y] = 24;
                 leds[i][T] = 1;
                 break;
      case 34:   leds[i][X] = 3;
                 leds[i][Y] = 31;
                 break;
      case 31:   leds[i][X] = 5;
                 leds[i][Y] = 37;
                 break;
      case 29:   leds[i][X] = 10;
                 leds[i][Y] = 42;
                 leds[i][T] = 1;
                 break;
      case 27:   leds[i][X] = 16;
                 leds[i][Y] = 41;
                 break;
      case 25:   leds[i][X] = 21;
                 leds[i][Y] = 38;
                 break;
      case 23:   leds[i][X] = 22;
                 leds[i][Y] = 23;
                 leds[i][T] = 1;
                 break;
      case 21:   leds[i][X] = 21;
                 leds[i][Y] = 26;
                 break;
      case 19:   leds[i][X] = 15;
                 leds[i][Y] = 24;
                 leds[i][T] = 1;
                 break;
      case 17:   leds[i][X] = 11;
                 leds[i][Y] = 27;
                 break;
      case 15:   leds[i][X] = 14;
                 leds[i][Y] = 32;
                 break;
      default:   leds[i][X] = 0;
                 leds[i][Y] = 0;
    }
    leds[i][R] = 0;
    leds[i][G] = 0;
    leds[i][B] = 0;
  }
}
