/*
 * A simple logic to control camera shutter using PIR sensor.
 * For certain Canon SLR models, the remote shutter has 3 contacts: (a) ground; (b) focus; and (c) shutter
 * Shorting (b) and/or (c) to the ground will trigger the corresponding function(s).
 * Here, a relay is connected between Arduino and the camera remote port.
 * Signals from Arduino will trigger the relay to connect the shutter/focus to ground.
 * 
 * Pin 2 - PIR sensor output
 * Pin 4 - IN of a relay switch. Triggering the relay will short the camera pins
 */

// Declare Constants
const int sensorPin = 2;           // PIR Sensor is attached to digital pin 2
const int shutterPin = 4;          // Set to high to fire camera shutter
const int ledPin = 13;             // Built-in LED

const int ledBlinkTime = 500;      // Blink one for half a second while calibrating
const int waitTime = 5000;         // Wait for PIR to reset after triggering
const int relayTriggertime = 1000; // How long should the relay be triggered? Set longer if using auto-focus

// Wait for the seonsor to calibrate (20 - 60 seconds according to datasheet)
const unsigned int calibrationTime = 30000;

void setup() {
  Serial.begin(115200);
  
  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(shutterPin, OUTPUT);

  digitalWrite(shutterPin, LOW);
  
  // We need to wait one minute for the sensor to calibrate
  // Get out of view of the sensor for this duration!
  
  // Blink the LED while calibrating
  for (unsigned int i=0; i<calibrationTime; i+=ledBlinkTime*2) {
    digitalWrite(ledPin, HIGH);
    delay(ledBlinkTime);
    digitalWrite(ledPin, LOW);
    delay(ledBlinkTime);
  }
}

void fireShutter() {
  digitalWrite(shutterPin, HIGH);
  delay(relayTriggertime);
  digitalWrite(shutterPin, LOW);
}

void loop() {
  // Constantly check PIR input
  // If it is HIGH the sensor is detecting motion
  if (digitalRead(sensorPin) == HIGH) {

    // Turn the LED on
    digitalWrite(ledPin, HIGH);

    // Fire shutter
    fireShutter();

    // Tell the host computer we detected motion
    Serial.print(1);

    // sleep for the PIR to reset
    delay(waitTime);
    
  } else {
    // Turn the LED off
    digitalWrite(ledPin, LOW);
  }

}
