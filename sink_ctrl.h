enum ServoName {
  R_SANITIZE,
  L_WASH,
  NUM_SERVOS
};

enum State {
  INIT,
  FILLING,
  TRIGGER
};
  

const uint8_t servoPins[NUM_SERVOS] = {9, 10};
const uint8_t initPosDeg = 7;
const uint8_t finalPosDeg = 120;
const uint16_t servoSettlingTimeMS = 800;
const uint8_t servoSanitizePin = 9;
const uint8_t servoWashPin = 10;
const uint8_t buttonSanitizerPin = 12;
const uint8_t buttonWashPin = 13;
const uint16_t waitTimeSec = 252;
const uint8_t debounceDelayMS = 240;
