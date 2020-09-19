const unsigned int CORRUPTION_CHECK_VALUE = 12345678;
const unsigned short WIFI_SSID_LENGTH = 64;
const unsigned short WIFI_PWD_LENGTH = 64;

struct settings_t {
  unsigned int watering_duration;  // in seconds
  char wifiSSID[WIFI_SSID_LENGTH];
  char wifiPWD[WIFI_PWD_LENGTH];
  unsigned short int speed;  // 0-100
  bool watering_days[7];
  unsigned short int watering_time_hours;
  unsigned short int watering_time_minutes;
  time_t last_watering_time;
  time_t last_sync_time;
  unsigned int corruptionCheck;
};

settings_t settings;

void readSettings() {
  EEPROM_readAnything(0, settings);
}

void saveSettings() {
  EEPROM_writeAnything(0, settings);
  EEPROM.commit();
}

void clearSettings() {
  settings.watering_duration = 2000;

  String ssid = "";
  while (ssid.length() < WIFI_SSID_LENGTH) {
    ssid.concat(' ');
  }
  ssid.toCharArray(settings.wifiSSID, WIFI_SSID_LENGTH);

  String pwd = "";
  while (pwd.length() < WIFI_PWD_LENGTH) {
    pwd.concat(' ');
  }
  pwd.toCharArray(settings.wifiPWD, WIFI_PWD_LENGTH);

  settings.corruptionCheck = CORRUPTION_CHECK_VALUE;

  for (int i = 0; i < 7; i++) {
    settings.watering_days[i] = 0;
  }

  settings.watering_time_hours = 12;
  settings.watering_time_minutes = 0;
  settings.last_watering_time = 0;

  saveSettings();
}

void displaySettings() {
  Serial.println("watering_duration: " + String(settings.watering_duration));
}

String realString(String storedString) {
  while (storedString.length() > 0 && storedString.charAt(storedString.length() - 1) == ' ') {
    storedString.remove(storedString.length() - 1, 1);
  }
  return storedString;
}