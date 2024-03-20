#include "stepperCtrl.h"

#define NBR_OF_STEPPERS 3
#define EN_PIN 8

char print_buffer[16];

enum stepperCtrls {
  SPEED_CTRL = 0,
  VARIO_CTRL = 1,
  ALTI_CTRL = 2
};

bool newData = false;
bool indexPulse = true;
int currentStepper = SPEED_CTRL;

stepperCtrl speedCtrl = {
  .stepPin = 2,
  .dirPin =  5,
  .stepPerRev = 1600,
  .pulseWidthMicros = 30,
  .millisBtwnSteps = 100,
  .pos = 0,
  .targetPos = 0,
};

stepperCtrl varioCtrl = {
  .stepPin = 3,
  .dirPin =  6,
  .stepPerRev = 1600,
  .pulseWidthMicros = 30,
  .millisBtwnSteps = 100,
  .pos = 0,
  .targetPos = 0,
};

stepperCtrl altiCtrl = {
  .stepPin = 4,
  .dirPin =  7,
  .stepPerRev = 1600,
  .pulseWidthMicros = 30,
  .millisBtwnSteps = 100,
  .pos = 0,
  .targetPos = 0,
};

stepperCtrl *steppers[NBR_OF_STEPPERS];

void step(stepperCtrl *motor) {
      digitalWrite(motor->stepPin, HIGH);
      delayMicroseconds(motor->pulseWidthMicros);
      digitalWrite(motor->stepPin, LOW);
      delayMicroseconds(motor->millisBtwnSteps);
      motor->pos < motor->targetPos ? motor->pos++ : motor->pos--;
}

void setup() {
 	Serial.begin(115200);
  Serial.setTimeout(10);

  pinMode(25, INPUT);

 	pinMode(EN_PIN, OUTPUT);
 	digitalWrite(EN_PIN, LOW);
  
  steppers[SPEED_CTRL] = &speedCtrl;
  steppers[VARIO_CTRL] = &varioCtrl;
  steppers[ALTI_CTRL] = &altiCtrl;
 	
  for (int stepper=0; stepper < NBR_OF_STEPPERS; stepper++){
    pinMode(steppers[stepper]->stepPin, OUTPUT);
 	  pinMode(steppers[stepper]->dirPin, OUTPUT);
  }

  indexPulse = digitalRead(26);
  if (!indexPulse) {
    Serial.println("Moving away from index");
    steppers[ALTI_CTRL]->targetPos = -100;
    while (steppers[ALTI_CTRL]->targetPos != steppers[ALTI_CTRL]->pos) {
      step(steppers[ALTI_CTRL]);
      delay(1);
    }
  }
  
  Serial.println("starting calibration");
  indexPulse = digitalRead(26);
  while(indexPulse) {
    indexPulse = digitalRead(26);
    // Serial.println(indexPulse);
    steppers[ALTI_CTRL]->targetPos++;
    step(steppers[ALTI_CTRL]);
    delayMicroseconds(500);
    // delay(1);
  }
}


void loop() {
  int targetPos;
  indexPulse = digitalRead(26);
  if (Serial.available() > 0) {
    targetPos = Serial.parseInt();
    newData = true;
  }

  if (newData) {
    steppers[currentStepper]->targetPos = targetPos;
    currentStepper++;
    currentStepper = currentStepper % 3;
    newData = false;
  }

  for (int stepper=0; stepper < NBR_OF_STEPPERS; stepper++){
    int count = 0;

    int *pos = &steppers[stepper]->pos;
    int *targetPos = &steppers[stepper]->targetPos;
    
    steppers[stepper]->pos < steppers[stepper]->targetPos ? digitalWrite(steppers[stepper]->dirPin, HIGH) : digitalWrite(steppers[stepper]->dirPin, LOW);
    while ((steppers[stepper]->pos != steppers[stepper]->targetPos) && (count < 1)) {
      step(steppers[stepper]);
      count++;
    }
  }

  // sprintf(print_buffer, "%i %i %i %i", steppers[SPEED_CTRL]->pos, steppers[VARIO_CTRL]->pos, steppers[ALTI_CTRL]->pos, indexPulse);
  // Serial.println(print_buffer);
}