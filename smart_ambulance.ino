// Pins
const int micPin = A0;       // Microphone analog pin
const int AMB_GREEN = 10;    // Ambulance lane LED (Green)
const int CR_RED = 11;       // Cross lane LED (Red)

// Parameters
int threshold = 540;            // Adjust after calibration (0–1023)
unsigned long holdMillis = 8000; // How long ambulance green stays ON
unsigned long lastTrigger = 0;
bool preemptActive = false;

void setup() {
  Serial.begin(9600);

  pinMode(AMB_GREEN, OUTPUT);
  pinMode(CR_RED, OUTPUT);

  // Initial state
  digitalWrite(CR_RED, HIGH);     // Cross lane red ON
  digitalWrite(AMB_GREEN, LOW);   // Ambulance lane green OFF
}

void loop() {
  int micVal = analogRead(micPin);
  Serial.println(micVal);

  if (micVal > threshold && !preemptActive) {
    // Trigger ambulance priority
    preemptActive = true;
    lastTrigger = millis();

    // Cross red OFF, Ambulance green ON
    digitalWrite(CR_RED, LOW);
    digitalWrite(AMB_GREEN, HIGH);

    Serial.println("🚨 Ambulance detected!");
  }

  // If time expired, restore normal
  if (preemptActive && millis() - lastTrigger >= holdMillis) {
    preemptActive = false;

    digitalWrite(CR_RED, HIGH);     // Cross lane red ON
    digitalWrite(AMB_GREEN, LOW);   // Ambulance lane green OFF

    Serial.println("✅ Back to normal");
  }

  delay(30); // Small delay to smooth readings
}