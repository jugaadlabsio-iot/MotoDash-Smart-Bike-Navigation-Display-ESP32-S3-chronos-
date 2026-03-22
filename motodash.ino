#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <ChronosESP32.h>

// ===== DISPLAY PINS =====
#define TFT_CS    5
#define TFT_RST   8
#define TFT_DC    4
#define TFT_MOSI  7
#define TFT_SCLK  6
#define TFT_BL    15
#define TFT_MISO  16

Adafruit_ST7789 tft = Adafruit_ST7789(&SPI, TFT_CS, TFT_DC, TFT_RST);
ChronosESP32 watch("MotoDash");

// ===== FLAGS =====
volatile bool deviceConnected = false;
volatile bool updateUI = true;
volatile bool newData = false;

// ===== CENTER TEXT =====
void centerText(String text, int y, int size, uint16_t color) {
  tft.setTextSize(size);
  tft.setTextColor(color);

  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

  tft.setCursor((240 - w) / 2, y);
  tft.println(text);
}

// ===== CALLBACKS =====
void connectionCallback(bool state) {
  deviceConnected = state;
  updateUI = true;
}

void dataCallback(uint8_t* data, int len) {
  newData = true;
}

void configCallback(Config config, uint32_t a, uint32_t b) {
  if (config == CF_NAV_DATA) {
    updateUI = true;
  }
}

// ===== SETUP =====
void setup() {
  Serial.begin(115200);

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  SPI.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);

  tft.init(240, 280);
  tft.setRotation(0);
  tft.fillScreen(ST77XX_BLACK);

  centerText("STARTING...", 120, 2, ST77XX_WHITE);

  watch.setConnectionCallback(connectionCallback);
  watch.setDataCallback(dataCallback);
  watch.setConfigurationCallback(configCallback);

  watch.begin();
}

// ===== LOOP =====
void loop() {

  static unsigned long lastRun = 0;

  // ===== SAFE CHRONOS LOOP =====
  if ((newData || millis() - lastRun > 1000) && deviceConnected) {
    newData = false;
    lastRun = millis();
    watch.loop();
  }

  // ===== UI =====
  if (updateUI) {
    updateUI = false;

    tft.fillScreen(ST77XX_BLACK);

    if (deviceConnected) {

      Navigation nav = watch.getNavigation();

      if (nav.active) {

        String dir = nav.directions;
        dir.toLowerCase();

        // ===== 🔥 BALANCED ARROW =====
        if (dir.indexOf("left") >= 0) {
          tft.fillTriangle(70, 80, 160, 30, 160, 130, ST77XX_WHITE);
        } 
        else if (dir.indexOf("right") >= 0) {
          tft.fillTriangle(170, 80, 80, 30, 80, 130, ST77XX_WHITE);
        } 
        else {
          tft.fillRect(110, 30, 20, 70, ST77XX_WHITE);
          tft.fillTriangle(100, 30, 140, 30, 120, 5, ST77XX_WHITE);
        }

        // ===== DISTANCE =====
        String dist = nav.distance;
        if (dist == "0 m") dist = "Now";
        centerText(dist, 130, 3, ST77XX_CYAN);

        // ===== ROAD NAME FIX =====
        String road = nav.title;
        if (road.length() < 3) road = nav.directions;

        String line1 = road.substring(0, min(18, (int)road.length()));
        String line2 = "";
        if (road.length() > 18) {
          line2 = road.substring(18, min(36, (int)road.length()));
        }

        centerText(line1, 170, 2, ST77XX_WHITE);
        if (line2.length() > 0) {
          centerText(line2, 190, 2, ST77XX_WHITE);
        }

        // ===== ETA =====
        centerText("ETA " + nav.duration, 220, 2, ST77XX_YELLOW);

      } else {
        centerText("CONNECTED", 120, 2, ST77XX_GREEN);
      }

    } else {
      centerText("DISCONNECTED", 120, 2, ST77XX_RED);
    }
  }

  delay(20);
}
