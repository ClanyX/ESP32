#define led 38

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  rgbLedWrite(led, 255, 255, 255); // White
  delay(1000);
  rgbLedWrite(led, 255, 0, 0);  // Red
  delay(1000);
  rgbLedWrite(led, 0, 255, 0);  // Green
  delay(1000);
  rgbLedWrite(led, 0, 0, 255);  // Blue
  delay(1000);
  rgbLedWrite(led, 0, 0, 0);  // Off / black
  delay(1000);
}
