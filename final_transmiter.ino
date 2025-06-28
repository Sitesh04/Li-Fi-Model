const int led0 = 3; // LED for '0'
const int led1 = 4; // LED for '1'

String inputBits = "";
bool transmitting = false;
int currentIndex = 0;

void setup() {
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  Serial.begin(9600);
  Serial.println("Enter text to transmit:");
}

void loop() {
  // Read input string from Serial Monitor
  if (Serial.available() > 0) {
    String rawInput = Serial.readStringUntil('\n');
    rawInput.trim(); // Clean up spaces/newlines

    inputBits = convertToBinary(rawInput);  // Convert ASCII to binary
    currentIndex = 0;
    transmitting = true;
    Serial.println("Transmitting (RZ): " + inputBits);
    
    // Send start signal
    digitalWrite(led0, HIGH);
    digitalWrite(led1, HIGH);
    delay(2000);  // Start signal: both LEDs ON
    digitalWrite(led0, LOW);
    digitalWrite(led1, LOW);
    delay(1000);  // Delay before data
  }

  // Transmit data
  if (transmitting && currentIndex < inputBits.length()) {
    char bit = inputBits.charAt(currentIndex);

    if (bit == '0') {
      digitalWrite(led0, HIGH);
    } else if (bit == '1') {
      digitalWrite(led1, HIGH);
    }

    delay(500); // LED ON (bit pulse)

    digitalWrite(led0, LOW);
    digitalWrite(led1, LOW);
    delay(500); // OFF

    currentIndex++;
  }

  if (currentIndex >= inputBits.length() && transmitting) {
    transmitting = false;
    Serial.println("Transmission complete.\nEnter next bit sequence:");
  }
}

// --- ASCII to Binary Converter ---
String convertToBinary(String text) {
  String result = "";
  for (int i = 0; i < text.length(); i++) {
    char c = text.charAt(i);
    byte asciiVal = byte(c);
    for (int b = 7; b >= 0; b--) {
      result += ((asciiVal >> b) & 1) ? "1" : "0";
    }
  }
  return result;
}
