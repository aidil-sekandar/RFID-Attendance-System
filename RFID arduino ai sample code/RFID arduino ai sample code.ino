#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

// Pin connections
#define RST_PIN 9
#define SS_PIN 10
#define OLED_SDA A4
#define OLED_SCL A5

// WiFi credentials
const char *ssid = "your_ssid";
const char *password = "your_password";

// Firebase credentials
#define FIREBASE_HOST "your_firebase_url"
#define FIREBASE_AUTH "your_firebase_api_key"

// RFID reader object
MFRC522 mfrc522(SS_PIN, RST_PIN);

// OLED display object
Adafruit_SSD1306 display(OLED_SDA, OLED_SCL);

void setup()
{
  // Initialize serial communication
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  // Initialize OLED display
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Welcome to");
  display.println("Attendance System");
  display.display();

  // Initialize RFID reader
  mfrc522.PCD_Init();
}

void loop()
{
  // Check for RFID card
  if (mfrc522.PICC_IsNewCardPresent())
  {
    // Select RFID card
    MFRC522::PICC_Type piccType = mfrc522.PICC_ReadCardSerial();
    // Get card ID
    String cardID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
      cardID += String(mfrc522.uid.uidByte[i], HEX);
    }
    // Compare card ID with registered IDs
    if (cardID == "your_registered_card_id")
    {
      // Record attendance data
      Firebase.set("/attendance/" + cardID, "Present " + String(millis()));
      // Display message on OLED display
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Welcome");
      display.setCursor(0, 20);
      display.println("Recorded");
      display.display();
      delay(3000);
    }
  }
}
