WebServer server(80);

void setupWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    debug_print("Connecting to WiFi ");
    debug_print(WIFI_SSID);

    uint32_t start = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - start) < WIFI_CONNECT_TIMEOUT) {
        delay(250);
        debug_print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
        debug_println("");
        debug_print("WiFi connected, IP: ");
        debug_println(WiFi.localIP());
    } else {
        debug_println("");
        debug_println("WiFi connect failed, continuing without web server");
    }
}

void handleRoot() {
    String log = readResetLog();
    if (log.length() == 0) {
        log = "(log is empty)";
    }

    String html = "<!DOCTYPE html><html><head><meta charset='utf-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>Reset log</title></head><body>";
    html += "<h2>Reset log</h2>";
    html += "<form method='POST' action='/clear' style='margin-bottom:12px'>";
    html += "<button type='submit'>Clear</button></form>";
    html += "<pre style='background:#f0f0f0;padding:10px;white-space:pre-wrap'>";
    html += log;
    html += "</pre></body></html>";

    server.send(200, "text/html", html);
}

void handleClear() {
    clearResetLog();
    server.sendHeader("Location", "/");
    server.send(303);  // redirect back to the main page
}

void setupWebServer() {
    if (WiFi.status() != WL_CONNECTED) {
        return;  // no network, skip starting the server
    }
    server.on("/", HTTP_GET, handleRoot);
    server.on("/clear", HTTP_POST, handleClear);
    server.begin();
    debug_println("Web server started");
}
