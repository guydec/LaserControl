   /**************************************************************/
   /* POT controlled software PWM                                */
   /*                                                            */
   /* Code written by William Hart, 2011                         */
   /* http://www.williamhart.info                                */
   /*                                                            */
   /* This is a very simple circuit which uses a potentiometer   */
   /* to set an analog input and then uses this input to drive a */
   /* PWM signal using a standard (non-PWM) port on the Arduino. */
   /**************************************************************/

   //Serial.println(microsecondsToClockCycles(1)); // gives a result of 16 clock cycles per microsecond

   // define pins
   #define ADJ_PIN 1 // Adjustment pin is analog 0
   #define PWM_PIN 8 // PWM output pin is digital 8

   // setup PWM values
   #define PWM_FREQ 300 // PWM Hz, must be greater than 60Hz to avoid delayMicroseconds issues
   #define MAX_V 5.00 // the maximum voltage we can output

   long cycle_length;
   float v_out;
   float duty_cycle;
   int on_time;
   int off_time;

   void setup()
   {
     // start up serial for debugging
     Serial.begin(9600);

     // set pin states
     pinMode(ADJ_PIN, INPUT);
     pinMode(PWM_PIN, OUTPUT);
     digitalWrite(PWM_PIN, LOW);

     // calculate the cycle length
     cycle_length = 1000000/PWM_FREQ; // the length of a single cycle of the PWM signal
   }

   void loop()
   {
     // read in the potentiometer value
     int val = analogRead(ADJ_PIN);

     // map the pot value to the PWM value - 0-5V, to two decimal places
     v_out = map(val, 0,1024, 0, 500);
     duty_cycle = (v_out/100) / MAX_V; // work out what percentage of the PWM cycle we should set high
     on_time = duty_cycle * cycle_length;
     off_time = cycle_length - on_time;

     // now set high, then delay for the duty_cycle percentage * cycle_length
     if(on_time > 0)
     {
       digitalWrite(PWM_PIN, HIGH);
       delayMicroseconds(on_time);
     }

     digitalWrite(PWM_PIN, LOW);
     delayMicroseconds(off_time);
   }
