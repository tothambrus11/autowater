void routeGet() {
  server.on("/get", []() {
    if (!requestAuthentication()) {
      return;
    }
    server.send(200, "application/json",
        "{\"watering_duration\": " + String(settings.watering_duration) + "\", \n" +
        "\"wifiSSID\": \"" + realString(String(settings.wifiSSID)) + "\", \n"+
        "\"wifiPWD\": \"" + realString(String(settings.wifiPWD)) + "\"" +
        "}");
  });
}