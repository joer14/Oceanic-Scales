
//adjustable constants, changing will dynamically resize things
int windowX = 800;
int windowY = 600;
int sqrlen = 20;
int numleds = 400;

//non-adjustable global variables.  Don't touch.

LED leds[] = new LED[numleds];
int rounding = sqrlen / 3;
Dispatch d = new Dispatch();

class LED
{
  int id;
  int x;
  int y;
  double vibration;
  int r;
  int g;
  int b; 
}

class Dispatch
{
  int urgency;
  String[] titles;
  void show()
  {
     int size = 100;
     for(int i = 0; i < titles.length; ++i)
     {
       tint(255, 126);
       rect(size,i * size, size, size);
       //textSize(24);  
     }
  }
}

void setup()
{
  size(windowX, windowY);
  background(0);
  colorMode(RGB);
  d.titles = new String[2];
  d.titles[0] = "Pulsate";
  d.titles[1] = "Twinke";
  d.urgency = 5;
  
  
  //set up the grid
  for(int i = 0; i < numleds; ++i)
  {
    LED newled = new LED();
    newled.id = i;
    newled.r = 0;
    newled.g = 0;
    newled.b = 0;
    newled.vibration = 0;
    newled.x = (int) (Math.random() * (windowX - sqrlen));
    newled.y = (int) (Math.random() * (windowY - sqrlen));
    leds[i] = newled;
  }
}

int counter = 0;
void draw(){

  update();
  background(0);
  for(int i = 0; i < numleds; ++i)
  {
    fill(leds[i].r, leds[i].g, leds[i].b);
    int x = leds[i].x + (int) ((sin((float) Math.random() * TWO_PI) + 1) * leds[i].vibration / 2);
    int y = leds[i].y + (int) ((sin((float) Math.random() * TWO_PI) + 1) * leds[i].vibration / 2);
    rect(x, y, sqrlen, sqrlen, rounding);
  }
  
  counter++;
}

void update()
{
   //tyler_twinkle();
   tyler_pulsate();
}

void tyler_twinkle()
{
  for(int i = 0; i < numleds; ++i)
  {
    switch(i % 5)
    {
      case 0: leds[i].r = leds[i].g = leds[i].b = 127 + (int) (127 * sin(TWO_PI * (float) (counter) / 400));
              leds[i].vibration = sin(TWO_PI * (float) (counter) / 400) + 1;
              break;
      case 1: leds[i].r = leds[i].g = leds[i].b = 127 + (int) (127 * sin(TWO_PI * (float) ((counter + 80) % 400) / 400));
              leds[i].vibration = sin(TWO_PI * (float) ((counter + 80) % 400) / 400) + 1;
              break;
      case 2: leds[i].r = leds[i].g = leds[i].b = 127 + (int) (127 * sin(TWO_PI * (float) ((counter + 160) % 400) / 400));
              leds[i].vibration = sin(TWO_PI * (float) ((counter + 160) % 400) / 400) + 1;
              break;
      case 3: leds[i].r = leds[i].g = leds[i].b = 127 + (int) (127 * sin(TWO_PI * (float) ((counter + 240) % 400) / 400));
              leds[i].vibration = sin(TWO_PI * (float) ((counter + 240) % 400) / 400) + 1;
              break;
      case 4: leds[i].r = leds[i].g = leds[i].b = 127 + (int) (127 * sin(TWO_PI * (float) ((counter + 320) % 400) / 400));
              leds[i].vibration = sin(TWO_PI * (float) ((counter + 320) % 400) / 400) + 1;
              break;
    }
  }

 counter = counter % 400; 
}

void tyler_pulsate()
{
   float center = sin(TWO_PI * (float) counter / 400) * windowX ;
   for(int i = 0; i < numleds; ++i)
   {
     
      float distance = abs(leds[i].x - center);
      int v = (int) ((distance / (windowX * 2)) * 255) + (int) (20 * (float) ((leds[i].id % 7) - 3));
      leds[i].r = leds[i].g = leds[i].b = v;
   }
   counter = counter % 400;

}


