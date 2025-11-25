void setAM2305(){
    dht.begin();
    debug_println(F("sensor 1 on"));

}

void getHumi(){
    float temp_h;
    temp_h = dht.readHumidity();

    // Check if any reads failed and exit early (to try again).
    if ( isnan(temp_h) ) {
        debug_println(F("Failed to read from DHT sensor!"));    
        return;
    }
    curent_humi = (int)temp_h;
}

void get_Temp(){
    float temp_t;
    temp_t = dht.readTemperature();
    if ( isnan(temp_t) ) {
        debug_println(F("Failed to read from DHT sensor!"));    
        return;
    }
    curent_temp = (int)temp_t;
}