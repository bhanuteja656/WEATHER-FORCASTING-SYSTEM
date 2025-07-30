#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "DHT.h"

// WiFi credentials
const char* ssid = "syam";
const char* password = "syam1234";

// Weather API details (Vijayawada)
String API_KEY = "9aa6a18948ee9613d37b7c92a8a3bb30";
String LAT = "16.5062";
String LON = "80.6480";
String UNITS = "metric";

// Sensor setup
#define DHTPIN 4
#define DHTTYPE DHT11
#define RAIN_SENSOR_PIN 34

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

// Forecast data structure
struct ForecastData {
  float temp;
  String description;
  String icon;
};
ForecastData forecast[6];

String currentDescription = "";
float currentTemp = 0;
String weatherHTML = "";

void fetchWeatherData() {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  String url = "http://api.openweathermap.org/data/2.5/forecast?lat=" + LAT + "&lon=" + LON + "&units=" + UNITS + "&appid=" + API_KEY;
  http.begin(url);
  int httpCode = http.GET();

  if (httpCode == 200) {
    String payload = http.getString();
    StaticJsonDocument<16384> doc;
    DeserializationError err = deserializeJson(doc, payload);
    if (!err) {
      JsonArray list = doc["list"];
      currentTemp = list[0]["main"]["temp"].as<float>();
      currentDescription = list[0]["weather"][0]["description"].as<String>();

      for (int i = 0; i < 6; i++) {
        forecast[i].temp = list[i * 8]["main"]["temp"].as<float>();
        forecast[i].description = list[i * 8]["weather"][0]["description"].as<String>();
        forecast[i].icon = list[i * 8]["weather"][0]["icon"].as<String>();
      }
    }
  }
  http.end();
}

String getWeatherIcon(String code) {
  if (code.indexOf("01") >= 0) return "☀️";
  if (code.indexOf("02") >= 0) return "🌤️";
  if (code.indexOf("03") >= 0) return "☁️";
  if (code.indexOf("04") >= 0) return "🌥️";
  if (code.indexOf("09") >= 0) return "🌧️";
  if (code.indexOf("10") >= 0) return "🌦️";
  if (code.indexOf("11") >= 0) return "⛈️";
  if (code.indexOf("13") >= 0) return "❄️";
  if (code.indexOf("50") >= 0) return "🌫️";
  return "❓";
}

String getDayOfWeek(int dayOffset) {
  time_t now = time(nullptr) + dayOffset * 86400;
  struct tm* timeInfo = localtime(&now);
  char buffer[10];
  strftime(buffer, sizeof(buffer), "%a", timeInfo);
  return String(buffer);
}

void generateWebPage() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  if (isnan(temperature) || isnan(humidity)) {
    temperature = 0;
    humidity = 0;
  }

  int rainValue = analogRead(RAIN_SENSOR_PIN);
  String rainStatus = rainValue < 1000 ? "🌧️ Rain Detected" : "☀️ No Rain";

  weatherHTML = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  weatherHTML += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  weatherHTML += "<title>WEATHER FORECASTING SYSTEM - Vijayawada</title><style>";
  weatherHTML += "body{margin:0;background:#111;color:#eee;font-family:sans-serif;text-align:center}";
  weatherHTML += ".container{padding:20px}";
  weatherHTML += ".card{background:#222;padding:20px;margin:10px;border-radius:15px;box-shadow:0 4px 12px rgba(0,0,0,0.4)}";
  weatherHTML += ".forecast-row{display:flex;flex-wrap:wrap;justify-content:center}";
  weatherHTML += ".forecast-row .card{flex:1 0 150px;margin:10px}";
  weatherHTML += ".scroll-banner{position:fixed;bottom:0;width:100%;background:#000;color:#0ff;padding:10px;font-size:16px;animation:scroll-left 20s linear infinite}";
  weatherHTML += "@keyframes scroll-left{0%{transform:translateX(100%)}100%{transform:translateX(-100%)}}";
  weatherHTML += "</style></head><body><div class='container'>";

  weatherHTML += "<h1>🌤️ WEATHER FORECASTING SYSTEM</h1>";
  weatherHTML += "<h2>City: Vijayawada</h2>";

  weatherHTML += "<div class='card'><h3>Current Conditions</h3>";
  weatherHTML += "<p>🌡️ Temp: <b>" + String(currentTemp) + " °C</b></p>";
  weatherHTML += "<p>💧 Humidity: <b>" + String(humidity) + " %</b></p>";
  weatherHTML += "<p>📝 Description: <b>" + currentDescription + "</b></p></div>";

  weatherHTML += "<div class='card'><h3>Rain Status</h3><p>" + rainStatus + "</p></div>";

  weatherHTML += "<div class='forecast-row'>";
  for (int i = 0; i < 5; i++) {
    weatherHTML += "<div class='card'>";
    weatherHTML += "<h4>" + getDayOfWeek(i) + "</h4>";
    weatherHTML += "<p style='font-size:30px'>" + getWeatherIcon(forecast[i].icon) + "</p>";
    weatherHTML += "<p>" + forecast[i].description + "</p>";
    weatherHTML += "<p>🌡️ " + String(forecast[i].temp) + " °C</p></div>";
  }
  weatherHTML += "</div></div>";

  weatherHTML += "<div class='scroll-banner'>";
  weatherHTML += "PROJECT BY: Bhanuteja Kolluri - 22HP1A0492 | Chinna Jagapathi Swamy - 22HP1A0494 | Syam Nimmadi - 23HP5A0416 | Anji Thammu - 23HP5A0412";
  weatherHTML += "</div></body></html>";
}

void handleRoot() {
  generateWebPage();
  server.send(200, "text/html", weatherHTML);
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(RAIN_SENSOR_PIN, INPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  configTime(19800, 0, "pool.ntp.org");

  fetchWeatherData();
  server.on("/", handleRoot);
  server.begin();
}

unsigned long lastFetch = 0;
void loop() {
  server.handleClient();
  if (millis() - lastFetch > 3600000) {
    fetchWeatherData();
    lastFetch = millis();
  }
}
