void routeSyncTime() {
  server.on("/syncTime", []() {
    if (!requestAuthentication()) {
      return;
    }
    syncTime();
    server.send(200, "text/html", String(rtc.now().hour()) + ":" + String(rtc.now().minute()));
  });
}