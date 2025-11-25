void setUpRelay(){
    pinMode(VENT_PIN, OUTPUT);
    digitalWrite(VENT_PIN, RELAY_OFF);

    pinMode(HEATER_PIN, OUTPUT);
    digitalWrite(HEATER_PIN, RELAY_OFF);

}

void setRelayStatus(int pin, int action){
    digitalWrite(pin, action);   
}