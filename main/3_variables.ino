// string readed from Display
String tmp_action = "";
uint8_t tmp_time = 0;
uint8_t tmp_temp = 0;

uint8_t action_time = 0;  // timer in minutes to run (from Display)
uint8_t action_temp = 0;  // temperature settings (from Display)

uint8_t set_humidity = 0;
uint8_t set_temperature = 0;

volatile uint32_t lastIsrAt5 = millis();
volatile uint32_t timer_running = millis();

uint8_t vent_action_status = NONE;
uint8_t heater_action_status = NONE;

uint8_t vent_status = RELAY_OFF;
uint8_t heater_status = RELAY_OFF;

// AM2305
DHT dht(DHTPIN, DHTTYPE);

int8_t curent_temp = 0;
uint8_t curent_humi = 0;


// Nextion
String nextion_action = "";

uint8_t percent = 0;