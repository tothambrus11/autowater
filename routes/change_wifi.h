void routeChangeWifi() {
  server.on("/change-wifi", []() {
    if (!requestAuthentication()) {
      return;
    }
    int paramCount = server.args();

    for (unsigned short int i = 0; i < paramCount; i++) {
      if (server.argName(i) == "wifiSSID") {
        String ssid = server.arg(i);
        while (ssid.length() < WIFI_SSID_LENGTH) {
          ssid.concat(' ');
        }
        ssid.toCharArray(settings.wifiSSID, WIFI_SSID_LENGTH);
      } else if (server.argName(i) == "wifiPWD") {
        String pwd = server.arg(i);
        while (pwd.length() < WIFI_PWD_LENGTH) {
          pwd.concat(' ');
        }
        pwd.toCharArray(settings.wifiPWD, WIFI_PWD_LENGTH);
      }
    }
    saveSettings();

    server.send(200, "text/html", u8R"(Restarting...<script>setTimeout(()=>{window.location.href = "/";}, 10000);</script>)");

    ESP.restart();  // First restart is bad, then it will be ok
  });
}