/* ------------------------------------------------------------ */
/*                                                              */
/* Sensor library for liquid level detection with Simarine Pico */
/* Written by D.Sánchez 20019 and published under the EUPL v1.2 */
/* Get your copy at:                                            */
/* https://joinup.ec.europa.eu/collection/eupl/eupl-text-11-12  */
/* Version 0.1A                                                 */
/*                                                              */
/* Hardware needed:                                             */
/* As many level sensors of type XKC-Y25-V as needed.           */
/*                                                              */
/* Connection:                                                  */
/* YELLOW (SIGNAL) -> Digital Input Pin                         */
/* BROWN  (VCC)    -> +5V                                       */
/* BLUE   (GND)    -> GND                                       */
/*                                                              */
/* Output:                                                      */
/* Variable Voltage through VCC_PIN (PWM)                       */
/*                                                              */
/* Errors:                                                      */
/* If a pin reads zero and the next pin one, an checksum error  */
/* is thrown. The LED blinks as many times, as the number of    */
/* the faulty pin.                                              */
/*                                                              */
/* ------------------------------------------------------------ */

#define INPUT_PINS 5 //Define how many inputs we have
#define PIN_0      5 //These are the input pins, mapped to a constant...
#define PIN_1      6 //...
#define PIN_2      7 //...
#define PIN_3      8 //...
#define PIN_4      9 //...
#define VCC_PIN    10 //This pin puts our working voltage out.

void setup(){

  Serial.begin(9600); //Start serial monitor

  pinMode(LED_BUILTIN, OUTPUT); //Set built in led to output - kind of straightforward, right?
  pinMode(PIN_0, INPUT); //Define all input pins...
  pinMode(PIN_1, INPUT); //...
  pinMode(PIN_2, INPUT); //...
  pinMode(PIN_3, INPUT); //...
  pinMode(PIN_4, INPUT); //...
  analogWrite( VCC_PIN, 0 );  //Turn output voltage off
  digitalWrite( LED_BUILTIN, LOW ); //Turn built in led off
  
}

void loop(){

  //Declare an array to hold the different relay states  
  int pin[INPUT_PINS] = { 0,0,0,0,0 };

  //Declare an integer to hold the checksum, this is for integrity check of the sensors
  int checksum = 0;

  //Keep record of erronoeous pins
  int errorpin = 0;

  //Read all relay states
  pin[0] = digitalRead(PIN_0); //Pin 3 stands for 20%
  pin[1] = digitalRead(PIN_1); //Pin 4 stands for 40%
  pin[2] = digitalRead(PIN_2); //Pin 5 stands for 60%
  pin[3] = digitalRead(PIN_3); //Pin 6 stands for 80%
  pin[4] = digitalRead(PIN_4); //Pin 7 stands for 100%
  
  //Construct checksum value
  for ( int i = 0; i<5; i++) {
    checksum = checksum + pin[i];  
  }
  
  //Print status messages
  Serial.print("Checksum: ");Serial.print( checksum );
  //Check sensors from high to low (this is important)
  if ( pin[4] == 1 ) { //Is Pin 7 (which is the 5th element of the array) on?
    //YES
    if (checksum == 5) { //Is the checksum correct?
      //YES
      Serial.println(" Capacity: Full"); //Print status message
      analogWrite( VCC_PIN, 255 ); //Set voltage at VCC_PIN
    } else {
      //NO
      Serial.println(" Checksum error reading pin 5"); //Print status message
      analogWrite( VCC_PIN, 255 ); //Set level to full...
      errorpin = getErrorPin( pin );
    }
    //NO
  } else if ( pin[3] == 1 ) { //Is pin 6 on? //The rest is exactly the same as documented above.
    //YES
    if (checksum == 4) {
      Serial.println(" Capacity: 80%");
      analogWrite( VCC_PIN, 220 );
    } else {
      Serial.println(" Checksum error reading pin 4");
      analogWrite( VCC_PIN, 255 );
      errorpin = getErrorPin( pin );
    }
  } else if ( pin[2] == 1 ) {
    if (checksum == 3) {
      Serial.println(" Capacity: 60%");
      analogWrite( VCC_PIN, 165 );
    } else {
      Serial.println(" Checksum error reading pin 3");
      analogWrite( VCC_PIN, 255 );
      errorpin = getErrorPin( pin );
    }
  } else if ( pin[1] == 1 ) {
    if (checksum == 2) {
      Serial.println(" Capacity: 40%");
      analogWrite( VCC_PIN, 110 );
    } else {
      Serial.println(" Checksum error reading pin 2");
      analogWrite( VCC_PIN, 255 );
      errorpin = getErrorPin( pin );
    }
  } else if ( pin[0] == 1 ) {
    Serial.println(" Capacity 20%");
    analogWrite( VCC_PIN, 55 );
  } else {
    Serial.println(" Capacity 0%");
    analogWrite( VCC_PIN, 0 );
  }

  if ( errorpin > 0 ) { //Do we have a faulty pin?
    //YES
    for ( int i = errorpin; i>0; i-- ) { //Blink as many times as the number of the pin
      digitalWrite( LED_BUILTIN, HIGH ); //Turn built in led on
      delay( 150 );
      digitalWrite( LED_BUILTIN, LOW ); //Turn built in led off
      delay(200);
    }
  }
  
  //This delay is needed for the serial string to be output
  delay(1000);

}

int getErrorPin( int pinArray[] ) { //This function searches for a low reading pin in between two high pins.
  for ( int i = 0; i<INPUT_PINS; i++ ) { //Count upwards from 0 - this is important
    if ( pinArray[i]==0 ) { //If an array reads zero (this must be the faulty pin (unless we have two)
      return i+1; //Return the human readable pin (counting from one)
    }
  }
}
