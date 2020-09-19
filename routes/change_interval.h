void routeChangeInterval() {
  server.on("/change-interval", []() {
    if (!requestAuthentication()) {
      return;
    }
    bool shouldRestart = false;

    int paramCount = server.args();

    for (unsigned short int i = 0; i < sizeof(settings.watering_days); i++) {
      settings.watering_days[i] = false;
    }

    for (unsigned short int i = 0; i < paramCount; i++) {
      if (server.argName(i) == "wtime") {
        float dur = server.arg(i).toFloat();
        if (dur >= 0) {
          settings.watering_duration = (int)(dur * 1000);
        }
      } else if (server.argName(i) == "day") {
        unsigned short int day = server.arg(i).toInt();
        settings.watering_days[day] = 1;
      } else if (server.argName(i) == "wh") {
        unsigned short int wh = server.arg(i).toInt();
        settings.watering_time_hours = wh;
      } else if (server.argName(i) == "wm") {
        unsigned short int wm = server.arg(i).toInt();
        settings.watering_time_minutes = wm;
      }
    }
    saveSettings();
    server.sendHeader("Location", String("/"));
    server.send(303);
  });
}