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
#include <SerialCommand.h>
#define DATA_PIN (11)
#define CLOCK_PIN (13)

#define LIGHT_COUNT (50)

uint32_t pixels[LIGHT_COUNT];

#define pinModeFast(x, y) pinMode(x, y)
#define digitalWriteFast(x, y) digitalWrite(x, y)
SerialCommand SCmd;   // The demo SerialCommand object

void setup() {
  Serial.begin(9600);
//  Serial.write("Set up!!!");
  pinModeFast(DATA_PIN, OUTPUT);
  pinModeFast(CLOCK_PIN, OUTPUT);
  digitalWriteFast(DATA_PIN, LOW);
  digitalWriteFast(CLOCK_PIN, LOW);
  SCmd.addCommand("P",process_command);  // Converts two arguments to integers and echos them back 
  SCmd.addCommand("party",party);  // Converts two arguments to integers and echos them back 
  SCmd.addCommand("breath",breath);  // Converts two arguments to integers and echos them back 
  SCmd.addCommand("id",id);  // Converts two arguments to integers and echos them back 
  
  SCmd.addDefaultHandler(unrecognized);  // Handler for command that isn't matched  (says "What?") 
  Serial.println("Ready"); 
}
void loop() {
  SCmd.readSerial();
//  uint32_t color = 0;
//    int val1 = analogRead(2) / 4;
////    solid_color(val1);
////    show();
//    for (uint8_t jj= 0; jj <50; ++jj){
//      set_pixel_rgb(jj, val1,255-val1,val1);
//      show();
//    }

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
  for (int i = 0; i < 50; i++) {
    toggle_clock();
  }
  for (int i = 0; i < LIGHT_COUNT; ++i) {
    write_pixel(i);
  }
  write_blank_pixel();
  delay(1);
}

static void solid_color(uint32_t color) {
  for (int i = 0; i < LIGHT_COUNT; ++i) {
    set_pixel(i, color);
  }
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

static uint32_t rand_color() {
  return simple_color(rand() % 7);
}

static uint32_t rand_color_except(uint32_t color) {
  uint32_t result_color;
  do {
    result_color = simple_color(rand() % 7);
  } while (result_color == color);
  return result_color;
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

void process_command()    
{
  int aNumber;  
  char *arg; 

  Serial.println("We're in process_command"); 
  arg = SCmd.next();
  int rr,bb,gg =0; 
  if (arg != NULL) 
  {
    aNumber=atoi(arg);    // Converts a char string to an integer
    Serial.print("First argument was: "); 
    Serial.println(aNumber); 
    rr=aNumber;
  } 
  else {
    Serial.println("No arguments"); 
  }

  arg = SCmd.next(); 
  if (arg != NULL) 
  {
    aNumber=atol(arg); 
    Serial.print("Second argument was: "); 
    Serial.println(aNumber); 
    gg=aNumber;
  } 
  else {
    Serial.println("No second argument"); 
  }
  arg = SCmd.next(); 
  if (arg != NULL) 
  {
    aNumber=atol(arg); 
    Serial.print("Third argument was: "); 
    Serial.println(aNumber); 
    bb = aNumber;
  } 
  else {
    Serial.println("No Third argument"); 
  }
  for (uint8_t jj= 0; jj <50; ++jj){
      set_pixel_rgb(jj,int(rr), int(gg),int(bb));
      Serial.println("Setting strip to:");
      Serial.print(rr);
      Serial.print(" ,");
      Serial.print(gg);
      Serial.print(" ,");
      Serial.print(bb);
      show();
    }
  
}

void party()
{
  solid_color(simple_color(3) & get_dimmer_mask(100));
  show();
  delay(2000);
  wipe(10);
  set_pixel(5,simple_color(3) & get_dimmer_mask(100));
  show();
  for (uint32_t j = 0; j < 255; ++j) {
      for (uint32_t n =0; n< LIGHT_COUNT; n++){
      if (n % 2 == 0){  
      set_pixel(n,simple_color(1) & get_dimmer_mask(j));
//      show();
      }
      else {
      set_pixel(n,simple_color(5) & get_dimmer_mask(j));
//      show();
      }
//      show();
//      n=n+2;
      }
      show();
    }
    for (uint32_t j = 255; j!= -1; --j) {
      for (uint32_t n =0; n< LIGHT_COUNT; n++){
      if (n % 2 == 0){  
      set_pixel(n,simple_color(1) & get_dimmer_mask(j));
      
      }
      else {
      set_pixel(n,simple_color(5) & get_dimmer_mask(j));
     
      }
//      show();
//      n=n+2;
      }
      if (j%4 == 0) show();
      
    }
// for (uint8_t k = 0; k < 2; ++k) {
//    uint32_t color = simple_color(k);
//    //Dim Up The Given Color
//    for (uint32_t j = 0; j < 255; ++j) {
//      for (uint32_t n =0; n< LIGHT_COUNT; n+2){      
//      set_pixel(n,color & get_dimmer_mask(j));
//      show();
//      }
//    }
//    //Dim Down the Given Color
//    for (uint32_t j = 255; j != 0; --j) {
//      solid_color(color & get_dimmer_mask(j));
//      show();
//    }
//  } 
}

void set_board_rgb(int r, int g, int b){
  set_pixel_rgb(15,r,g,b);
  set_pixel_rgb(17,g,r,b);
  set_pixel_rgb(19,r,g,b);
  set_pixel_rgb(21,g,r,b);
  set_pixel_rgb(23,r,g,b);
  set_pixel_rgb(25,g,r,b);
  set_pixel_rgb(27,r,g,b);
  set_pixel_rgb(29,g,r,b);
  set_pixel_rgb(31,r,g,b);
  set_pixel_rgb(34,g,r,b);
  set_pixel_rgb(36,r,g,b);
  set_pixel_rgb(39,g,r,b);
  set_pixel_rgb(43,r,g,b);
  set_pixel_rgb(46,g,r,b);
  set_pixel_rgb(49,r,g,b);
    
}

int i = 0;
void breath(){
 
for(i = 15 ; i <= 255; i+=1)
  { 
//    solid_color(i);
    set_board_rgb(i,0,0);
    
//    set_pixel_rgb(5,i,0,0);
//    set_pixel_rgb(7,0,i,0);
//    set_pixel_rgb(9,i,0,0);
//    set_pixel_rgb(11,0,i,0);
     
    show();
    if (i > 150) {
      delay(4);
    }
    if ((i > 125) && (i < 151)) {
      delay(5);
    }
    if (( i > 100) && (i < 126)) {
      delay(7);
    }
    if (( i > 75) && (i < 101)) {
      delay(10);
    }
    if (( i > 50) && (i < 76)) {
      delay(14);
    }
    if (( i > 25) && (i < 51)) {
      delay(18);
    }
    if (( i > 1) && (i < 26)) {
      delay(19);
    }
  }
  for(i = 255; i >=15; i-=1)
  {
//    solid_color(i);
//    set_pixel_rgb(5,i,0,0);
//    set_pixel_rgb(7,0,i,0);
//    set_pixel_rgb(9,i,0,0);
//    set_pixel_rgb(11,0,i,0);
     set_board_rgb(i,0,0);
    
    show();
    if (i > 150) {
      delay(4);
    }
    if ((i > 125) && (i < 151)) {
      delay(5);
    }
    if (( i > 100) && (i < 126)) {
      delay(7);
    }
    if (( i > 75) && (i < 101)) {
      delay(10);
    }
    if (( i > 50) && (i < 76)) {
      delay(14);
    }
    if (( i > 25) && (i < 51)) {
      delay(18);
    }
    if (( i > 1) && (i < 26)) {
      delay(19);
    }
  }
  SCmd.readSerial();
  delay(970);
  SCmd.readSerial();
  breath();
}
void solid()
{
 for (uint8_t k = 0; k < 7; ++k) {
    uint32_t color = simple_color(k);
    //Dim Up The Given Color
    for (uint32_t j = 0; j < 255; ++j) {
      solid_color(color & get_dimmer_mask(j));
      show();
    }
    //Dim Down the Given Color
    for (uint32_t j = 255; j != 0; --j) {
      solid_color(color & get_dimmer_mask(j));
      show();
    }
  } 
}

void id()    
{
  int aNumber;  
  char *arg; 

//  Serial.println("We're in process_command"); 
  arg = SCmd.next();
  int rr,bb,gg =0; 
  if (arg != NULL) 
  {
    aNumber=atoi(arg);    // Converts a char string to an integer
    Serial.print("First argument was: "); 
    Serial.println(aNumber); 
    rr=aNumber;
  } 
  else {
    Serial.println("No arguments"); 
  }
  solid_color(0);
  set_pixel_rgb(rr, 255, 0, 0);
  show();
}
// This gets set as the default handler, and gets called when no other command matches. 
void unrecognized()
{
  Serial.println("What?"); 
   for (uint8_t jj= 0; jj <50; ++jj){
      set_pixel_rgb(jj,0, 0,255);
      set_pixel_rgb(50-jj,255, 255,0);
      show();
      solid_color(0);
      show();
    }
}

