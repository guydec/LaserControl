// ------------------------------------------------------------------------------------------------------------
//
//                                                 ----------------
//                                          L A S E R   C O N T R O L L E R
//                                                 ----------------
//
//    Arduino UNO
//
//    Démarrage power supply par clé
//    Contrôle de la puissance
//    Affichage de la température du laser
//    Gestion surchauffe laser
//    fonction Focus ou passe la main à la cnc
//    Arrêt d'urgence
// 
//    Guy Dechamps
//    gdechamps@hotmail.com
//
//
// ------------------------------------------------------------------------------------------------------------
#define Version "B 0.1"
#define Date "10/16"


// ------------------------------------------------------------------------------------------------------------
//                                                LIBRARIES
// ------------------------------------------------------------------------------------------------------------
#include <LiquidCrystal.h>
#include "Thermometre.h"
#include "RotaryActuator.h"

// ------------------------------------------------------------------------------------------------------------
//                                            PIN ASSIGNATION
// ------------------------------------------------------------------------------------------------------------
//         DIGITAL -----------
//         libre ...         0
#define   PIN_ENCODER_BUTTON   8                                         // push button switch
#define   PIN_ENCODER1         2
#define   PIN_ENCODER2         3                                         // .. 2/3 are special pins
#define   Relay           4
#define   FeedHold        5
#define   lcdRS           6
#define   lcdEN           7
#define   StepperEnable   8
#define   StartKey        9
#define   LaserOut       10                                         // Timer 1 "B" output: OC1B
//         libre ...        11
#define   SpindleEnable  12
#define   PIN_ONEWIRE_BUS 13

//         ANALOGIQUE ----------
#define   lcdD4          14
#define   lcdD5          15
#define   lcdD6          16
#define   lcdD7          17
#define   Buzzer         18  

// ------------------------------------------------------------------------------------------------------------
//                                                OTHER DEFINES
// ------------------------------------------------------------------------------------------------------------
#define TEMPERATURE_ALARM 27
#define DELAY_ACCUEIL 2500

// ------------------------------------------------------------------------------------------------------------
//                                              Variables
// ------------------------------------------------------------------------------------------------------------
Thermometre* Th1;

RotaryActuator* Ra1;

LiquidCrystal lcd(lcdRS, lcdEN, lcdD4, lcdD5, lcdD6, lcdD7);


const long     TIMER1_OCR1A_Setting = 16000000L / 700L;

// Flags
boolean         EncoderSwitch_flag;
boolean         StartKey_flag;
boolean         SpindleEnable_flag;
boolean         StepperEnable_flag;
boolean         LcdAccueilMenu_flag = true;
boolean         LcdStartMenu_flag = false;
boolean         LcdStartMenuL2_flag = true;
boolean         LcdMainMenu_flag = false;
boolean         LcdGrblMenu_flag = false;
boolean         KeyOn_flag = false;
boolean         Percent_flag = false;
boolean         Temperature_flag = false;
boolean         Laser_flag = false;
boolean         LaserOn_flag = true;
boolean         LaserOff_flag = true;
boolean         RotaryInUse_flag = false;
boolean         TemperatureAlarm_flag = false;

int             lastMSB = 0;
int             lastLSB = 0;
unsigned int    Val = 0;
volatile int    lastEncoded = 0;
byte            Duty = 12;
long            Percent = 0;                                        // Percent and PercentTemp =!
long            PercentTemp = 1;                                    // .. for the first lcd print            
long            lastencoderValue = 0;
long            coder = 0;
unsigned long   currentMillis;
unsigned long   previousMillis;
unsigned long   previousMillisRotary = 0;
unsigned long   previousMillisDebounce;
volatile long   encoderValue = 0;

bool LaserStatus = false;
bool OldLaserStatus = false;

// ------------------------------------------------------------------------------------------------------------
//                                                 SETUP
// ------------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("setup...");

  Th1 = new Thermometre(PIN_ONEWIRE_BUS);
  Th1->SetResolution(12);

  Ra1 = new RotaryActuator(PIN_ENCODER1, PIN_ENCODER2, PIN_ENCODER_BUTTON);
  Ra1->SetMinValue(0);
  Ra1->SetMaxValue(100);
  Ra1->SetStep(2);
  Ra1->DebouceDelayForButton = 1;
  Ra1->ResetButtonStatus();

  // pinMode INPUT   
  pinMode(SpindleEnable, INPUT);
  //pinMode(StepperEnable, INPUT);
  pinMode(StartKey, INPUT);
  pinMode(FeedHold, INPUT);

  // pinMode OUTPUT
  pinMode(Relay, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(LaserOut, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(Buzzer, OUTPUT);

  // digitalWrite
  digitalWrite(Relay, HIGH);
  digitalWrite(StartKey, HIGH);
  digitalWrite(FeedHold, HIGH);

// Lcd custom char
  byte selection[8] = {
    0b11000,
    0b11100,
    0b11110,
    0b11111,
    0b11110,
    0b11100,
    0b11000,
    0b00000
  };
  lcd.createChar(1, selection);

  byte degres[8] = {
    0b01000,
    0b10100,
    0b01000,
    0b00011,
    0b00100,
    0b00100,
    0b00011,
    0b00000
  };
  lcd.createChar(2, degres);

  // Initialisations   
  lcd.begin(16, 2);
  lcd.home();
  lcd.clear();

  currentMillis = millis();
  previousMillis = millis();



}



// ------------------------------------------------------------------------------------------------------------
//                                                 LOOP
// ------------------------------------------------------------------------------------------------------------
void loop() {

  // Lecture des entrées
  
  if (digitalRead(StartKey) == LOW) {
    StartKey_flag = true;
  }

  if (digitalRead(SpindleEnable) == HIGH) {
    SpindleEnable_flag = true;
  }

  // Menu Accueil   
  if (LcdAccueilMenu_flag == true) {
    LcdAccueilMenu();
    LcdAccueilMenu_flag = false;
    delay(DELAY_ACCUEIL);
    LcdStartMenu_flag = true;
  }

  

  // Menu Start   
  if (LcdStartMenu_flag == true) {
    currentMillis = millis();
    if ((currentMillis - previousMillis) >= 800) {
      previousMillis = currentMillis;
      lcdStartMenu();
      if (StartKey_flag == true) {
        digitalWrite(Relay, LOW);
        LcdStartMenu_flag = false;
        LcdMainMenu_flag = true;
      }
    }
  }


  // Menu Principal   
  if (LcdMainMenu_flag == true) {
    LcdMainMenu();
    LcdMainMenu_flag = false;
    Percent_flag = true;
    Temperature_flag = true;
    Laser_flag = true;
  }


  // Menu GRBL   
  if (LcdGrblMenu_flag == true) {
    LcdGrblMenu();
  }


  if (Percent_flag == true) {
    Percent = Ra1->GetCurrentValue();
    
    if (PercentTemp != Percent) {                                   // on doit afficher le %

      lcd.setCursor(11, 1);
      if (Percent > 99) {
        lcd.print("max ");
      } else if (Percent < 1) {
        lcd.print("min ");
      } else {
        lcd.print(Percent);
        lcd.print(" % ");
      }

      PercentTemp = Percent;
    }
  }

  if (Ra1->ButtonPushedThenReleased()) {
    Serial.println("changing laser status");
    OldLaserStatus = LaserStatus;
    LaserStatus = !LaserStatus;
  }
  

/////////////////////////////////////////////////////////////////////////////
// Reading and display laser temperature
/////////////////////////////////////////////////////////////////////////////
  Th1->ReadTemperatureAsync();
  if (Th1->DataReady && Th1->DataChanged && (Temperature_flag)) {
    DisplayTemperature(Th1->Temperature);
  }

  // Laser   
  if (LaserStatus != OldLaserStatus) {
    lcd.setCursor(6, 0);
    if (LaserStatus) {
      //Beep(2);
      lcd.print("on ");
    } else {
      //Beep(0);
      lcd.print("off");
    }
    
  }

  //Ra1->GetButtonStatus();



}





// ------------------------------------------------------------------------------------------------------------
//                                                 FONCTIONS
// ------------------------------------------------------------------------------------------------------------


// Menu d'acceuil
void LcdAccueilMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Laser Controller");
  lcd.setCursor(0, 1);
  lcd.print("Ver.");
  lcd.setCursor(5, 1);
  lcd.print(Version);
  lcd.setCursor(11, 1);
  lcd.print(Date);
}


// Menu Start
void lcdStartMenu() {
  LcdStartMenuL2_flag = !LcdStartMenuL2_flag;
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("DC power off");
  if (LcdStartMenuL2_flag == true) {
    lcd.setCursor(0, 1);
    lcd.print("wait for key on");
  }
}


// Menu principal
void LcdMainMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Laser off");
  lcd.setCursor(15, 0);
  lcd.write(2);                                                     // custom character
  lcd.setCursor(0, 1);
  lcd.print("Intensity ");
}


// Menu GRBL
void LcdGrblMenu() {
}


// Affichage de la température   
void DisplayTemperature(float temperature) {
  lcd.setCursor(10, 0);
  lcd.print(temperature, 2);
}


// Start PWM
void PWMstart() {
  TCCR1A = _BV(WGM10) | _BV(WGM11) | _BV(COM1B1);                // fast PWM, clear OC1B on compare
  TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS10);                  // fast PWM, no prescaler
  OCR1A = TIMER1_OCR1A_Setting - 1;                                // zero relative  
}


// STop PWM
void PWMstop() {
  TCCR1A = 0;                                                      // set entire TCCR1A register to 0
  TCCR1B = 0;                                                      // same for TCCR1B
}


//Rotary Encoder
//void GetEncoderValue() {
//  int MSB = digitalRead(encoderPin1);                               // MSB = most significant bit
//  int LSB = digitalRead(encoderPin2);                               // LSB = least significant bit
//  int encoded = (MSB << 1) | LSB;                                    // converting the 2 pin value to single number
//  int sum = (lastEncoded << 2) | encoded;                          // adding it to the previous encoded value
//  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue++;
//  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue--;
//  lastEncoded = encoded;                                            // store this value for next time
//  if (encoderValue >= 50) {
//    encoderValue = 50;
//  } else if (encoderValue <= 0) {
//    encoderValue = 0;
//  }
//}


void GetRotarySwitch() {
  Beep(5);
}

// Beep 1200 hz  
void Beep(int Repeat) {
  for (int i = 0; i <= Repeat; i++) {
    tone(Buzzer, 1200);
    delay(200);
    noTone(Buzzer);
    delay(100);
  }
}



void PrintAlarms(DeviceAddress deviceAddress) {
  //char temp;
  //temp = sensors.getHighAlarmTemp(deviceAddress);
  //Serial.print("High Alarm: ");
  //Serial.print(temp, DEC);
  //Serial.print("C");
}





void checkAlarm(DeviceAddress deviceAddress) {
  //if (sensors.hasAlarm(deviceAddress)) {
  //  lcd.print("ALARM: ");
  //}
}












// ---------------------------------------------------------------------------------------------------- END --




