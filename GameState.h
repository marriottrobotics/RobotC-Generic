#define RED true //Should be used if the robot is on the red alliance.
#define BLUE false //Should be used if the robot is on the blue alliance.
#define TOP true //Indicates the robot is on the "top" starting tile which is arbitrarly chosen.
#define BOTTOM false //Indicates the robot is on the "bottom" starting tile which is arbitrarly chosen.

void initGameState(bool colour, bool end); //If you use the macros with this method it makes sense with natural language.

bool isStartRed(); //If the starting side is red, this is true. Otherwise it's false.
bool isStartBlue(); //If the starting side is blue, this is true. Otherwise it's false.

bool isStartTop(); //If we are starting at the top, this is true. Otherwise it's false.
bool isStartBottom(); //If we are starting at the bottom, this is true. Otherwise, it's false.

bool startRed;
bool startTop;

/*Call in the initalization if sensors are used to define these values.
* if this method isn't called, then nothing is going to work.*/
void initGameState(bool colour, bool end){
  startRed = colour;
  startTop = end;
}

bool isStartRed(){
  if(startRed){
    return true;
  }
  return false;
}
bool isStartBlue(){
  if(!startRed){
    return true;
  }
  return false;
}

bool isStartTop(){
  if(startTop){
    return true;
  }
  return false;
}
bool isStartBottom(){
  if(!startTop){
    return true;
  }
  return false;
}
