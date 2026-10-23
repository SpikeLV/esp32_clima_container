#include "HardwareSerial.h"
#include "SPIFFS.h"
#include "esp_system.h"  // esp_reset_reason()

#include <WiFi.h>        // WiFi station
#include <WebServer.h>   // simple HTTP server

#include "DHT.h"  // temperature and Humidity AM2305n