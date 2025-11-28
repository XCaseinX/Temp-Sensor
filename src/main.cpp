#include <Arduino.h>
#include <DHT.h>               // Include the DHT sensor library
#include <Adafruit_SSD1306.h>  //OLED Library
#include <Wire.h>              //I2C Connection

#define DHTPIN 5       // Digital pin connected to the DHT sensor //original pin D2
#define DHTTYPE DHT11  // Define sensor type (DHT11 or DHT22)

const int screenWidth = 128;
const int screenHeight = 64;
float humidity;
float temperature;

DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT sensor
Adafruit_SSD1306 display(screenWidth, screenHeight, &Wire, -1);


void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("DHT11 Temperature and Humidity Sensor Test");

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  //Test if oled connects
    Serial.println("SSD1306 allocation failed");
    for (;;)
      ;
  }
  delay(2000);

  //Prepare OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  display.display();
}

void loop() {
humidity = dht.readHumidity();
  temperature = (dht.readTemperature() * 1.8) + 32;

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000); // Wait before trying again
    return;
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Humidity: ");
  display.print(humidity);
  display.println("%");

  display.setCursor(0, 20);
  display.print("Temp: ");
  display.print(temperature);
  display.print(" ");
  display.print((char)247);
  display.print("F");

  display.display();

  Serial.print("Humidity: "); Serial.print(humidity); Serial.print("%\t");
  Serial.print("Temperature: "); Serial.println(temperature);

  delay(2000);
}