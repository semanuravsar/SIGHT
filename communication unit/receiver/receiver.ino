// Define the pin for the digital input
const int inputPin = 2; // Change this to the appropriate pin number

// Define the interval duration in milliseconds
const unsigned long interval = 5000; // Change this to the desired interval duration in milliseconds (5 seconds in this example)

// Variables to store the state and counter
int previousState = HIGH; // Assuming idle state is HIGH
int currentState;
unsigned long startTime;
int counter = 0;

void setup() {
  // Set up the digital input pin
  pinMode(inputPin, INPUT);

  // Initialize serial communication for debugging (optional)
  Serial.begin(9600);
}

void loop() {
  // Read the current state of the digital input
  currentState = digitalRead(inputPin);

  // Check if the state has changed from HIGH to LOW (transition begins)
  if (currentState == LOW && previousState == HIGH) {
    // Record the start time when the transition begins
    startTime = millis();
    
    // Increment the counter
    counter++;
    
    // Print debug information (optional)
    Serial.println("Transition begins! Counter: " + String(counter));
  }

  // Update the previous state for the next iteration
  previousState = currentState;

  // Check if the specified interval has passed
  if (millis() - startTime >= interval) {
    // Reset the counter and start time for the next interval
    counter = 0;
    startTime = millis();
  }

  // Your other code can go here

  // Add a delay to avoid rapid readings and conserve resources
  delay(10);
}
