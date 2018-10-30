//Pi, to probably more decimal places than we'll need.
#define PI 3.14159265359

//Allows the user to instruct the movement what size wheels it will be using as well as what motors are for
void setupMovements(struct pid *left1, struct pid *left2, struct pid *right1, struct pid *right2, float ticksPerRot, float diameter, float robotDiameter);
//Helper method to tell the movement what the drive motors are without including wheel information.
void defineDriveMotors(struct pid *left1, struct pid *left2, struct pid *right1, struct pid *right2);
//Helper method to tell the movement what the wheel dimensions are without including the drive motors.
void defineWheels(float ticksPerRot, float diameter);
void defineRobot(float wheelDiameter, float robotDiameter, float ticksPerRot);

/*Moves the robot a distance based on the ticks imput.
* Variable:
*   ticks - the number of ticks to move in a direction. Positive is forward, negative is backwards.
*   waitTarget - If true, this method will return after the action is completed. Otherwise, it will return immediatly.
*/
void drive(int ticks, bool waitTarget);
/*Moves the robot a distance forward based on the ticks imput.
* Variable:
*   ticks - the number of ticks to move in a direction. Should be positive.
*   waitTarget - If true, this method will return after the action is completed. Otherwise, it will return immediatly.
*/
void driveForwards(int ticksIn, bool waitTarget);
/*Moves the robot a distance backwards based on the ticks imput.
* Variable:
*   ticks - the number of ticks to move in a direction. Should be positive.
*   waitTarget - If true, this method will return after the action is completed. Otherwise, it will return immediatly.
*/
void driveBackwards(int ticksIn, bool waitTarget);
/*Moves the robot a distance inwards (towards center) based on the ticks imput.
* Variable:
*   ticks - the number of ticks to move in a direction. Should be positive.
*   waitTarget - If true, this method will return after the action is completed. Otherwise, it will return immediatly.
*/
void driveIn(int ticksIn, bool waitTarget);
/*Moves the robot a distance outwards (from center) based on the ticks imput.
* Variable:
*   ticks - the number of ticks to move in a direction. Should be positive.
*   waitTarget - If true, this method will return after the action is completed. Otherwise, it will return immediatly.
*/
void driveOut(int ticksIn, bool waitTarget);
/*Moves the robot the number of centimeters indicated using the values passed from setupMovements()
* Variable:
*   cm - the number of centimeters to move in a direction. Positive is forward, negative is backwards.
*   waitTarget - If true, this method will return after the action is completed. Otherwise, it will return immediatly.
*/
void driveCm(float cm, bool waitTarget);
/*Turns the robot a defined number of ticks "up" towards the top of the game board.
*   Requires game state to be properly configured.
*/
void turnUp(int ticks, bool waitTarget);
/*Turns the robot a defined number of ticks "down" towards the top of the game board.
*   Requires game state to be properly configured.
*/
void turnDown(int ticks, bool waitTarget);

void turnDegrees(float degrees, bool waitTarget);

/*Turns the robot a defined number of degrees "up" towards the top of the game board.
*   Requires game state to be properly configured.
*/
void turnUpDeg(float degrees, bool waitTarget);
/*Turns the robot a defined number of degrees "down" towards the top of the game board.
*   Requires game state to be properly configured.
*/
void turnDownDeg(float degrees, bool waitTarget);

struct pid *leftDriveA;
struct pid *leftDriveB;
struct pid *rightDriveA;
struct pid *rightDriveB;

float ticksPerCm;

float ticksPerDegree;

void waitForTarget(struct pid *left, struct pid *right);

void setupMovements(struct pid *left1, struct pid *left2, struct pid *right1, struct pid *right2, float ticksPerRot, float diameter, float robotDiameter){
  defineDriveMotors(left1, left2, right1, right2);
  defineWheels(ticksPerRot, diameter);
  defineRobot(diameter, robotDiameter, ticksPerRot);
}

void defineDriveMotors(struct pid *left1, struct pid *left2, struct pid *right1, struct pid *right2){
  leftDriveA = left1;
  leftDriveB = left2;
  rightDriveA = right1;
  rightDriveB = right2;
}

void defineWheels(float ticksPerRot, float diameter){
  ticksPerCm = ticksPerRot / diameter * PI;
}

void defineRobot(float wheelDiameter, float robotDiameter, float ticksPerRot){
  float top = (robotDiameter * PI) / (wheelDiameter * PI) * ticksPerRot;
  ticksPerDegree = top/360.0;
}

//Positive is forwards, negative is backwards.
void drive(int ticks, bool waitTarget){
  driveForwards(ticks, waitTarget);
}

void driveForwards(int ticksIn, bool waitTarget){
  leftDriveA->mtarget += ticksIn;
  leftDriveB->mtarget += ticksIn;
  rightDriveA->mtarget += ticksIn;
  rightDriveB->mtarget += ticksIn;

  if(waitTarget){
    waitForTarget(leftDriveA, rightDriveA);
  }
}

void driveBackwards(int ticksIn, bool waitTarget){
  leftDriveA->mtarget -= ticksIn;
  leftDriveB->mtarget -= ticksIn;
  rightDriveA->mtarget -= ticksIn;
  rightDriveB->mtarget -= ticksIn;

  if(waitTarget){
    waitForTarget(leftDriveA, rightDriveA);
  }
}

void driveIn(int ticksIn, bool waitTarget){
	if(isStartRed()){
		driveForwards(ticksIn, waitTarget);
	}else{
		driveBackwards(ticksIn, waitTarget);
	}
}

void driveOut(int ticksIn, bool waitTarget){
	if(isStartRed()){
		driveBackwards(ticksIn, waitTarget);
	}else{
		driveForwards(ticksIn, waitTarget);
	}
}

void driveCm(float cm, bool waitTarget){
  leftDriveA->mtarget += cm*ticksPerCm;
  leftDriveB->mtarget += cm*ticksPerCm;
  rightDriveA->mtarget -= cm*ticksPerCm;
  rightDriveB->mtarget -= cm*ticksPerCm;

  if(waitTarget){
    waitForTarget(leftDriveA, rightDriveA);
  }
}

void turnUp(int ticks, bool waitTarget){
  if(isStartRed()){
    rightDriveA->mtarget += ticks;
    rightDriveB->mtarget += ticks;
    leftDriveA->mtarget -= ticks;
    leftDriveB->mtarget -= ticks;
  }else if(isStartBlue()){
    rightDriveA->mtarget -= ticks;
    rightDriveB->mtarget -= ticks;
    leftDriveA->mtarget += ticks;
    leftDriveB->mtarget += ticks;
  }

  if(waitTarget){
    waitForTarget(leftDriveA, rightDriveA);
  }
}

void turnDown(int ticks, bool waitTarget){
  if(isStartRed()){
    rightDriveA->mtarget -= ticks;
    rightDriveB->mtarget -= ticks;
    leftDriveA->mtarget += ticks;
    leftDriveB->mtarget += ticks;
  }else if(isStartBlue()){
    rightDriveA->mtarget += ticks;
    rightDriveB->mtarget += ticks;
    leftDriveA->mtarget -= ticks;
    leftDriveB->mtarget -= ticks;
  }

  if(waitTarget){
    waitForTarget(leftDriveA, rightDriveA);
  }
}

void turnDegrees(float degrees, bool waitTarget){
  rightDriveA->mtarget += degrees * ticksPerDegree;
  rightDriveB->mtarget += degrees * ticksPerDegree;
  leftDriveA->mtarget -= degrees * ticksPerDegree;
  leftDriveB->mtarget -= degrees * ticksPerDegree;

  if(waitTarget){
    waitForTarget(leftDriveA, rightDriveA);
  }
}

void turnUpDeg(float degrees, bool waitTarget){
  if(isStartRed()){
    rightDriveA->mtarget += degrees*ticksPerDegree;
    rightDriveB->mtarget += degrees*ticksPerDegree;
    leftDriveA->mtarget -= degrees*ticksPerDegree;
    leftDriveB->mtarget -= degrees*ticksPerDegree;
  }else if(isStartBlue()){
    rightDriveA->mtarget -= degrees*ticksPerDegree;
    rightDriveB->mtarget -= degrees*ticksPerDegree;
    leftDriveA->mtarget += degrees*ticksPerDegree;
    leftDriveB->mtarget += degrees*ticksPerDegree;
  }

  if(waitTarget){
    waitForTarget(leftDriveA, rightDriveA);
  }
}

void turnDownDeg(float degrees, bool waitTarget){
  if(isStartRed()){
    rightDriveA->mtarget -= degrees*ticksPerDegree;
    rightDriveB->mtarget -= degrees*ticksPerDegree;
    leftDriveA->mtarget += degrees*ticksPerDegree;
    leftDriveB->mtarget += degrees*ticksPerDegree;
  }else if(isStartBlue()){
    rightDriveA->mtarget += degrees*ticksPerDegree;
    rightDriveB->mtarget += degrees*ticksPerDegree;
    leftDriveA->mtarget -= degrees*ticksPerDegree;
    leftDriveB->mtarget -= degrees*ticksPerDegree;
  }

  if(waitTarget){
    waitForTarget(leftDriveA, rightDriveA);
  }
}

void waitForTarget(struct pid *left, struct pid *right){
  wait_ontarget(left);
  wait_ontarget(right);
}
