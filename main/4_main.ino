HardwareSerial nextionSerial(1);

void setup() {
    debug.begin(115200);
    debug_println("Storage Container R.20251124-01"); // relay for ventilation and heating logic

    nextionSerial.begin(9600, SERIAL_8N1, NEXTION_RX, NEXTION_TX);
    
    Nextion_send_Str("loading.txt=\"Loading relay\"");
    setUpRelay();
    delay(500);

    Nextion_send_Str("loading.txt=\"Loading sensor\"");
    
    setAM2305();
    delay(500);

    get_Humi();
    get_Temp();

    
    Nextion_displayHumidity(curent_humi);
    Nextion_displayTemperature(curent_temp);

    delay(500);
    Nextion_setScreen();
}

void loop() {
    Nextion_read();

    //  -------- Reading sensor each X seconds
    if ((millis() - lastIsrAt5) > 2000) {
        lastIsrAt5 = millis();
        getHumi();
        get_Temp();
        Nextion_displayHumidity(curent_humi);
        Nextion_displayTemperature(curent_temp);
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