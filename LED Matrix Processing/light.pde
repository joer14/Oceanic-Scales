
//adjustable constants, changing will dynamically resize things
int windowX = 800;
int windowY = 600;
int sqrlen = 20;

//non-adjustable global variables.  Don't touch.
int inRow = (int)windowX / (sqrlen * 2);
int inCol = (int)windowY / (sqrlen * 2);
int numsquares = inRow * inCol;
int squares[][] = new int[numsquares][3];
rounding = sqrlen / 3;

void setup()
{
  size(windowX, windowY);
  background(0);
  //noLoop();
  for(int i = 0; i < numsquares; ++i)
  {
    squares[i][0] = 255;
    squares[i][1] = 255;
    squares[i][2] = 255;
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
  tyler_sin();
}

int sin_inc = 0;
void tyler_sin()
{
  for (int i = 0; i < numsquares; ++i)
  {
    squares[i][0] = (int) abs(sin(.01 * sin_inc * PI) * 255);
    squares[i][1] = (int) abs(cos(.01 * sin_inc * PI) * 255);
    squares[i][2] = (int) abs(sin(.01 * sin_inc * PI) * 255);

  }
  sin_inc = (sin_inc + 1) % 200;
}