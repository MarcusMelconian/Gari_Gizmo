/*
 *  ARDUINO CONTROLLER - MM - Final Version - 2/03/19
 *  Act on commands from nodeMCU over wire
 */

/*
 * Headers below read wire communication to Arduino, servo motor, standard
 * arduino, external JSON support functions, Adafruit library, Adafruit LED matrix backpack, automation state machine package
 */
#include <Wire.h>
#include <Servo.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <Automaton.h>

/*
 * Creating an LED matrix object
 * As the eyes copy one another only one object is required for the two
 */
Adafruit_8x8matrix matrix =  Adafruit_8x8matrix();

/*
 * In case we want to point output to another pipe instead of serialport
 */
#define USE_SERIAL Serial

/*
 * Gari commands
 */
#define GARI_ALIVE 1
#define GARI_FORWARD 2
#define GARI_BACKWARD 3
#define GARI_TURNLEFT 4
#define GARI_TURNRIGHT 5
#define GARI_SPINLEFT 6
#define GARI_SPINRIGHT 7
#define GARI_LOOKAROUND 8
#define GARI_GREETING 9
#define GARI_FEELING 10
#define GARI_JOKE 11
#define GARI_EXERCISE 12
#define GARI_BALL 13
#define GARI_RAVE 14

/*
 * Defining the pins 
 * 'IN' and 'EN' are referring to the H-Bridge motor controller
 */
int led1 = 3;
int led2 = 4;
int led3 = 5;

int buzzer = 2;

const int IN1 = 7;
const int IN2 = 8;
const int IN3 = 10;
const int IN4 = 12;

const int ENA = 6;
const int ENB = 11;

const int servoPin = 9;

/*
 * Servo object created and starting position set
 */
Servo GariHead;
int pos = 90;

/*
 * Defining the mini state machines for the interactions
 */
Atm_timer aliveMachine;
Atm_timer moveMachine;
Atm_timer turnMachine;
Atm_timer spinMachine;
Atm_timer lookAroundLeftMachine;
Atm_timer lookAroundRightMachine;
Atm_timer wheelShakeMachine;
Atm_timer eyeSadMachine;
Atm_timer eyeHappyMachine;
Atm_timer eyeExerciseMachine;
Atm_timer eyeRaveMachine;
Atm_timer eyeIdleMachine;

/*
 * Settings for each mini state machine
 * 
 */
#define M_ALIVE_TIME 250
#define M_ALIVE_STEPS 16
#define M_MOVE_TIME 2000
#define M_TURN_TIME 1550
#define M_SPIN_TIME 6200
#define M_LOOKAROUND_TIME 20
#define M_LOOKAROUNDLEFT_STEPS 89
#define M_LOOKAROUNDRIGHT_STEPS 179
#define M_WHEELSHAKE_TIME 400
#define M_WHEELSHAKE_STEPS 10
#define M_SAD_TIME 100
#define M_SAD_STEPS -1
#define M_HAPPY_TIME 75
#define M_HAPPY_STEPS -1
#define M_EXERCISE_TIME 75
#define M_EXERCISE_STEPS -1
#define M_RAVE_TIME 75
#define M_RAVE_STEPS -1 
#define M_IDLE_TIME 20
#define M_IDLE_STEPS -1

/*
 * Purpose: Wire io event
 * Parameters:  
 *    howMany: the length of the buffer - not used
 * Return:
 *    void
 */
void receiveEvent(int howMany) {
  String data = "";

  Serial.println(F("Wire data arrived"));   
 
  /*
   * Get data - reading one byte over wire at a time and adding to data string
   */
  while (0 < Wire.available()) {
    char c = Wire.read();      
    data += c;
  }

  Serial.println(data);  

  /*
   * Calls the main loop with the received data
   */
  processCall(data);         
}

/*
 * Purpose: Main processing loop
 * Parameters:  
 *    command: The command id received over websockets
 * Return:
 *    void
 */
void processCall(String command){

    Serial.println(F("Command arrived"));    
  
    /*
     * String is JSON format
     */
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root= jsonBuffer.parseObject(command);
    
    if (root.success()  ) {
      
      /*
       * Got a command from Gari
       */        
      int gariCommand = atoi(root[F("command")]);
      int gariCommandState = atoi(root[F("state")]);

      /*
       * Gari command received from server printed in serial monitor
       */
      Serial.print(F("New command arrived:"));      
      Serial.print(gariCommandState);
      Serial.println(gariCommand);    

      /*
       * Switch statements
       * The received Gari command will call it's specific switch statement
       * The switch statement then calls the desired interaction start function(s)
       */
      switch (gariCommand) {
        case GARI_ALIVE:
          Serial.print(F("Gari Alive.. "));           
          machineAliveStart();       
          break;        

        case GARI_FORWARD:
          Serial.print(F("Gari Forward.. "));         
          machineForwardStart();
          break;

        case GARI_BACKWARD:
          Serial.print(F("Gari Backward.."));
          machineBackwardStart();        
          break;

        case GARI_TURNLEFT:
          Serial.print(F("Gari turnLeft.. "));
          machineLeftStart();        
          break;

        case GARI_TURNRIGHT:
          Serial.print(F("Gari turnRight.. "));  
          machineRightStart();          
          break;

        case GARI_SPINLEFT:
          Serial.print(F("Gari spinLeft.. "));
          machineSpinLeftStart();         
          break;

        case GARI_SPINRIGHT:
          Serial.print(F("Gari spinRight.."));
          machineSpinRightStart();        
          break;

        case GARI_LOOKAROUND:
          Serial.print(F("Gari lookAround.. "));
          machineLookAroundLeftStart();
          break;

        case GARI_GREETING:
          Serial.print(F("Gari spinRight.."));
          machineGreetingStart();
          machineEyeHappyStart();
          machineAliveStart();
          break;

        case GARI_FEELING:
          Serial.print(F("Gari spinRight.. "));
          machineWheelShakeStart();
          machineEyeSadStart();
          break;

        case GARI_JOKE:
          Serial.print(F("Gari spinRight.. "));
          machineWheelShakeStart();
          machineEyeHappyStart();
          break;

        case GARI_EXERCISE:
          Serial.print(F("Gari spinRight.. "));
          machineExerciseStart();
          machineEyeExerciseStart();
          break;

        case GARI_BALL:
          Serial.print(F("Gari spinRight.. "));
          machineForwardStart();
          machineEyeExerciseStart();
          break;

        case GARI_RAVE:
           Serial.print(F("Gari spinRight.."));
          machineRaveStart();
          machineEyeRaveStart();
          break;
          
          
        default:           
          Serial.println(F("No handler for this command"));          
      }
    }

    USE_SERIAL.flush();    
}

/*
 * Purpose: Arduino startup function
 * Parameters:  
 *    none
 * Return:
 *    void
 */
void setup() {
  
    /*
     * Serial monitor and componenets are established
     */
    USE_SERIAL.begin(115200);
    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    randomSeed(analogRead(A0));
    matrix.begin(0x70);

    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3,OUTPUT);

    pinMode(buzzer, OUTPUT);

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);

    GariHead.attach(servoPin);
    
    /*
     *  Pause to allow modules and components to intialise
     *  Acts as a spacer
     */
    for(uint8_t t = 4; t > 0; t--) {
      USE_SERIAL.println(F("Booting  ...\n"));
      USE_SERIAL.flush();
      delay(1000);
    }    

    /*
     * Handy for testing...
     */
    pinMode(LED_BUILTIN, OUTPUT);

    /*
     * ic2 wire address 8
     */
    Wire.begin(8);      

    /*
     * Register wire event call-back
     */
    Wire.onReceive(receiveEvent);   

    /*
     * Register the control of the mini-states machines
     * Using the data provided in the settings at the top of the script
     * 'onFinish' provides the end on function and 'onTimer' provides the function called at each step interval
     * See https://github.com/tinkerspy/Automaton
     */
    aliveMachine.begin(M_ALIVE_TIME).repeat(M_ALIVE_STEPS).onTimer(machineAliveStep).onFinish(machineWheelOnFinish);
    moveMachine.begin(M_MOVE_TIME).onFinish(machineWheelOnFinish);
    turnMachine.begin(M_TURN_TIME).onFinish(machineWheelOnFinish);   
    spinMachine.begin(M_SPIN_TIME).onFinish(machineWheelOnFinish);   
    lookAroundLeftMachine.begin(M_LOOKAROUND_TIME).repeat(M_LOOKAROUNDLEFT_STEPS).onTimer(machineLookAroundLeftStep).onFinish(machineLookAroundRightStart);
    lookAroundRightMachine.begin(M_LOOKAROUND_TIME).repeat(M_LOOKAROUNDRIGHT_STEPS).onTimer(machineLookAroundRightStep).onFinish(machineWheelOnFinish);      
    wheelShakeMachine.begin(M_WHEELSHAKE_TIME).repeat(M_WHEELSHAKE_STEPS).onTimer(machineWheelShakeStep).onFinish(machineWheelOnFinish);
    eyeSadMachine.begin(M_SAD_TIME).repeat(M_SAD_STEPS).onTimer(machineEyeSadStep);
    eyeHappyMachine.begin(M_HAPPY_TIME).repeat(M_HAPPY_STEPS).onTimer(machineEyeHappyStep);
    eyeExerciseMachine.begin(M_EXERCISE_TIME).repeat(M_EXERCISE_STEPS).onTimer(machineEyeExerciseStep);
    eyeRaveMachine.begin(M_RAVE_TIME).repeat(M_RAVE_STEPS).onTimer(machineEyeRaveStep);      
    eyeIdleMachine.begin(M_IDLE_TIME).repeat(M_IDLE_STEPS).onTimer(machineEyeIdleStep);          

    /*
     * If no switch statements are ran - run standby function
     */
    machineWheelOnFinish();                          
   
}

/*
 *  Purpose: Initialising the state machine package
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void loop() {
  automaton.run();
}

/*
 *  Purpose: Standby state callback function after each interaction is complete
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineWheelOnFinish() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    machineEyeIdleStart();
}

/*
 *  Purpose: The step function for the Alive machine
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineAliveStep() {
  if (digitalRead(led1) == HIGH) {
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);       
  }
  else {
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
  }
}

/*
 *  Purpose: Initialising the parameters and starting the Alive state machine
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineAliveStart() {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    aliveMachine.start();
  
}

/*
 *  Purpose: Initialising the parameters and starting the Move state machine
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineForwardStart() {
    machineWheelInit();
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    moveMachine.start();    
    
}

/*
 *  Purpose: Initialising the parameters and starting the Move state machine
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineBackwardStart() {
    machineWheelInit(); 
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    moveMachine.start().onFinish(machineWheelOnFinish);   
}

/*
 *  Purpose: Initialising the parameters and starting the Turn state machine
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineLeftStart() {
    machineWheelInit();   
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    turnMachine.start();    
}

/*
 *  Purpose: Initialising the parameters and starting the Turn state machine
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineRightStart() {
    machineWheelInit();
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    turnMachine.start();    
}

/*
 *  Purpose: Initialising the parameters and starting the Spin state machine
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineSpinLeftStart() {
    machineWheelInit();
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    spinMachine.start().onFinish(machineWheelOnFinish);    
}

/*
 *  Purpose: Initialising the parameters and starting the Spin state machine
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineSpinRightStart() {
    machineWheelInit();
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    spinMachine.start().onFinish(machineWheelOnFinish);    
}

/*
 *  Purpose: The left step function for the Look Around machine
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineLookAroundLeftStep() {
      if (pos > 0) { 
      pos -= 1; 
      GariHead.write(pos);
    }
}

/*
 *  Purpose: Initialising the parameters and starting the Look Around LEFT state machine
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineLookAroundLeftStart() {
    pos = 90;
    GariHead.write(pos);
    lookAroundLeftMachine.start();
}

/*
 *  Purpose: The right step function for the Look Around machine
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineLookAroundRightStep() {
      if (pos < 180) {
      pos += 1; 
      GariHead.write(pos);      
    }
}

/*
 *  Purpose: Initialising the parameters and starting the Look Around RIGHT state machine
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineLookAroundRightStart() {
   pos = 0;
   GariHead.write(pos);
   lookAroundRightMachine.start();
  
}

/*
 *  Purpose: Initialising the parameters and starting the move state machine
 *           Notice here how its overiding the standard set moveMachine and calling the Backward start function on completion
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineGreetingStart() {
    machineWheelInit();
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    moveMachine.start().onFinish(machineBackwardStart);
}

/*
 *  Purpose: The step function for the Wheel Shake machine
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineWheelShakeStep() {
  if (digitalRead(IN1) == LOW) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }

  else {
     digitalWrite(IN1, LOW);
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, LOW);
  }
}

/*
 *  Purpose: Initialising the parameters and starting the Wheel Shake state machine
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineWheelShakeStart() {
     machineWheelInit();   
     digitalWrite(IN1, LOW);
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, LOW);
     wheelShakeMachine.start().onFinish(machineWheelOnFinish);
}

/*
 *  Purpose: Initialising the parameters and starting the Spin state machine
 *           Notice here how its overiding the standard set spinMachine and calling the Wheel Shake start function on completion
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineExerciseStart() {
    machineWheelInit();
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    spinMachine.start().onFinish(machineWheelShakeStart);
}

/*
 *  Purpose: Initialising the parameters and starting the Spin state machine
 *           Notice here how its overiding the standard set spinMachine and calling the Spin Right start function on completion
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineRaveStart() {
    machineWheelInit();
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    spinMachine.start().onFinish(machineSpinRightStart);
}

/*
 * Storing the bitmaps in the PROGMEM for the 5 eye states
 */
static const uint8_t PROGMEM 
  blinkImg[][8] = {    
  { B00111100,        
    B01111110,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100 },
  { B00000000,
    B01111110,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100 },
  { B00000000,
    B00000000,
    B00111100,
    B11111111,
    B11111111,
    B11111111,
    B00111100,
    B00000000 },
  { B00000000,
    B00000000,
    B00000000,
    B00111100,
    B11111111,
    B01111110,
    B00011000,
    B00000000 },
  { B00000000,         
    B00000000,
    B00000000,
    B00000000,
    B10000001,
    B01111110,
    B00000000,
    B00000000 } };

static const uint8_t PROGMEM
  rave[][8] = {
  { B00111100,         
    B01111110,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100 },
  { B00000000,         
    B00111100,
    B01111110,
    B11111111,
    B11111111,
    B01111110,
    B00111100,
    B00000000 },
  { B00000000,         
    B00000000,
    B00111100,
    B01111110,
    B01111110,
    B00111100,
    B00000000,
    B00000000 }, 
  { B00000000,         
    B00000000,
    B00000000,
    B00111100,
    B00111100,
    B00000000,
    B00000000,
    B00000000 }, 
  { B00000000,         
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000 }
  };

static const uint8_t PROGMEM
  happy[][8] = {
  { B00111100,         
    B01111110,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100 },
  { B00000000,         
    B00111100,
    B01111110,
    B11111111,
    B11111111,
    B01111110,
    B00111100,
    B00000000 },
  { B00000000,         
    B00000000,
    B00000000,
    B11111111,
    B00000000,
    B00000000,
    B00000000,
    B00000000 }, 
  { B00000000,         
    B00000000,
    B11111111,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000 }, 
  { B00000000,         
    B11111111,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000 },
  { B11111111,         
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000 }
  };

static const uint8_t PROGMEM
  sad[][8] = {
  { B00111100,         
    B01111110,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100 },
  { B00000000,         
    B00111100,
    B01111110,
    B11111111,
    B11111111,
    B01111110,
    B00111100,
    B00000000 },
  { B00000000,         
    B00000000,
    B00000000,
    B00000000,
    B11111111,
    B00000000,
    B00000000,
    B00000000 }, 
  { B00000000,         
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B11111111,
    B00000000,
    B00000000 }, 
  { B00000000,         
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B11111111,
    B00000000 },
  { B00000000,         
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B11111111 }
  };

static const uint8_t PROGMEM
  exercise[][8] = {
  { B00111100,         
    B01111110,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100 },
  { B00000000,         
    B00111100,
    B01111110,
    B11111111,
    B11111111,
    B01111110,
    B00111100,
    B00000000 },
  { B00000000,         
    B00000000,
    B00111100,
    B01111110,
    B01111110,
    B00111100,
    B00000000,
    B00000000 }, 
  { B00000000,         
    B00000000,
    B01111000,
    B11111100,
    B11111100,
    B01111000,
    B00000000,
    B00000000 }, 
  { B00000000,         
    B00000000,
    B00111100,
    B01111110,
    B01111110,
    B00111100,
    B00000000,
    B00000000 },
  { B00000000,         
    B00000000,
    B00011110,
    B00111111,
    B00111111,
    B00011110,
    B00000000,
    B00000000 }
  };

/*
 * Idle eye variables
 */
uint8_t
  blinkIndex[] = { 1, 2, 3, 4, 3, 2, 1 }, // Blink bitmap sequence
  blinkCountdown = 100, // Countdown to next blink (in frames)
  gazeCountdown  =  75, // Countdown to next eye movement
  gazeFrames     =  50;  // Duration of eye movement (smaller = faster)

int8_t
  eyeX = 3, eyeY = 3,   // Current eye position
  newX = 3, newY = 3,   // Next eye position
  dX   = 0, dY   = 0;   // Distance from prior to new position

/*
 * Sad vars
 */
int sadIndex = 0;

/*
 * Happy vars
 */
int happyIndex = 0;

/*
 * Exercise vars
 */
int exerciseIndex = 0;

/*
 * Rave vars
 */
int raveIndex = 0;

/*
 *  Purpose: Initialising function for the eyes
 *           Called before any new eye machine begins      
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */   
void machineEyeInit() {
  
  /*
   * Stop all eye machines and clear matrix
   */
  eyeSadMachine.stop();
  eyeHappyMachine.stop();
  eyeExerciseMachine.stop();
  eyeRaveMachine.stop();   
  eyeIdleMachine.stop(); 
  matrix.clear();  

  /*
   * Idle variables reset
   */
  blinkCountdown = 100; 
  gazeCountdown =  75; 
  gazeFrames =  50;  
  eyeX = 3; 
  eyeY = 3; 
  newX = 3; 
  newY = 3;   
  dX   = 0; 
  dY   = 0; 

  /*
   * Reset sad
   */
  sadIndex = 0;
  
  /*
   * Reset happy
   */
  happyIndex = 0;

  /*
   * Rest exercise
   */
  exerciseIndex = 0;

  /*    
   * Reset rave
   */
  raveIndex = 0;
}

/*
 *  Purpose: The step function for the Sad Eyes machine
 *           The bitmap is updated and ran through the loop in line with the state machine settings
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineEyeSadStep() {
    matrix.clear();
    matrix.drawBitmap(0,0, sad[sadIndex++], 8, 8, LED_ON);
    matrix.writeDisplay();
    if (sadIndex == 6) {
      sadIndex = 2;
    }
}

/*
 *  Purpose: Initialising the parameters and starting the Sad Eyes state machine
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineEyeSadStart() {
    machineEyeInit();
    eyeSadMachine.start();
}

/*
 *  Purpose: The step function for the Happy Eyes machine
 *           The bitmap is updated and ran through the loop in line with the state machine settings
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineEyeHappyStep() {
    matrix.clear();
    matrix.drawBitmap(0, 0, happy[happyIndex++], 8, 8, LED_ON);
    matrix.writeDisplay();
    if (happyIndex == 6) {
      happyIndex = 2;
    }
}

/*
 *  Purpose: Initialising the parameters and starting the Happy Eyes state machine
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineEyeHappyStart() {
    machineEyeInit();
    eyeHappyMachine.start();
}

/*
 *  Purpose: The step function for the Exercise Eyes machine
 *           The bitmap is updated and ran through the loop in line with the state machine settings
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineEyeExerciseStep() {
    matrix.clear();
    matrix.drawBitmap(0, 0, exercise[exerciseIndex++], 8, 8, LED_ON);
    matrix.writeDisplay();
    if (exerciseIndex == 6) {
      exerciseIndex = 2;
    }
}

/*
 *  Purpose: Initialising the parameters and starting the Exercise Eyes state machine
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineEyeExerciseStart() {
    machineEyeInit();
    eyeExerciseMachine.start();
}

/*
 *  Purpose: The step function for the Rave Eyes machine
 *           The bitmap is updated and ran through the loop in line with the state machine settings
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineEyeRaveStep(int idx, int v, int up){   
    matrix.clear();    
    matrix.drawBitmap(0, 0, rave[raveIndex++], 8, 8, LED_ON);
    matrix.writeDisplay(); 
    if (raveIndex == 5) {
      raveIndex = 0;
    }
}

/*
 *  Purpose: Initialising the parameters and starting the Rave Eyes state machine
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineEyeRaveStart(){
    machineEyeInit(); 
    eyeRaveMachine.start();
}

/*
 *  Purpose: The step function for the Idle Eyes machine
 *           The bitmap is updated and ran through the loop in line with the state machine settings
 *           THIS FUNCTION WAS OBTAINED VIA THE ADAFRUIT LIBRARIES
 *  Parameters:  
 *    The 3 inputs are not used
 *  Return:
 *    void
 */
void machineEyeIdleStep(int idx, int v, int up){       
    // When counting down to the next blink, show the eye in the fully-
    // open state.  On the last few counts (during the blink), look up
    // the corresponding bitmap index.
    matrix.clear();    
    matrix.drawBitmap(0, 
                      0,
                      blinkImg[
                        (blinkCountdown < sizeof(blinkIndex)) ? // Currently blinking?
                        blinkIndex[blinkCountdown] :            // Yes, look up bitmap #
                        0                                       // No, show bitmap 0
                      ], 8, 8, LED_ON);
    // Decrement blink counter.  At end, set random time for next blink.
    if(--blinkCountdown == 0) {
      blinkCountdown = random(5, 180);
    }
        
    // Add a pupil (2x2 black square) atop the blinky eyeball bitmap.
    // Periodically, the pupil moves to a new position...
    if(--gazeCountdown <= gazeFrames) {
      // Eyes are in motion - draw pupil at interim position
      matrix.fillRect(
      newX - (dX * gazeCountdown / gazeFrames),
      newY - (dY * gazeCountdown / gazeFrames),
      2, 2, LED_OFF);
      
      if(gazeCountdown == 0) {    // Last frame?
        eyeX = newX; eyeY = newY; // Yes.  What's new is old, then...
        
        do { // Pick random positions until one is within the eye circle
            newX = random(7); newY = random(7);
            dX   = newX - 3;  dY   = newY - 3;
        } while((dX * dX + dY * dY) >= 10);      // Thank you Pythagoras
        
        dX            = newX - eyeX;             // Horizontal distance to move
        dY            = newY - eyeY;             // Vertical distance to move
        gazeFrames    = random(3, 15);           // Duration of eye movement
        gazeCountdown = random(gazeFrames, 120); // Count to end of next movement
      }
      
    } else {
      // Not in motion yet -- draw pupil at current static position
      matrix.fillRect(eyeX, eyeY, 2, 2, LED_OFF);
    }
    matrix.writeDisplay();
}

/*
 *  Purpose: Initialising the parameters and starting the Idle Eyes state machine
 *  Parameters:  
 *    none
 *  Return:
 *    void
 */
void machineEyeIdleStart(){
    machineEyeInit(); 
    eyeIdleMachine.start();
}
