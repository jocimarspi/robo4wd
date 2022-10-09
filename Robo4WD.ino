#include <HCSR04.h>               // INCLUI BIBLIOTECA HCSR04
#include <AFMotor.h>              // INCLUI A BIBLIOTECA DA MOTOR SHIELD
#include <SoftwareSerial.h>       // INCLUI SERIAL DO BLUETOOTH
SoftwareSerial bluetooth(A1, A0); // RX(TX DO MÓDULO BT), TX(RX DO MÓDULO BT)

#define FORWARD 1  //VALOR DA BIBLIOTECA 1
#define BACKWARD 2 //VALOR DA BIBLIOTECA 2

#define ACTION_FORWARD 'F'
#define ACTION_FORWARD_LEFT 'G'
#define ACTION_FORWARD_RIGHT 'I'
#define ACTION_BACKWARD 'B'
#define ACTION_BACKWARD_LEFT 'H'
#define ACTION_BACKWARD_RIGHT 'J'
#define ACTION_LEFT 'L'
#define ACTION_RIGHT 'R'

#define FRONT_RIGHT_MOTOR 1
#define FRONT_LEFT_MOTOR 2
#define REAR_LEFT_MOTOR 3
#define REAR_RIGHT_MOTOR 0

#define SPEED_0 0
#define SPEED_1 125
#define SPEED_2 140
#define SPEED_3 153
#define SPEED_4 168
#define SPEED_5 181
#define SPEED_6 196
#define SPEED_7 209
#define SPEED_8 224
#define SPEED_9 237
#define SPEED_MAX 255

AF_DCMotor motors[] = {(1), (2), (3), (4)};
byte motorsCurrentSpeed[] = {SPEED_3, SPEED_3, SPEED_3, SPEED_3};
byte motorsCurrentRotation[] = {RELEASE, RELEASE, RELEASE, RELEASE};

HCSR04 sensorDistancia(A2, A3); //PINO TRIGGER, PINO ECHO

char comando = 0;
float distancia;
byte currentSpeed = SPEED_4;

void setup()
{
  //Serial.begin(9600);
  bluetooth.begin(9600); //INICIA COMUNICAÇÃO SERIAL DO BLUETOOTH
  initializeMotors();
}

void loop()
{ 
  while (bluetooth.available())
  {
    comando = bluetooth.read();
    distancia = sensorDistancia.dist();

    //Serial.print("Comando: ");
    //Serial.println(comando);
    
    setSpeed();

    if (isForwardAction(comando))
    {
      moveForward(comando);
    }
    else if (isBackwardAction(comando))
    {
      moveBackward(comando);
    }
    else if (comando == ACTION_LEFT)
    {
      moveLeft();
    }
    else if (comando == ACTION_RIGHT)
    {
      moveRight();
    }
    else
    {
      stop();
    }
  }
}

byte getTotalMotors() {
  return sizeof(motors)/sizeof(motors[0]);
}

void initializeMotors()
{
  for (byte i = 0; i < getTotalMotors(); i++)
  {
    motors[i].setSpeed(motorsCurrentSpeed[i]);
    motors[i].run(motorsCurrentRotation[i]);
  }
}

void setMotorSpeed(byte speed, byte motorId)
{
  if (speed != motorsCurrentSpeed[motorId])
  {
    motors[motorId].setSpeed(speed);
    motorsCurrentSpeed[motorId] = speed;
  }
}

void setSpeed()
{
  switch (comando) {
    case '1': currentSpeed = SPEED_1; break;
    case '2': currentSpeed = SPEED_2; break;
    case '3': currentSpeed = SPEED_3; break;
    case '4': currentSpeed = SPEED_4; break;
    case '5': currentSpeed = SPEED_5; break;
    case '6': currentSpeed = SPEED_6; break;
    case '7': currentSpeed = SPEED_7; break;
    case '8': currentSpeed = SPEED_8; break;
    case '9': currentSpeed = SPEED_9; break;
    case 'q': currentSpeed = SPEED_MAX; break;
  }
}

/*void setLeftSideMotorsSpeed(byte speed)
{
  setMotorSpeed(speed, FRONT_LEFT_MOTOR);
  setMotorSpeed(speed, REAR_LEFT_MOTOR);
}

void setRightSideMotorsSpeed(byte speed)
{
  setMotorSpeed(speed, FRONT_RIGHT_MOTOR);
  setMotorSpeed(speed, REAR_RIGHT_MOTOR);
}*/

void setMotorRotation(byte motorId, byte rotation) {
  if (motorsCurrentRotation[motorId] != rotation) {
    motors[motorId].run(rotation);
    motorsCurrentRotation[motorId] = rotation;
  }
}

void setAllMotorRotation(byte rotation)
{
  for (byte i = 0; i < getTotalMotors(); i++) {
    setMotorSpeed(currentSpeed, i);
  }

  for (byte i = 0; i < getTotalMotors(); i++)
  {
    setMotorRotation(i, rotation);
  }
}

void setLeftSideMotorsRotation(byte rotation)
{
  setMotorSpeed(currentSpeed, FRONT_LEFT_MOTOR);
  setMotorSpeed(currentSpeed, REAR_LEFT_MOTOR);

  setMotorRotation(FRONT_LEFT_MOTOR, rotation);
  setMotorRotation(REAR_LEFT_MOTOR, rotation);
}

void setRightSideMotorsRotation(byte rotation)
{
  setMotorSpeed(currentSpeed, FRONT_RIGHT_MOTOR);
  setMotorSpeed(currentSpeed, REAR_RIGHT_MOTOR);

  setMotorRotation(FRONT_RIGHT_MOTOR, rotation);
  setMotorRotation(REAR_RIGHT_MOTOR, rotation);
}

bool isForwardAction(char action)
{
  return (action == ACTION_FORWARD) 
    || (action == ACTION_FORWARD_LEFT) 
    || (action == ACTION_FORWARD_RIGHT);
}

bool isBackwardAction(char action)
{
  return (action == ACTION_BACKWARD)
    || (action == ACTION_BACKWARD_LEFT)
    || (action == ACTION_FORWARD_RIGHT);
}

bool isNearObject()
{
  return distancia < 20;
}

void moveForward(char action)
{
  if (isNearObject())
  {
    stop();
    return;
  }

  if (action == ACTION_FORWARD)
  {
    setAllMotorRotation(FORWARD);    
  }
  if (action == ACTION_FORWARD_LEFT)
  {
    setAllMotorRotation(FORWARD);
  }
  else if (action == ACTION_FORWARD_RIGHT)
  {
    setAllMotorRotation(FORWARD);
  }  
}

void moveBackward(char action)
{
  if (action == ACTION_BACKWARD)
  {
    setAllMotorRotation(BACKWARD);
  }
  else if (action == ACTION_BACKWARD_LEFT)
  {
    setAllMotorRotation(BACKWARD);
  }
  else if (action == ACTION_BACKWARD_RIGHT)
  {
    setAllMotorRotation(BACKWARD);
  }  
}

void moveRight()
{
  setLeftSideMotorsRotation(FORWARD);
  setRightSideMotorsRotation(BACKWARD);
}
void moveLeft()
{
  setLeftSideMotorsRotation(BACKWARD);
  setRightSideMotorsRotation(FORWARD);
}

void stop()
{
  setAllMotorRotation(RELEASE);
}
