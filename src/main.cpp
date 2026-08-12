#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <ctype.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <WiFi.h>

#include "web_page.h"

#if __has_include("wifi_config.h")
#include "wifi_config.h"
#else
#define WIFI_SSID "YOUR_2_4_GHZ_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#endif

namespace {
constexpr uint8_t LED_PIN = 27;
constexpr size_t LED_COUNT = 25;
// M5Stackの注意事項に合わせた低い固定輝度。上げすぎると発熱・破損の原因になる。
constexpr uint8_t LED_BRIGHTNESS = 20;
constexpr uint32_t WIFI_TIMEOUT_MS = 15000;

constexpr char HOST_NAME[] = "atom-matrix";
constexpr char AP_SSID[] = "AtomMatrix-LED";
constexpr char AP_PASSWORD[] = "atommatrix";

// Web画面の左上から右下へ並ぶ論理番号と、LEDの物理番号の対応。
constexpr uint8_t PIXEL_MAP[LED_COUNT] = {
    0,  1,  2,  3,  4,
    5,  6,  7,  8,  9,
    10, 11, 12, 13, 14,
    15, 16, 17, 18, 19,
    20, 21, 22, 23, 24,
};

constexpr uint32_t DEFAULT_COLORS[LED_COUNT] = {
    0xFF4D6D, 0x000000, 0x000000, 0x000000, 0x4DABFF,
    0xFF4D6D, 0xFF9F43, 0x000000, 0x7C5CFC, 0x4DABFF,
    0xFF4D6D, 0x000000, 0xFFE66D, 0x000000, 0x4DABFF,
    0xFF4D6D, 0x000000, 0x000000, 0x000000, 0x4DABFF,
    0xFF4D6D, 0x000000, 0x000000, 0x000000, 0x4DABFF,
};

Adafruit_NeoPixel matrix(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
WebServer server(80);
uint32_t currentColors[LED_COUNT];

String colorToHex(uint32_t color) {
  char result[8];
  snprintf(result, sizeof(result), "#%06lX",
           static_cast<unsigned long>(color & 0xFFFFFF));
  return String(result);
}

bool parseColor(const String &value, uint32_t &color) {
  if (value.length() != 7 || value.charAt(0) != '#') {
    return false;
  }

  for (size_t i = 1; i < value.length(); ++i) {
    if (!isxdigit(static_cast<unsigned char>(value.charAt(i)))) {
      return false;
    }
  }

  color = strtoul(value.substring(1).c_str(), nullptr, 16) & 0xFFFFFF;
  return true;
}

bool parseColorList(const String &value, uint32_t parsed[LED_COUNT]) {
  int start = 0;
  for (size_t i = 0; i < LED_COUNT; ++i) {
    const int comma = value.indexOf(',', start);
    if ((i < LED_COUNT - 1 && comma < 0) ||
        (i == LED_COUNT - 1 && comma >= 0)) {
      return false;
    }

    const int end = comma >= 0 ? comma : value.length();
    String token = value.substring(start, end);
    token.trim();
    if (!parseColor(token, parsed[i])) {
      return false;
    }
    start = end + 1;
  }
  return true;
}

void showPixels() {
  for (size_t i = 0; i < LED_COUNT; ++i) {
    matrix.setPixelColor(PIXEL_MAP[i], currentColors[i]);
  }
  matrix.show();
}

void sendPixelsJson() {
  String json;
  json.reserve(320);
  json += F("{\"colors\":[");
  for (size_t i = 0; i < LED_COUNT; ++i) {
    if (i > 0) {
      json += ',';
    }
    json += '"';
    json += colorToHex(currentColors[i]);
    json += '"';
  }
  json += F("],\"brightness\":");
  json += LED_BRIGHTNESS;
  json += F("}");
  server.send(200, "application/json; charset=utf-8", json);
}

void handlePixelsUpdate() {
  if (!server.hasArg("colors")) {
    server.send(400, "application/json; charset=utf-8",
                F("{\"error\":\"colors is required\"}"));
    return;
  }

  uint32_t nextColors[LED_COUNT];
  if (!parseColorList(server.arg("colors"), nextColors)) {
    server.send(400, "application/json; charset=utf-8",
                F("{\"error\":\"25 colors in #RRGGBB format are required\"}"));
    return;
  }

  // Webから受け取った色はRAMにだけ保持する。
  // リセットや電源再投入時にはDEFAULT_COLORSへ戻し、フラッシュへ書き込まない。
  memcpy(currentColors, nextColors, sizeof(currentColors));
  showPixels();
  sendPixelsJson();
}

void startNetwork() {
  const bool hasCredentials = String(WIFI_SSID).length() > 0 &&
                              String(WIFI_SSID) != "YOUR_WIFI_SSID" &&
                              String(WIFI_SSID) != "YOUR_2_4_GHZ_WIFI_SSID";

  if (hasCredentials) {
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(HOST_NAME);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.printf("Connecting to Wi-Fi: %s", WIFI_SSID);

    const uint32_t startedAt = millis();
    while (WiFi.status() != WL_CONNECTED &&
           millis() - startedAt < WIFI_TIMEOUT_MS) {
      delay(500);
      Serial.print('.');
    }
    Serial.println();
  }

  if (WiFi.status() == WL_CONNECTED) {
    if (MDNS.begin(HOST_NAME)) {
      MDNS.addService("http", "tcp", 80);
    }
    Serial.println("Wi-Fi connected.");
    Serial.printf("URL: http://%s.local/\n", HOST_NAME);
    Serial.printf("IP : http://%s/\n", WiFi.localIP().toString().c_str());
    return;
  }

  WiFi.disconnect(true);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASSWORD);
  Serial.println("Access point mode started.");
  Serial.printf("SSID: %s\n", AP_SSID);
  Serial.printf("PASS: %s\n", AP_PASSWORD);
  Serial.printf("URL : http://%s/\n", WiFi.softAPIP().toString().c_str());
}

void startWebServer() {
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Cache-Control", "no-store");
    server.send_P(200, "text/html; charset=utf-8", INDEX_HTML);
  });
  server.on("/api/leds", HTTP_GET, sendPixelsJson);
  server.on("/api/leds", HTTP_POST, handlePixelsUpdate);
  server.onNotFound([]() {
    server.send(404, "application/json; charset=utf-8",
                F("{\"error\":\"not found\"}"));
  });
  server.begin();
  Serial.println("Web server started.");
}
}  // namespace

void setup() {
  Serial.begin(115200);
  delay(300);

  matrix.begin();
  matrix.setBrightness(LED_BRIGHTNESS);
  matrix.clear();

  memcpy(currentColors, DEFAULT_COLORS, sizeof(currentColors));
  showPixels();

  startNetwork();
  startWebServer();
}

void loop() {
  server.handleClient();
  delay(2);
}
