import controlP5.*;

ControlP5 cp5;
int myColor = color(0,0,0);

//adjustable constants, changing will dynamically resize things
int windowX = 800;
int windowY = 600;
int sqrlen = 20;

int nitrogen = 255;
int ph = 255;
int salinity = 255;

//non-adjustable global variables.  Don't touch.
int inRow = (int)windowX / (sqrlen * 2);
int inCol = (int)windowY / (sqrlen * 2);
int numsquares = inRow * inCol;
int squares[][] = new int[numsquares][3];
int rounding = sqrlen / 3;

void setup()
{
  size(windowX, windowY);
  background(0);
  colorMode(RGB);
  cp5 = new ControlP5(this);
  
  cp5.addSlider("nitrogen")
    .setPosition(0,windowY-60)
    .setRange(0,255)
    .setSize(155,20)
    .setNumberOfTickMarks(10)
    ;
  cp5.addSlider("ph")
    .setPosition(0,windowY-40)
    .setRange(0,255)
    .setSize(155,20)
    .setNumberOfTickMarks(10)
    ;
  cp5.addSlider("salinity")
    .setPosition(0,windowY-20)
    .setRange(0,255)
    .setSize(155,20)
    .setNumberOfTickMarks(10)
    ;
    
  cp5.getController("salinity").getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER).setPaddingX(0);
  cp5.getController("ph").getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER).setPaddingX(0);
  cp5.getController("nitrogen").getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER).setPaddingX(0);

  //set up the grid
  for(int i = 0; i < numsquares; ++i)
  {
    squares[i][0] = nitrogen;
    squares[i][1] = ph;
    squares[i][2] = salinity;
  }
}

void draw(){

  update();

  for(int i = 0; i < numsquares; ++i)
  {
    fill(squares[i][0], squares[i][1], squares[i][2]);
    rect(((i * sqrlen * 2) % windowX) + sqrlen / 2, (sqrlen * 2 * ((int) (i / (windowX / (sqrlen * 2))))) + sqrlen / 2, sqrlen, sqrlen, rounding);
  }
}

void update()
{
  if (nitrogen == ph && ph == salinity) {
    tyler_sin();
  } else {
    graham_func();
  }
}

int sin_inc = 0;
void tyler_sin()
{
  for (int i = 0; i < numsquares; ++i)
  {
    squares[i][0] = (int) abs(sin(0.01 * sin_inc * PI) * 255);
    squares[i][1] = (int) abs(cos(0.01 * sin_inc * PI) * 255);
    squares[i][2] = (int) abs(sin(0.01 * sin_inc * PI) * 255);

  }
  sin_inc = (sin_inc + 1) % 200;
}

void graham_func() {
  for (int i = 0; i < numsquares; ++i) {
    squares[i][0] = nitrogen;
    squares[i][1] = ph;
    squares[i][2] = salinity;
  }
}

void slider(float theColor) {
  myColor = color(theColor);
}

