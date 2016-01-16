#include <AFMotor.h>

int Lspeed = 0;
int Rspeed = 0;
String inputString = "";
String inputString1 = "";
String inputStringSave = "";
boolean stringComplete = false;
boolean stringComplete1 = false;
boolean setSpeeds = false;
int16_t x = 0;
int16_t y = 0;
int Ldirection;
int Rdirection;


const uint16_t motorSpeedTableL[9][5] ={
{0,128,255,255,255},
{0,96,192,192,192},
{0,96,128,128,128},
{-96,0,96,96,128},
{-128,-96,0,96,128},
{-128,-96,-96,0,0},
{-96,-96,-128,-128,-128},
{0,-96,-192,-192,-192},
{0,-128,-255,-255,-255}};

const int16_t motorSpeedTableR[9][5] ={
{255,255,255,128,0},
{192,192,192,96,0},
{128,128,128,96,0},
{128,96,96,0,-96},
{128,96,0,-96,-128},
{0,0,-96,-96,-128},
{-128,-128,-128,-96,-96},
{-192,-192,-192,-96,0},
{-255,-255,-255,-128,0}};

AF_DCMotor Lmotor(1,MOTOR12_8KHZ);
AF_DCMotor Rmotor(2,MOTOR12_8KHZ);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  inputString.reserve(200);
  inputString1.reserve(200);
  inputStringSave.reserve(200);
}

void loop() {
  // put your main code here, to run repeatedly:

    String xString;
    String yString;
    
  
  // read JS positions from the serial connection and set Left and Right motor speeds based on table
  serialEvent();
  serialEvent1();
  if(stringComplete)
  {
    inputStringSave = inputString;
    inputString = "";
    stringComplete = false;
    setSpeeds = true;

  }
  else if(stringComplete1)
  {
    inputStringSave = inputString1;
    inputString1 = "";
    stringComplete1 = false;
    setSpeeds = true;

  }
  //Serial.println(inputStringSave);
  
  if(setSpeeds)
  {
    if(inputStringSave.startsWith("X"))
    {
      xString = inputStringSave.substring(1, 4);
      x = map(xString.toInt(), 0, 240, 0, 4);
      //x = x + 2;
      Serial.print("X ");
      Serial.println(x);
    }
    if(inputStringSave.startsWith("Y"))
    {
      yString = inputStringSave.substring(1, 4);
      y = map(yString.toInt(), 0, 240, 0, 8);
      //y = y + 4;
      Serial.print("Y ");
      Serial.println(y);

    }
    Lspeed = motorSpeedTableL[y][x];
    Rspeed = motorSpeedTableR[y][x]; 

    if (Lspeed<0)
    {
      Lspeed = Lspeed*-1;
      Ldirection = FORWARD;
    }
    else
    {
      Ldirection = BACKWARD;
    }

    if (Rspeed<0)
    {
      Rspeed = Rspeed*-1;
      Rdirection = FORWARD;
    }
    else
    {
      Rdirection = BACKWARD;
    }

    setSpeeds = false;
    inputStringSave = "";
  }


  Lmotor.setSpeed(Lspeed);
  Rmotor.setSpeed(Rspeed);

  Lmotor.run(Ldirection);
  Rmotor.run(Rdirection);
  
  delay(50);
}

void serialEvent(){
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n')
    {
      stringComplete = true;
    }
  }
}

void serialEvent1(){
  while (Serial1.available())
  {
    char inChar = (char)Serial1.read();
    inputString1 += inChar;
    if (inChar == '\n')
    {
      stringComplete1 = true;
    }
  }
}

 
