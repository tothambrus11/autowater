#include <Chrono.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <NTPClient.h>
#include <Time.h>
#include <TimeLib.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <Wire.h>

#include "EEPROMAnything.h"
#include "RTClib.h"
#include "model.h";

ESP8266WebServer server(12345);

Chrono eachSecondChrono;

const uint8_t water_pump_pin = D5;
Chrono wateringChrono;

RTC_DS3231 rtc;

WiFiUDP ntpUDP;
//                                            UTC+2    update interval (only if the board doesn't restart for one week)
NTPClient timeClient(ntpUDP, "pool.ntp.org", 2 * 3600, 3600 * 24 * 7);

void setup() {
  // Set up serial
  Serial.begin(115200);
  Serial.println();

  // Set up pins
  pinMode(D5, OUTPUT);

  wateringChrono.stop();

  // Init EEPROM
  EEPROM.begin(4096);

  readSettings();
  if (settings.corruptionCheck != CORRUPTION_CHECK_VALUE) {
    clearSettings();
  }

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1) { // Hehe restarts the board
    }
  }
  rtc.disable32K();

  WiFi.hostname("autowater");
  WiFi.mode(WIFI_STA);
  char* wifiSSID;
  char* wifiPWD;
  realString(String(settings.wifiSSID)).toCharArray(wifiSSID, realString(String(settings.wifiSSID)).length());
  realString(String(settings.wifiPWD)).toCharArray(wifiPWD, realString(String(settings.wifiPWD)).length());
  Serial.print("Connecting to: '" + String(wifiSSID));

  WiFi.begin(wifiSSID, wifiPWD);

  WiFi.softAP("AutoWater", "myBestWifiPwd");

  Chrono connectingChrono;
  connectingChrono.start();

  bool failed = false;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    if (connectingChrono.hasPassed(20000)) { // No WIFI
      failed = true;
    }
  }
  Serial.println();

  if (!failed) {
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());

    timeClient.begin();
    if (rtc.lostPower()) {
      Serial.println("RTC lost power, lets set the time!");
      // following line sets the RTC to the date & time this sketch was compiled
      syncTime();
    }
  } else {
    Serial.println("Failed to connect. Please update the wifi credentials!");
    if (rtc.lostPower()) {
      Serial.println("Failed to sync time on startup.");
      settings.last_sync_time = 0;
    }
  }

  if (MDNS.begin("autowater")) {
    Serial.println("MDNS responder started");
  }

  setTime(rtc.now().unixtime());

  displaySettings();

  initRoutes();
  server.begin();

  Serial.println("HTTP server started.");
}

Chrono rtcChrono;

void loop() {
  server.handleClient();
  // Serial.printf("Stations connected to soft-AP = %d\n",
  // WiFi.softAPgetStationNum());
  MDNS.update();

  if (eachSecondChrono.hasPassed(1000)) {
    Serial.println("Time: " + (String)year() + "-" + (String)month() + "-" + String(day()) + " " + String(hour()) + ":" + String(minute()) + " Temperature: " + String(rtc.getTemperature()));

    if (settings.watering_days[_dayOfWeek() - 1]) {
      if (year(settings.last_watering_time) != year() ||
          month(settings.last_watering_time) != month() ||
          day(settings.last_watering_time) != day()) {
        Serial.println("watering day");

        if (settings.watering_time_hours <= hour() && settings.watering_time_minutes <= minute()) {
          settings.last_watering_time = now();
          saveSettings();
          startWatering();
          Serial.println(String(hour()) + ":" + String(minute()) + " vs " + String(settings.watering_time_hours) + ":" + String(settings.watering_time_minutes));
          Serial.println("start pump");
        }
      }
    }
    if (now() >= settings.last_sync_time + 86400) { // Check whether one day has passed
      syncTime();
    }
    if (rtcChrono.hasPassed(60000)) { // Sync time every minute with the RTC module
      setTime(rtc.now().unixtime());
      rtcChrono.restart();
    }
    eachSecondChrono.restart();
  }

  if (wateringChrono.isRunning() && wateringChrono.hasPassed(settings.watering_duration)) {
    wateringChrono.stop();
    digitalWrite(water_pump_pin, false);
    Serial.println("stop pump");
  }
}

void startWatering() {
  wateringChrono.restart();
  digitalWrite(water_pump_pin, true);
}

unsigned short int _dayOfWeek() {
  _dayOfWeek(now());
}

// Monday is the first day of the week here in Hungary ;)
unsigned short int _dayOfWeek(time_t t) {
  if ((weekday(t) + 6) % 7 == 0) {
    return 7;
  }
  return (weekday(t) + 6) % 7;
}

// Sync time with an online clock
void syncTime() {
  if (timeClient.update()) {
    rtc.adjust(timeClient.getEpochTime());
    setTime(rtc.now().unixtime());
    Serial.println("Successfully synced time.");
    settings.last_sync_time = now();
    saveSettings();
  } else {
    Serial.println("Failed to sync time.");
  }
}


bool requestAuthentication() {
  if (!server.authenticate("ambrus", "12345678")) {
    server.requestAuthentication();
    return false;
  }
  return true;
}
