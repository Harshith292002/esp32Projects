#include <Arduino.h>
#include <LiquidCrystal.h>

// Define the LCD pins
LiquidCrystal lcd(16, 17, 18, 19, 20, 21);

void setup() {
    // Set up the LCD
    lcd.begin(16, 2); // Initialize the LCD with 16 columns and 2 rows
    lcd.print("Hello, ESP32!"); // Print a message to the LCD
}

void loop() {
    // Nothing to do in the loop for this example
}
