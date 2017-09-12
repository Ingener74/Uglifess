void setup() {
  Serial.begin(115200);  
  pinMode(PA0, INPUT);
}

void loop() {
	long now = millis();
	Serial.write(reinterpret_cast<uint8_t*>(&now), sizeof(now));
}

