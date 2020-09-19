void waterThisDay() {
  server.on("/water-this-day", []() {
    if (!requestAuthentication()) {
      return;
    }
    settings.last_watering_time = 0;
    saveSettings();
    server.send(200, "text/html", "OK");
  });
}