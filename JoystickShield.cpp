/**
   JoystickShield - Arduino Library for JoystickShield (http://hardwarefun.com/projects/joystick-shield)

   Copyright 2011  Sudar Muthu  (email : sudar@sudarmuthu.com)
 *
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <sudar@sudarmuthu.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer or coffee in return - Sudar
 * ----------------------------------------------------------------------------
  Confirgured for Nokia 5110 Joystick shield v2.4. (http://www.elecfreaks.com/wiki/index.php?title=Joystick_Shield)
  remapped button pins. added 2 additional 'START' and "SELECT' buttons
  modified yLow threshold

  danielelijahsmith@gmail.com
  https://github.com/RoboDonut
 */

#include "JoystickShield.h"

/**
 * Constructor
 *
 */
JoystickShield::JoystickShield() {
    // define some threshold values. Change them if your Joystick is different
    //int xLow, int xHigh, int yLow, int yHigh
    setThreshold(510, 530, 490, 540);

    // Sparkfun Joystick shield connects the Joystick to Pins 0 and 1.
    // Change it if you are using a different shield
    setJoystickPins(0, 1);

    // Sparkfun Joystick shield connects the buttons to the following pins.
    // change it if you are using a different shield.
    // pinSelect (joystick), pinUp, pinRight, pinDown, pinLeft, 
    setButtonPins(8, 2, 3, 4, 5,6,7); // 6 is button E and 7 is button F

    // by default set the position to centered
    currentStatus = CENTER;

    // by default set the button state to NO_BUTTON
    currentButton = NO_BUTTON;

    // initialize all callback function pointers to NULL
    initializeCallbacks();
}

/**
 * Set Analyog pins which are connected to the Joystick
 *
 */
void JoystickShield::setJoystickPins(byte pinX, byte pinY) {
    pin_analog_x = pinX;
    pin_analog_y = pinY;

    // set Joystick pins to input mode
    pinMode(pin_analog_x, INPUT);
    pinMode(pin_analog_y, INPUT);
}

/**
 * Set the pins used by the buttons
 * modify setbutton pins function inputs to: byte pinJoy, byte pinUp, byte pinRight, byte pinDown, byte pinLeft, byte pinSelect, byte pinStart
 */
void JoystickShield::setButtonPins(byte pinJoy, byte pinUp, byte pinRight, byte pinDown, byte pinLeft, byte pinSelect, byte pinStart) {
    pin_joystick_button = pinJoy;
    pin_up_button       = pinUp;
    pin_right_button    = pinRight;
    pin_down_button     = pinDown;
    pin_left_button     = pinLeft;
	pin_select_button     = pinSelect;
    pin_start_button     = pinStart;

    // set Button pins to input mode
    pinMode(pin_joystick_button, INPUT);
    pinMode(pin_up_button      , INPUT);
    pinMode(pin_right_button   , INPUT);
    pinMode(pin_down_button    , INPUT);
    pinMode(pin_left_button    , INPUT);
	pinMode(pin_select_button    , INPUT);
	pinMode(pin_start_button    , INPUT);

    // Enable "pull-up resistors" for buttons
    digitalWrite(pin_joystick_button, HIGH);
    digitalWrite(pin_up_button      , HIGH);
    digitalWrite(pin_right_button   , HIGH);
    digitalWrite(pin_down_button    , HIGH);
    digitalWrite(pin_left_button    , HIGH);
	digitalWrite(pin_select_button    , HIGH);
	digitalWrite(pin_start_button    , HIGH);
}

/**
 * Configure threshold values for Joystick
 *
 */
void JoystickShield::setThreshold(int xLow, int xHigh, int yLow, int yHigh) {
    x_threshold_low  = xLow;
    x_threshold_high = xHigh;
    y_threshold_low  = yLow;
    y_threshold_high = yHigh;
}

/**
 * Process Events. This should be called in the loop()
 *
 */
void JoystickShield::processEvents() {
    int x_direction = 0;
    int y_direction = 0;

    // read from Joystick pins
    int x_position = analogRead(pin_analog_x);
    int y_position = analogRead(pin_analog_y);

    // determine Joystick direction
    if (x_position > x_threshold_high) {
        x_direction = 1;
    } else if (x_position < x_threshold_low) {
        x_direction = -1;
    }

    if (y_position > y_threshold_high) {
        y_direction = 1;
    } else if (y_position < y_threshold_low) {
        y_direction = -1;
    }

    if (x_direction == -1) {
        if (y_direction == -1) {
            currentStatus = LEFT_DOWN;
        } else if (y_direction == 0) {
            currentStatus = LEFT;
        } else {
            currentStatus = LEFT_UP;
        }
    } else if (x_direction == 0) {
        if (y_direction == -1) {
            currentStatus = DOWN;
        } else if (y_direction == 0) {
            currentStatus = CENTER;
        } else {
            currentStatus = UP;
        }
    } else {
        if (y_direction == -1) {
            currentStatus = RIGHT_DOWN;
        } else if (y_direction == 0) {
            currentStatus = RIGHT;
        } else {
            currentStatus = RIGHT_UP;
        }
    }

    // Determine which buttons were pressed
    if (digitalRead(pin_joystick_button) == LOW) {
        currentButton = JOYSTICK_BUTTON;
    }

    if (digitalRead(pin_up_button) == LOW) {
        currentButton = UP_BUTTON;
    }

    if (digitalRead(pin_right_button) == LOW) {
        currentButton = RIGHT_BUTTON;
    }

    if (digitalRead(pin_down_button) == LOW) {
        currentButton = DOWN_BUTTON;
    }

    if (digitalRead(pin_left_button) == LOW) {
        currentButton = LEFT_BUTTON;
    }
	if (digitalRead(pin_select_button) == LOW) {
        currentButton = SELECT_BUTTON;
    }
	if (digitalRead(pin_start_button) == LOW) {
        currentButton = START_BUTTON;
    }

}


void JoystickShield::processCallbacks() {
    processEvents();

    // Joystick Callbacks
    if (isCenter() && centerCallback != NULL) {
        centerCallback();
    }

    if (isUp() && upCallback != NULL) {
        upCallback();
    }

    if (isRightUp() && rightUpCallback != NULL) {
        rightUpCallback();
    }

    if (isRight() && rightCallback != NULL) {
        rightCallback();
    }

    if (isRightDown() && rightDownCallback != NULL) {
        rightDownCallback();
    }

    if (isDown() && downCallback != NULL) {
        downCallback();
    }

    if (isLeftDown() && leftDownCallback != NULL) {
        leftDownCallback();
    }

    if (isLeft() && leftCallback != NULL) {
        leftCallback();
    }

    if (isLeftUp() && leftUpCallback != NULL) {
        leftUpCallback();
    }

    // Button Callbacks
    if (isJoystickButton() && jsButtonCallback != NULL) {
        jsButtonCallback();
    }

    if (isUpButton() && upButtonCallback != NULL) {
        upButtonCallback();
    }

    if (isRightButton() && rightButtonCallback != NULL) {
        rightButtonCallback();
    }

    if (isDownButton() && downButtonCallback != NULL) {
        downButtonCallback();
    }

    if (isLeftButton() && leftButtonCallback != NULL) {
        leftButtonCallback();
    }
	if (isSelectButton() && selectButtonCallback != NULL) {
        selectButtonCallback();
    }
	if (isStartButton() && startButtonCallback != NULL) {
        startButtonCallback();
    }
}

/**
 * Joystick in Center status
 *
 */
bool JoystickShield::isCenter() {
    if (currentStatus == CENTER ) {
        return true;
    } else {
        return false;
    }
}

/**
 * Joystick in Up status
 *
 */
bool JoystickShield::isUp() {
    if (currentStatus == UP ) {
        return true;
    } else {
        return false;
    }
}

/**
 * Joystick in Right Up status
 *
 */
bool JoystickShield::isRightUp() {
    if (currentStatus == RIGHT_UP ) {
        return true;
    } else {
        return false;
    }
}

/**
 * Joystick in Right status
 *
 */
bool JoystickShield::isRight() {
    if (currentStatus == RIGHT ) {
        return true;
    } else {
        return false;
    }
}

/**
 * Joystick in Right Down status
 *
 */
bool JoystickShield::isRightDown() {
    if (currentStatus == RIGHT_DOWN ) {
        return true;
    } else {
        return false;
    }
}

/**
 * Joystick in Down status
 *
 */
bool JoystickShield::isDown() {
    if (currentStatus == DOWN ) {
        return true;
    } else {
        return false;
    }
}

/**
 * Joystick in Left Down status
 *
 */
bool JoystickShield::isLeftDown() {
    if (currentStatus == LEFT_DOWN ) {
        return true;
    } else {
        return false;
    }
}

/**
 * Joystick in Left status
 *
 */
bool JoystickShield::isLeft() {
    if (currentStatus == LEFT ) {
        return true;
    } else {
        return false;
    }
}

/**
 * Joystick in Left Up status
 *
 */
bool JoystickShield::isLeftUp() {
    if (currentStatus == LEFT_UP) {
        return true;
    } else {
        return false;
    }
}

/**
 * Joystick button pressed
 *
 */
bool JoystickShield::isJoystickButton() {
    if (currentButton == JOYSTICK_BUTTON) {
        clearButtonStates();
        return true;
    } else {
        return false;
    }
}

/**
 * Up button pressed
 *
 */
bool JoystickShield::isUpButton() {
    if (currentButton == UP_BUTTON) {
        clearButtonStates();
        return true;
    } else {
        return false;
    }
}

/**
 * Right button pressed
 *
 */
bool JoystickShield::isRightButton() {
    if (currentButton == RIGHT_BUTTON) {
        clearButtonStates();
        return true;
    } else {
        return false;
    }
}

/**
 * Down button pressed
 *
 */
bool JoystickShield::isDownButton() {
    if (currentButton == DOWN_BUTTON) {
        clearButtonStates();
        return true;
    } else {
        return false;
    }
}

/**
 * Left button pressed
 *
 */
bool JoystickShield::isLeftButton() {
    if (currentButton == LEFT_BUTTON) {
        clearButtonStates();
        return true;
    } else {
        return false;
    }
}
/**
 * Select button pressed
 *
 */
bool JoystickShield::isSelectButton() {
    if (currentButton == SELECT_BUTTON) {
        clearButtonStates();
        return true;
    } else {
        return false;
    }
}
/**
 * Start button pressed
 *
 */
bool JoystickShield::isStartButton() {
    if (currentButton == START_BUTTON) {
        clearButtonStates();
        return true;
    } else {
        return false;
    }
}

/**
 * Joystick Callbacks
 *
 */
/****************************************************************** */
void JoystickShield::onJSCenter(void (*centerCallback)(void)) {
    this->centerCallback = centerCallback;
}

void JoystickShield::onJSUp(void (*upCallback)(void)) {
    this->upCallback = upCallback;
}

void JoystickShield::onJSRightUp(void (*rightUpCallback)(void)) {
    this->rightUpCallback = rightUpCallback;
}

void JoystickShield::onJSRight(void (*rightCallback)(void)) {
    this->rightCallback = rightCallback;
}

void JoystickShield::onJSRightDown(void (*rightDownCallback)(void)) {
    this->rightDownCallback = rightDownCallback;
}

void JoystickShield::onJSDown(void (*downCallback)(void)) {
    this->downCallback = downCallback;
}

void JoystickShield::onJSLeftDown(void (*leftDownCallback)(void)) {
    this->leftDownCallback = leftDownCallback;
}

void JoystickShield::onJSLeft(void (*leftCallback)(void)) {
    this->leftCallback = leftCallback;
}

void JoystickShield::onJSLeftUp(void (*leftUpCallback)(void)) {
    this->leftUpCallback = leftUpCallback;
}

/****************************************************************** */

/**
 * Button Callbacks
 *
 */
 /****************************************************************** */
void JoystickShield::onJoystickButton(void (*jsButtonCallback)(void)) {
    this->jsButtonCallback = jsButtonCallback;
}

void JoystickShield::onUpButton(void (*upButtonCallback)(void)) {
    this->upButtonCallback = upButtonCallback;
}

void JoystickShield::onRightButton(void (*rightButtonCallback)(void)) {
    this->rightButtonCallback = rightButtonCallback;
}

void JoystickShield::onDownButton(void (*downButtonCallback)(void)) {
    this->downButtonCallback = downButtonCallback;
}

void JoystickShield::onLeftButton(void (*leftButtonCallback)(void)) {
    this->leftButtonCallback = leftButtonCallback;
}
void JoystickShield::onSelectButton(void (*selectButtonCallback)(void)) {
    this->selectButtonCallback = selectButtonCallback;
}
void JoystickShield::onStartButton(void (*startButtonCallback)(void)) {
    this->startButtonCallback = startButtonCallback;
}
/****************************************************************** */

/**
 * Clear the current button state
 *
 */
void JoystickShield::clearButtonStates() {
    currentButton = NO_BUTTON;
}

/**
 * Initialize all Function pointers to NULL
 *
 */
void JoystickShield::initializeCallbacks() {
    // Joystick callbacks
    centerCallback      = NULL;
    upCallback          = NULL;
    rightUpCallback     = NULL;
    rightCallback       = NULL;
    rightDownCallback   = NULL;
    downCallback        = NULL;
    leftDownCallback    = NULL;
    leftCallback        = NULL;
    leftUpCallback      = NULL;

    // Button callbacks
    jsButtonCallback    = NULL;
    upButtonCallback    = NULL;
    rightButtonCallback = NULL;
    downButtonCallback  = NULL;
    leftButtonCallback  = NULL;
	selectButtonCallback  = NULL;
	startButtonCallback  = NULL;
}
/**
 * Calibrate Joystick
 *
 */
void JoystickShield::calibrateShield(){
	bool xHigh= false;
	bool xLow= false;
	bool yHigh= false;
	bool yLow= false;

	int newXhigh = 0;
	int newXlow = 0;
	int newYhigh = 0;
	int newXlow = 0;

	while (isCenter() == false){//determine if calbration is needed
		//determine what needs to be calibrated
		if (isRightUp() || isRight() || isRightDown()){
			xHigh = true;
			if isRightUp(){
				yHigh = true;
			}
			if isRightDown(){
				yLow = true;
			}
		}
		else if (isLeftUp() || isLeft() || isLiftDown()) {
			xLow = true;
			if isLefttUp(){
				yHigh = true;
			}
			if isLeftDown(){
				yLow = true;
			}
		}
		else if (isUp){
			yHigh = true
		}
		else if (isDown()){
			yLow = true
		}
		else{
			//something is dreadfully wrong
		}

		
		// adjust  values
		if (xHigh){
			//adjust x_threshold_high
			x_threshold_high += 5;
		}
		if (xLow){
			//adjust x_threshold_low
			x_threshold_low += 5;
		}
		if (yHigh){
			//adjust y_threshold_high
			y_threshold_high += 5;
		}
		if (yLow){
			//adjust y_threshold_low
			y_threshold_low +=5;
		}
		
	}
}