void setup() {
  Serial.begin(9600);
  delay(3000);
  Serial.print("key\n");
  delay(1000);
  Serial.print("vulndrv\n");
  delay(1000);
  Serial.print("pname\n");
  delay(1000);
  // Stack Buffer Overflow
  Serial.print("aaaaaaaaaaaaaaaaa");
  delay(1000);
  // Stack Buffer Overflow
  Serial.print("ccount\n");
  delay(1000);
  Serial.print("2\n");
  delay(1000);
  Serial.print("done\n");
  delay(1000);

  // Double Free
  Serial.print("reset\n");
  delay(1000);
  Serial.print("reset\n");
  delay(1000);
  // Double Free
  // Use After Free
  Serial.print("pname\n");
  delay(1000);
  // Use After 
  Serial.print("key\n");
  delay(1000);
  Serial.print("vulndrv\n");
  delay(1000);
  Serial.print("joystick\n");
  delay(1000);
  Serial.print("ccount\n");
  delay(1000);
  // Heap Buffer Overflow
  Serial.print("2\n");
  delay(1000);
  Serial.print("done\n");
  delay(1000);
  Serial.print("120, 80, 1\n");
  delay(1000);
  // Heap Buffer Overflow

  Serial.print("reset\n");
  delay(1000);
  Serial.print("key\n");
  delay(1000);
  Serial.print("vulndrv\n");
  delay(1000);
  Serial.print("pname\n");
  delay(1000);
  Serial.print("joystick\n");
  delay(1000);
  Serial.print("ccount\n");
  delay(1000);
  // Int Overflow
  Serial.print("2147483648\n");
  delay(1000);
  // Int Overflow
  Serial.print("done\n");
  delay(1000);
}

void loop() {
  //random(1,4);
}
