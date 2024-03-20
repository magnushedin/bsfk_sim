typedef struct {
  int stepPin;
  int dirPin;
  int stepPerRev;
  int pulseWidthMicros;
  int millisBtwnSteps;
  int pos;
  int targetPos;
} stepperCtrl;

// stepXPin = 2; //X.STEP
// dirXPin = 5; // X.DIR
// stepYPin = 3; //Y.STEP
// dirYPin = 6; // Y.DIR
// stepZPin = 4; //Z.STEP
// dirZPin = 7; // Z.DIR
