void routeReset() {
  server.on("/reset", []() {
    if (!requestAuthentication()) {
      return;
    }
    server.send(200, "application/json", "TODO RESET");
  });
}