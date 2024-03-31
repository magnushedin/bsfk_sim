typedef struct {
  int stepPin;
  int dirPin;
  int stepPerRev;
  int pulseWidthMicros;
  int millisBtwnSteps;
  int pos;
  int targetPos;
} stepperCtrl;

typedef struct {int speed; int step;} map_t;

map_t stepMap[12] = 
{
  {0,   0},
  {20,  70},
  {40,  150},
  {50,  280},
  {60,  440},
  {80,  790},
  {100, 1080},
  {120, 1320},
  {150, 1650},
  {200, 2090},
  {250, 2500},
  {300, 2860}
};

// stepXPin = 2; //X.STEP
// dirXPin = 5; // X.DIR
// stepYPin = 3; //Y.STEP
// dirYPin = 6; // Y.DIR
// stepZPin = 4; //Z.STEP
// dirZPin = 7; // Z.DIR
