#include "FastLED.h"
#include <ArduinoJson.h>


//--------LED----------
#define PIN_RBGLED 4
#define NUM_LEDS 1
CRGB leds[NUM_LEDS];
int r=0,g=0,b=0;
//--------------------


//-------Ultrasonido--------------
#define TRIG_PIN 13  
#define ECHO_PIN 12  
//--------------------------------

//-------Inflarrojo---------------
#define PIN_ITR20001_LEFT   A2
#define PIN_ITR20001_MIDDLE A1
#define PIN_ITR20001_RIGHT  A0

#define FORWARD 100
#define GIRO 100
#define RECORD 80

#define START_LAP '0'
#define END_LAP '1'
#define OBSTACLE_DETECTED '2'
#define LINE_LOST '3'
#define PING '4'
#define INIT_LINE_SEARCH '5'
#define STOP_LINE_SEARCH '6'
#define LINE_FOUND '7' 
#define VISIBLE_LINE '8'

//-------Motor------------------
// Enable/Disable motor control.
//  HIGH: motor control enabled
//  LOW: motor control disabled
#define PIN_Motor_STBY 3

// Group A Motors (Right Side)
// PIN_Motor_AIN_1: Digital output. HIGH: Forward, LOW: Backward
#define PIN_Motor_AIN_1 7
// PIN_Motor_PWMA: Analog output [0-255]. It provides speed.
#define PIN_Motor_PWMA 5

// Group B Motors (Left Side)
// PIN_Motor_BIN_1: Digital output. HIGH: Forward, LOW: Backward
#define PIN_Motor_BIN_1 8
// PIN_Motor_PWMB: Analog output [0-255]. It provides speed.
#define PIN_Motor_PWMB 6

// Mas de 400 es linea detectada
const int min_threshold = 400;
int left_val;
int right_val;
int mid_val;
bool end = false;
bool perdido = false;
int start_lap;
int end_lap;
int linea_vista = 0;
int distance;
int giro = 0;
float t; //timepo que demora en llegar el eco
int d; //distancia en centimetros
int tiempo_ping = 4000;
int prev_time = 0;
int time_final;
int cant_line = 0;
int iterations = 0;
//-----------------------------


uint32_t Color(uint8_t r, uint8_t g, uint8_t b)
{
  return (((uint32_t)r << 16) | ((uint32_t)g << 8) | b);
}

void led(String color){
  if(color == "rojo"){
    r=255;
    g=0;
    b=0;

  } else if (color == "verde"){
    r=0;
    g=255;
    b=0;

  } else if (color == "amarillo"){
    r=255;
    g=255;
    b=0;
    
  } else if (color == "rosa"){
    r=255;
    g=192;
    b=203;
    
  } else if (color == "azul"){
    r=0;
    g=0;
    b=255;
    
  } else if (color == "marron"){
    r=165;
    g=42;
    b=42;
    
  } else if (color == "blanco"){
    r=255;
    g=255;
    b=255;
    
  } else if (color == "apagar"){
    r=0;
    g=0;
    b=0;
    
  } else {
    Serial.print("Error");
  }

  FastLED.showColor(Color(r, g, b));
}

// Function to control motors
void motorControl(bool motorAForward, int speedA, bool motorBForward, int speedB) {
  // Control motor A (RIGHT)
  digitalWrite(PIN_Motor_AIN_1, motorAForward ? HIGH : LOW);
  analogWrite(PIN_Motor_PWMA, speedA);

  // Control motor B (LEFT)
  digitalWrite(PIN_Motor_BIN_1, motorBForward ? HIGH : LOW);
  analogWrite(PIN_Motor_PWMB, speedB);
}

void sensorReading() {
  left_val = analogRead(A2); // read from left Sensor
  right_val = analogRead(A0);
  mid_val =  analogRead(A1);
}

void turnLeft(int vel){
  led("amarillo");
  motorControl(true,vel,true,vel/4);
}
void turnRight(int vel){
  led("azul");
  motorControl(true,vel/4,true,vel);
}
void forward(int vel){
  led("verde");
  motorControl(true,vel,true,vel);
}
void stop_motors() {
  motorControl(false,0,false,0);
}

void setup() {
  
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  //ultrasonidos
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  //inflarrojos
  pinMode(PIN_ITR20001_LEFT, INPUT);
  pinMode(PIN_ITR20001_MIDDLE, INPUT);
  pinMode(PIN_ITR20001_RIGHT, INPUT);
  //motors
  // Turn on the engines !!!!
  digitalWrite(PIN_Motor_STBY, HIGH);
  // Set motor control pins as outputs
  pinMode(PIN_Motor_STBY, OUTPUT);
  pinMode(PIN_Motor_AIN_1, OUTPUT);
  pinMode(PIN_Motor_PWMA, OUTPUT);
  pinMode(PIN_Motor_BIN_1, OUTPUT);
  pinMode(PIN_Motor_PWMB, OUTPUT);

  // LED
  FastLED.addLeds<NEOPIXEL, PIN_RBGLED>(leds, NUM_LEDS);
  FastLED.setBrightness(20);

  
  // comienza al recibir que tiene conexion
  while ( true ) {
    led("rosa");
    if (Serial.available() > 0) {
      break;
    }
  }

  Serial.print(START_LAP);

  start_lap = millis();
  prev_time = millis();

}



void loop() {

  // Cuenta las iteraciones
  iterations += 1;
  
  // Solo se mete aqui si no ha acabado
  if (end == false){

    //Ping
    if (millis() - prev_time >= tiempo_ping) {
      Serial.print(PING);
      Serial.println((millis() - prev_time));

    // Actualizar el tiempo anterior al actual
      prev_time = millis();
    }

    // Sensor de distancia
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(2);          //Enviamos un pulso de 10us
    digitalWrite(TRIG_PIN, LOW);

    t = pulseIn(ECHO_PIN, HIGH); //obtenemos el ancho del pulso
    d = t/59;

    // Si es menor de 8
    if ( d < 8 ) {
      // manda distancia detectada
      Serial.print(OBSTACLE_DETECTED);
      Serial.println(d);

      end_lap = millis();
      time_final = (end_lap - start_lap);

      stop_motors();
      
      //end lap
      Serial.print(END_LAP);
      Serial.println(time_final);


      Serial.print(VISIBLE_LINE);
      double linea_vista = cant_line/iterations;
      Serial.println(String(cant_line/iterations));

      end = true;

    }
    sensorReading();

    // si detecta por la izquierda
    if (left_val > min_threshold) { //LEFT
      cant_line += 1;

      // si antes ha pasado por recovery avisa que este ya a acabado
      if (perdido == true) {
        
        Serial.print(LINE_FOUND);

        Serial.print(STOP_LINE_SEARCH);
      }
      turnLeft(GIRO);
      // guarda que lo ultimo detectado 
      giro = 0;
      perdido = false;
      
    } else if (right_val > min_threshold ) { //RIGHT
      cant_line += 1;
      if (perdido == true) {
    
        Serial.print(LINE_FOUND);

        Serial.print(STOP_LINE_SEARCH);
      }
      turnRight(GIRO);
      // guarda que lo ultimo detectado 
      giro = 1;
      perdido = false;

    } else if (mid_val > min_threshold) {//FORWARD
      cant_line += 1;
      if (perdido == true) {
        
        Serial.print(LINE_FOUND);

        Serial.print(STOP_LINE_SEARCH);
      }

      forward(FORWARD); 
      perdido = false;
      
    } else { //RECOBERY
      // avisa que se a perdido
      if (perdido == false) {
        Serial.print(LINE_LOST);

        Serial.print(INIT_LINE_SEARCH);
      }

      led("rojo");
      perdido = true;
      // seun ultimo sensor detectado gira hacia alli 
      if (giro == 1) {

        motorControl(false,RECORD,true,RECORD);

      } else if (giro == 0) {

        motorControl(true,RECORD,false,RECORD);

      }
    }
  } else {
    stop_motors();
  }
}