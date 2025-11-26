#define debug Serial
#define DEBUG 1

#if DEBUG == 1
#define debug_print(x) debug.print(x)
#define debug_println(x) debug.println(x)
#define NET_SSID "Do-You-24-2"
#define NET_PASSWORD "Moon413157"
#else
#define debug_print(x)
#define debug_println(x)
#define NET_SSID "TomyLimon"
#define NET_PASSWORD "Moon413157" 
#endif

// devices status
#define NONE      0
#define STARTING  1
#define WARMUP    2
#define RUNNING   3
#define COOLDOWN  4
#define STOPPING  5
#define STOPPED   6

#define RELAY_ON 1
#define RELAY_OFF 0

#define TRASHOLE_MIN 3

#define VENT_PIN 16
#define HEATER_PIN 17

// temperature and humidity AM2305
#define DHTPIN 26
#define DHTTYPE DHT22

// Serial2 for Display
#define NEXTION_RX 35  // Connect to Nextion TX
#define NEXTION_TX 32  // Connect to Nextion RX