/*
 * Sink water / sanitizer solution flow controller firmware
 * (c) 2017 Nilangshu Bidyanta
 */

#include <Servo.h>
#include "sink_ctrl.h"

static Servo servo[NUM_SERVOS];
static State nextState;
static ServoName activeServo;

/* Trigger the servo to turn off the switch and reset its position */
static void triggerServo(ServoName n)
{
  servo[n].write(finalPosDeg);
  delay(servoSettlingTimeMS);
  servo[n].write(initPosDeg);
}

void setup()
{
  /* Initialize the servo positions */
  for (uint8_t i = 0; i < NUM_SERVOS; i++) {
    servo[i].attach(servoPins[i]);
    servo[i].write(initPosDeg);
  }

  /* Initialize input pins */
  pinMode(buttonSanitizerPin, INPUT_PULLUP);
  pinMode(buttonWashPin, INPUT_PULLUP);

  /* Initialize the state */
  nextState = INIT;

  /* Wait for capacitive sensor module to settle down */
  delay(2000);

  Serial.begin(115200);
  Serial.println("Initialized");
}

static void delay_sec(uint16_t sec)
{
  uint8_t whole_multiple_10 = sec / 10;
  uint8_t remaining_sec = sec - 10 * whole_multiple_10;
  for (uint8_t i = 0; i < whole_multiple_10; i++)
    delay(10000);
  delay(remaining_sec * 1000);
}

void loop()
{
  uint8_t pinValue;
  switch (nextState) {

    case INIT:
      /* Check for button presses */
      pinValue = digitalRead(buttonSanitizerPin);
      if (pinValue == LOW) {
        nextState = FILLING;
        activeServo = R_SANITIZE;
        Serial.println("Fill Sanitizer");
        break;
      }
      pinValue = digitalRead(buttonWashPin);
      if (pinValue == LOW) {
        nextState = FILLING;
        activeServo = L_WASH;
        Serial.println("Fill Wash");
        break;
      }
      break;

    case FILLING:
      /* Wait for the liquid to fill */
      Serial.print("Wait for ");
      Serial.print(waitTimeSec, DEC);
      Serial.println(" seconds");
      delay_sec(waitTimeSec);
      nextState = TRIGGER;
      break;

    case TRIGGER:
      /* Trigger the servo to stop the flow of the liquid */
      Serial.println("Trigger servo");
      triggerServo(activeServo);
      nextState = INIT;
      break;

    default:
      break;
  }
}
