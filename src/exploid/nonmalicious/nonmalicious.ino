void setup() {
  Serial.begin(9600);
  delay(3000);
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
  Serial.print("3\n");
  delay(1000);
  Serial.print("done\n");
  delay(1000);
}

void loop() {
  int x;
  int y;
  int z;

  x = random(-1024, 1024);
  y = random(-1024, 1024);
  z = random(-1024, 1024);

  Serial.print(" ");
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print(", ");
  Serial.println(z);

  delay(1000);
}
