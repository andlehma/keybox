#include <Bounce2.h>
#include <LUFA.h>
#include "LUFAConfig.h"
#include "Joystick.h"

#define MILLIDEBOUNCE 1

#define UP_PIN 0
#define DOWN_PIN 1
#define LEFT_PIN 2
#define RIGHT_PIN 3
#define B_PIN 4
#define A_PIN 5
#define Y_PIN 6
#define X_PIN 7
#define RB_PIN 8
#define LB_PIN 9
#define RT_PIN 10
#define LT_PIN 14
#define START_PIN 15
#define SELECT_PIN 16
#define HOME_PIN 18

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define B 4
#define A 5
#define Y 6
#define X 7
#define RB 8
#define LB 9
#define RT 10
#define LT 11
#define START 12
#define SELECT 13
#define HOME 14

#define DPAD_UP_MASK 0x00
#define DPAD_UPRIGHT_MASK 0x01
#define DPAD_RIGHT_MASK 0x02
#define DPAD_DOWNRIGHT_MASK 0x03
#define DPAD_DOWN_MASK 0x04
#define DPAD_DOWNLEFT_MASK 0x05
#define DPAD_LEFT_MASK 0x06
#define DPAD_UPLEFT_MASK 0x07
#define DPAD_NOTHING_MASK 0x08
#define A_MASK 0x04
#define B_MASK 0x02
#define X_MASK 0x08
#define Y_MASK 0x01
#define LB_MASK 0x10
#define RB_MASK 0x20
#define LT_MASK 0x40
#define RT_MASK 0x80
#define START_MASK 0x200
#define SELECT_MASK 0x100
#define HOME_MASK 0x1000

byte buttonStatus[15];

Bounce up = Bounce();
Bounce down = Bounce();
Bounce left = Bounce();
Bounce right = Bounce();
Bounce b = Bounce();
Bounce a = Bounce();
Bounce y = Bounce();
Bounce x = Bounce();
Bounce lb = Bounce();
Bounce rb = Bounce();
Bounce lt = Bounce();
Bounce rt = Bounce();
Bounce start = Bounce();
Bounce select = Bounce();
Bounce home = Bounce();

void setupPins() {
  up.attach(UP_PIN, INPUT_PULLUP);
  down.attach(DOWN_PIN, INPUT_PULLUP);
  left.attach(LEFT_PIN, INPUT_PULLUP);
  right.attach(RIGHT_PIN, INPUT_PULLUP);
  b.attach(B_PIN, INPUT_PULLUP);
  a.attach(A_PIN, INPUT_PULLUP);
  y.attach(Y_PIN, INPUT_PULLUP);
  x.attach(X_PIN, INPUT_PULLUP);
  lb.attach(LB_PIN, INPUT_PULLUP);
  rb.attach(RB_PIN, INPUT_PULLUP);
  lt.attach(LT_PIN, INPUT_PULLUP);
  rt.attach(RT_PIN, INPUT_PULLUP);
  start.attach(START_PIN, INPUT_PULLUP);
  select.attach(SELECT_PIN, INPUT_PULLUP);
  home.attach(HOME_PIN, INPUT_PULLUP);

  up.interval(MILLIDEBOUNCE);
  down.interval(MILLIDEBOUNCE);
  left.interval(MILLIDEBOUNCE);
  right.interval(MILLIDEBOUNCE);
  b.interval(MILLIDEBOUNCE);
  a.interval(MILLIDEBOUNCE);
  y.interval(MILLIDEBOUNCE);
  x.interval(MILLIDEBOUNCE);
  lb.interval(MILLIDEBOUNCE);
  rb.interval(MILLIDEBOUNCE);
  lt.interval(MILLIDEBOUNCE);
  rt.interval(MILLIDEBOUNCE);
  start.interval(MILLIDEBOUNCE);
  select.interval(MILLIDEBOUNCE);
  home.interval(MILLIDEBOUNCE);
}

void setup() {
  setupPins();
  SetupHardware();
  GlobalInterruptEnable();
}

void buttonRead() {
  if (up.update()) { buttonStatus[UP] = up.fell(); }
  if (down.update()) { buttonStatus[DOWN] = down.fell(); }
  if (left.update()) { buttonStatus[LEFT] = left.fell(); }
  if (right.update()) { buttonStatus[RIGHT] = right.fell(); }
  if (b.update()) { buttonStatus[B] = b.fell(); }
  if (a.update()) { buttonStatus[A] = a.fell(); }
  if (y.update()) { buttonStatus[Y] = y.fell(); }
  if (x.update()) { buttonStatus[X] = x.fell(); }
  if (lb.update()) { buttonStatus[LB] = lb.fell(); }
  if (rb.update()) { buttonStatus[RB] = rb.fell(); }
  if (lt.update()) { buttonStatus[LT] = lt.fell(); }
  if (rt.update()) { buttonStatus[RT] = rt.fell(); }
}

void processButtons() {
  // neutralize analog sticks
  ReportData.LX = 128;
  ReportData.LY = 128;
  ReportData.RX = 128;
  ReportData.RY = 128;
  
  // socd cleaning
  // buttonStatus is the raw button data
  // directions is the cleaned directional output data
  byte directions[4];
  directions[UP] = buttonStatus[UP];
  directions[DOWN] = buttonStatus[DOWN];
  directions[LEFT] = buttonStatus[LEFT];
  directions[RIGHT] = buttonStatus[RIGHT];
  
  if (buttonStatus[UP] && buttonStatus[DOWN]) {
    directions[DOWN] = false;
  }
  if (buttonStatus[LEFT] && buttonStatus[RIGHT]) {
    directions[LEFT] = false;
    directions[RIGHT] = false;
  }
  
  // dpad inputs
  if (directions[UP] && directions[RIGHT]) { ReportData.HAT = DPAD_UPRIGHT_MASK; }
  else if (directions[UP] && directions[LEFT]) { ReportData.HAT = DPAD_UPLEFT_MASK; }
  else if (directions[DOWN] && directions[RIGHT]) { ReportData.HAT = DPAD_DOWNRIGHT_MASK; }
  else if (directions[DOWN] && directions[LEFT]) { ReportData.HAT = DPAD_DOWNLEFT_MASK; }
  else if (directions[UP]) { ReportData.HAT = DPAD_UP_MASK; }
  else if (directions[RIGHT]) { ReportData.HAT = DPAD_RIGHT_MASK; }
  else if (directions[DOWN]) { ReportData.HAT = DPAD_DOWN_MASK; }
  else if (directions[LEFT]) { ReportData.HAT = DPAD_LEFT_MASK; }
  else { ReportData.HAT = DPAD_NOTHING_MASK; }
  
  // buttons
  if (buttonStatus[B]) { ReportData.Button |= B_MASK; }
  if (buttonStatus[A]) { ReportData.Button |= A_MASK; }
  if (buttonStatus[Y]) { ReportData.Button |= Y_MASK; }
  if (buttonStatus[X]) { ReportData.Button |= X_MASK; }
  if (buttonStatus[LB]) { ReportData.Button |= LB_MASK; }
  if (buttonStatus[RB]) { ReportData.Button |= RB_MASK; }
  if (buttonStatus[LT]) { ReportData.Button |= LT_MASK; }
  if (buttonStatus[RT]) { ReportData.Button |= RT_MASK; }
  if (buttonStatus[START]) { ReportData.Button |= START_MASK; }
  if (buttonStatus[SELECT]) { ReportData.Button |= SELECT_MASK; }
  if (buttonStatus[HOME]) { ReportData.Button |= HOME_MASK; }
}

void loop() {
  buttonRead();
  processButtons();
  HID_Task();
  USB_USBTask();
}
