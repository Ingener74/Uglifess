void setup() {
  Serial.begin(115200);  
  pinMode(PA0, INPUT);
}

void loop() {
/*
	long now = millis();
	Serial.write(reinterpret_cast<uint8_t*>(&now), sizeof(now));
	int volt = analogRead(PA0);
	Serial.write(reinterpret_cast<uint8_t*>(&volt), sizeof(volt));
*/

	String output;
	output += millis();
	output += " ";
	output += analogRead(PA0);
	Serial.println(output);
}

