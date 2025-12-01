#include <Arduino.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// -------------------- Configuration --------------------
#define DHTPIN 13
#define DHTTYPE DHT11

const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;

// -------------------- Global Variables --------------------
float humidity = 0.0;
float temperatureF = 0.0;

// -------------------- Objects --------------------
DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// -------------------- Setup --------------------
void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10); //Program will only begin once serial monitor is opened

  Serial.println("Mini Weather Station Started");

  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not detected!");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.display();
}

// -------------------- Helper: Draw Bar Graph --------------------
void drawBar(int x, int y, int width, int height, float value, float maxVal) {
  display.drawRect(x, y, width, height, WHITE); // Outline
  int fillWidth = (int)((value / maxVal) * width);
  fillWidth = constrain(fillWidth, 0, width);  // Prevent overflow
  display.fillRect(x, y, fillWidth, height, WHITE); // Filled portion
}

// -------------------- Loop --------------------
void loop() {
  temperatureF = (dht.readTemperature() * 1.8) + 32;
  humidity = dht.readHumidity();

  Serial.print("Temp: "); Serial.print(temperatureF, 1); Serial.print(" F\t");
  Serial.print("Humidity: "); Serial.println(humidity, 1);

  if (isnan(temperatureF) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }


  // Print to Serial
  Serial.print("Temp: "); Serial.print(temperatureF, 1); Serial.print(" F\t");
  Serial.print("Humidity: "); Serial.println(humidity, 1);

  // Display on OLED
  display.clearDisplay();

  // ---- Temperature ----
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temperatureF, 1);
  display.write(247); // Degree symbol
  display.print("F");

  // ---- Humidity ----
  display.setCursor(0, 16);//16 is the minimum x position where humidity appears fully blue 
  display.print("Humidity: ");
  display.print(humidity, 1);
  display.println("%");

  // ---- Bar graphs ----
  // Dynamic scaling: temperature 0–120°F, humidity 0–100%
  drawBar(0, 40, 128, 10, temperatureF, 120); // temperature bar
  drawBar(0, 55, 128, 7, humidity, 100);      // humidity bar

  display.display();

  delay(2000);
}
