// RC_BLINDING_LIGHTS: Arduino based lights control for RC cars.

/*
  Copyright (C) 2021  Emilio Alvarez.

  This program is free software: you can redistribute it and / or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

// ***************************
// Lights calls for each conditions.
//*******************************

//*****************************
//HEADLIGHTS

void XenonOn() {
  analogWrite(6, 100);
  delay(50);
  analogWrite(6, 0);
  delay(50);
  analogWrite(6, 100);
  delay(50);
  analogWrite(6, 0);
  delay(50);
}

void XenonOff() {
  for (int i = 30; i <= 110; i++) {
    analogWrite(6, i);
    delay(2);
  }
  for (int i = 127; i >= 0; i--) {
    analogWrite(6, i);
    delay(15);
  }
}

//**************************
// TAIL LIGHTS
void Brake() {
  analogWrite(5, 180);
}

//*************************
//TURNING LIGHTS

void Left() {
  analogWrite(7, 500);
  delay(400);
  analogWrite(7, 0);
  delay (200);
}

void  Right() {
  analogWrite(8, 500);
  delay(400);
  analogWrite(8, 0);
  delay (200);
}

//*********************
// VARIABLES
//***********************

unsigned long previousMillis = 0;
unsigned long currentMillis;
unsigned long lastRxIn = 0;
int T = 0;
int T2 = 0;
bool Switch = false;

//********************************
// INPUT CHANNELS FROM RC TRANSMITTER
//*******************************
unsigned long RxIn;
unsigned long CH3;
unsigned long CH2;
unsigned long CH1;





// Setup function - executed once on startup
void setup() {

  //****************
  // OUTPUTS
  //****************

  pinMode(5, OUTPUT); // rear lights (PWM)
  digitalWrite(5, LOW);
  pinMode(6, OUTPUT); //  headlights (PWM)
  digitalWrite(6, LOW);
  pinMode(7, OUTPUT); //  left corner (PWM)
  digitalWrite(7, LOW);
  pinMode(8, OUTPUT); // right corner  (PWM)
  digitalWrite(8, LOW);
  pinMode(3, OUTPUT); // Exhaust  (PWM)
  digitalWrite(3, LOW);
  pinMode (4, OUTPUT); // Reverse Lights (PWM)
  digitalWrite (4, LOW);

  //**********************
  // INPUTS
  //*********************

  pinMode(2, INPUT); //receiver input (CH1)
  pinMode(10, INPUT); //receiver input (CH2)
  pinMode(12, INPUT); //receiver input (CH3)


  //SERIAL MONITOR FOR MONITORING CHANNEL SIGNALS. ONCE SETUP AND AJUSTED IT CAN BE SUPRESSED.
  Serial.begin(9600);
}

// Loop function - executed infinitely again and again..
void loop() {



  //*********************
  //MONITORING DISPLAY OF CHANNEL SIGNALS. ONCE SETUP AND AJUSTED CORRECTLY IT CAN BE SUPRESSED,
  //ESPECIALLY THE DELAY AND THE END OTHERWISE IT WILL DELAY ALL OTHER FUCNTIONS.
  //**************************

  CH1 = pulseIn(2, HIGH);
  CH2 = pulseIn(10, HIGH);
  CH3 = pulseIn(12, HIGH);

  Serial.print("CH1: ");
  Serial.print(CH1);
  Serial.print("     ");
  Serial.print("CH2: ");
  Serial.print(CH2);
  Serial.print("     ");
  Serial.print("CH3: ");
  Serial.print(CH3);
  Serial.print("\n");
  delay (100);

  //*****************************

  currentMillis = millis();

  //*********************
  // LIGHTS SWITCH ON
  //THIS IS THE CHANNEL WHERE YOU CAN TURN ON YOUR LIGHTS ON AND OFF. CHECK FOR THE SIGNAL VALUE.

  if (CH3 > 2000)
  {
    if (Switch == false) {
      XenonOn();
      Switch = true;
      analogWrite(6, 120);
      analogWrite(5, 20);
    }
  }


  //*********************
  //BRAKE LIGHTS
  //(DETERMINE A SLIGHLY GREATER THAN VALUE FROM STANDING STILL, AND LOWER THAN REVERSE ACTION.)

  if ((CH2 > 1520) && (CH2 < 1600) && (Switch == true)) {
    Brake();
  }
  else if (Switch == true) {
    analogWrite (5, 20);
  }

  //******************************
  // TURNING LIHGTS
  // (TURNING LIGHTS WILL BLINK AFTER 5 SECONDS CAR NOT MOVING. AFTER THAT EITHER STEERING LEFT OR RIGHT WILL BLINK THE TURN LIGHTS)
  //** THESE WILL WORK EVEN IF THE LIGHTS SWITHC IS TURNED OFF**

  if ((currentMillis - previousMillis) >= 2000 && (T == 0) && (CH2 > 1480))
  {
    previousMillis = currentMillis;
    T = 1;
  }
  if ((currentMillis - previousMillis) >= 1000 && (T == 1) && (CH2 > 1480))
  {
    previousMillis = currentMillis;
    T = 2;
  }
  if ((currentMillis - previousMillis) >= 1000 && (T == 2) && (CH2 > 1480))
  {
    previousMillis = currentMillis;
    T = 3;
  }
  if (T == 1 && CH2 < 1480)
  {
    T = 0;
  }
  if (T == 2 && CH2 < 1480)
  {
    T = 0;
  }
  if (T == 3 && CH2 < 1480)
  {
    T = 0;
  }
  if (CH1 > 1600 && CH2 > 1480 && CH2 < 1550 && T == 3)
  {
    Left();
  }
  if (CH1 < 1400 && CH2 > 1480 && CH2 < 1550 && T == 3)
  {
    Right();
  }

  //******************
  //REVERSE
  //(DETERMINE A VALUE WHERE THE CAR STARTS MOVING IN REVERSE)

  if ((CH2 > 1680) && (Switch == true)) {
    analogWrite (4, 80);
  }
  else {
    analogWrite (4, 0);
  }

  //************************
  //LIGHTS SWITHC OFF

  if ((CH3 < 2000) && (CH3 > 700)) {
    if (Switch == true) {
      XenonOff();
      Switch = false;
    }
    analogWrite(4, 0);
    analogWrite(5, 0);
    analogWrite(6, 0);
  }

  //*************************
  // EXHAUST DECEL BACKFIRE (ON EVEN IF LIGHTS SWITCH IS OFF)
  // WILL WORK ONCE LEFT OFF THE THROTTLE. ADJUST SIGNAL SENSITIBITY (lastRxIn + ##) AND SIGNAL VALUE WITHOUT GOING INTO REVERSE.

  RxIn = CH2;
  if (RxIn > (lastRxIn + 200) && (CH2 < 1550))

  {
    analogWrite (3, 127);
    delay (100);
    analogWrite (3, 0);
    delay (100);
    analogWrite (3, 127);
    delay (50);
    analogWrite (3, 0);
    delay (60);
    analogWrite (3, 127);
    delay (300);
    analogWrite (3, 0);
    delay (250);
  }
  lastRxIn = RxIn;
}
