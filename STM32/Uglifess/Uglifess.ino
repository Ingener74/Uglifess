void setup() {
  Serial.begin(115200);  
  pinMode(PA0, INPUT);
}

unsigned long delayMs = 1000;

void loop() {
	if (Serial.available() > 0) {
		String command = Serial.readStringUntil(' ');
		if (command == "delay") {
			delayMs = Serial.parseInt();
		}
	}

	String output;
	output += "T";
	output += millis();
	output += " U";
	output += analogRead(PA0);
	Serial.println(output);
  Serial.flush();
	
	delay(delayMs); // TODO replace with async delay
}

