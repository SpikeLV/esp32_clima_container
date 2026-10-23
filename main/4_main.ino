HardwareSerial nextionSerial(1);

void logResetReason() {
    esp_reset_reason_t reason = esp_reset_reason();
    String text;
    switch (reason) {
        case ESP_RST_POWERON:   text = "POWERON";   break; // normal power-on
        case ESP_RST_EXT:       text = "EXT";        break; // external pin reset
        case ESP_RST_SW:        text = "SW";         break; // esp_restart()
        case ESP_RST_PANIC:     text = "PANIC";      break; // exception/panic (crash)
        case ESP_RST_INT_WDT:   text = "INT_WDT";    break; // interrupt watchdog
        case ESP_RST_TASK_WDT:  text = "TASK_WDT";   break; // task watchdog
        case ESP_RST_WDT:       text = "WDT";        break; // other watchdog
        case ESP_RST_DEEPSLEEP: text = "DEEPSLEEP";  break;
        case ESP_RST_BROWNOUT:  text = "BROWNOUT";   break; // voltage sag (relays!)
        case ESP_RST_SDIO:      text = "SDIO";       break;
        default:                text = "UNKNOWN";    break;
    }

    String line = "Reset reason: " + String((int)reason) + " (" + text + ")";
    appendResetLog(line);
}

void setup() {
    debug.begin(115200);
    debug_println("Storage Container R.20251124-01"); // relay for ventilation and heating logic
    logResetReason();

    pinMode(POWER_LED_PIN, OUTPUT);
    digitalWrite(POWER_LED_PIN, HIGH);

    nextionSerial.begin(9600, SERIAL_8N1, NEXTION_RX, NEXTION_TX);

    Nextion_send_Str("loading.txt=\"Loading settings\"");
    loadSetpoints();

    Nextion_send_Str("loading.txt=\"Loading WiFi\"");
    setupWiFi();
    setupWebServer();
    
    Nextion_send_Str("loading.txt=\"Loading relay\"");
    setUpRelay();
    delay(500);

    Nextion_send_Str("loading.txt=\"Loading sensor\"");
    
    setAM2305();
    delay(500);

    getHumi();
    getTemp();

    
    Nextion_displayHumidity(curent_humi);
    Nextion_displayTemperature(curent_temp);


    delay(500);
    Nextion_setScreen(set_temperature, set_humidity);
}

void loop() {
    server.handleClient();
    Nextion_read();

    //  -------- Reading sensor each X seconds
    if ((millis() - lastIsrAt5) > 2000) {
        lastIsrAt5 = millis();
        getHumi();
        getTemp();
        Nextion_displayHumidity(curent_humi);
        Nextion_displayTemperature(curent_temp);
        debug.println(curent_temp);
    }
    if(lastIsrAt5>millis()){
        lastIsrAt5 = 0;
    }
    
    ///  ----------- info form DISPLAY

    if(nextion_action.indexOf("VENT_M_ON") != -1){
        setRelayStatus(VENT_PIN, RELAY_ON);
        debug.println("VENT_M_ON");
        vent_action_status = NONE;
    }
    if(nextion_action.indexOf("VENT_M_OFF") != -1){
        setRelayStatus(VENT_PIN, RELAY_OFF);
        debug.println("VENT_M_OFF");
        vent_action_status = NONE;
    }

    if(nextion_action.indexOf("HEATER_M_ON") != -1){
        setRelayStatus(HEATER_PIN, RELAY_ON);
        debug.println("HEATER_M_ON");
        heater_action_status = NONE;
    }
    if(nextion_action.indexOf("HEATER_M_OFF") != -1){
        setRelayStatus(HEATER_PIN, RELAY_OFF);
        debug.println("HEATER_M_OFF");
        heater_action_status = NONE;
    }

    ///// CLIMAT VENT
    if(nextion_action.indexOf("VENT_A_ON") != -1){
        set_humidity = Nextion_readVal(nextion_action, 1);
        saveSetpoints();
        debug.println(set_humidity);
        vent_action_status = RUNNING;
        debug.println("VENT_A_ON");
    }

    if(nextion_action.indexOf("VENT_A_OFF") != -1){
        setRelayStatus(VENT_PIN, RELAY_OFF);
        vent_status = RELAY_OFF;
        vent_action_status = NONE;
        debug.println("VENT_A_OFF");
    }

    ///// CLIMAT HEATER
    if(nextion_action.indexOf("HEATER_A_ON") != -1){
        set_temperature = Nextion_readVal(nextion_action, 1);
        saveSetpoints();
        debug.println(set_temperature);
        heater_action_status = RUNNING;
        debug.println("HEATER_A_ON");
    }

    if(nextion_action.indexOf("HEATER_A_OFF") != -1){
        setRelayStatus(HEATER_PIN, RELAY_OFF);
        heater_status = RELAY_OFF;
        heater_action_status = NONE;
        debug.println("HEATER_A_OFF");
    }

    ///   ----    ACTIONS VENT CLIMATE
    if( vent_action_status == RUNNING ){
        if( (curent_humi>=set_humidity) && (vent_status==RELAY_OFF) ){
            setRelayStatus(VENT_PIN, RELAY_ON);
            vent_status = RELAY_ON;
            debug.println("VENT Climate RELAY_ON");
        }
        if( (curent_humi<=set_humidity-TRASHOLE_MIN) && (vent_status==RELAY_ON)){
            setRelayStatus(VENT_PIN, RELAY_OFF);
            vent_status = RELAY_OFF;
            debug.println("VENT Climate RELAY_OFF");
        }
    }
    ///   ----    ACTIONS HEATER CLIMATE
    if( heater_action_status == RUNNING ){
        if( (curent_temp<=set_temperature) && (heater_status==RELAY_OFF) ){
            setRelayStatus(HEATER_PIN, RELAY_ON);
            heater_status = RELAY_ON;
            debug.println("HEATER Climate RELAY_ON");
        }
        if( (curent_temp>=set_temperature+TRASHOLE_MIN) && (heater_status==RELAY_ON)){
            setRelayStatus(HEATER_PIN, RELAY_OFF);
            heater_status = RELAY_OFF;
            debug.println("HEATER Climate RELAY_OFF");
        }
    }
}