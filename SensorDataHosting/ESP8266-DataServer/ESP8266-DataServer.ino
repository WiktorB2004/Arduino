void setup() {
// Open serial communications and wait for port to open:
Serial.begin(115200);
while (!Serial) {
; // wait for serial port to connect. Needed for native USB port only
}
}

String data;
void loop() { // run over and over
if (Serial.available()) {
  data = Serial.read();
}
  Serial.println(data);
}