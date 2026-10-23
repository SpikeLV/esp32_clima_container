bool saveSetpoints() {
    File file = SPIFFS.open(SETPOINTS_PATH, FILE_WRITE);
    if (!file) {
        debug_println("Failed to save setpoints");
        return false;
    }

    file.println(set_temperature);
    file.println(set_humidity);
    file.close();

    debug_print("Saved set_temperature=");
    debug_print(set_temperature);
    debug_print(" set_humidity=");
    debug_println(set_humidity);
    return true;
}

void loadSetpoints() {
    if (!SPIFFS.begin(true)) {
        debug_println("SPIFFS mount failed, using defaults");
        set_temperature = DEFAULT_SET_TEMPERATURE;
        set_humidity = DEFAULT_SET_HUMIDITY;
        return;
    }

    File file = SPIFFS.open(SETPOINTS_PATH, FILE_READ);
    if (!file || file.size() == 0) {
        if (file) {
            file.close();
        }
        debug_println("No saved setpoints, using defaults");
        set_temperature = DEFAULT_SET_TEMPERATURE;
        set_humidity = DEFAULT_SET_HUMIDITY;
        saveSetpoints();
        return;
    }

    int temperature = file.readStringUntil('\n').toInt();
    int humidity = file.readStringUntil('\n').toInt();
    file.close();

    if (temperature < 0 || temperature > 99 || humidity < 0 || humidity > 100) {
        debug_println("Saved setpoints out of range, using defaults");
        set_temperature = DEFAULT_SET_TEMPERATURE;
        set_humidity = DEFAULT_SET_HUMIDITY;
        saveSetpoints();
        return;
    }

    set_temperature = (uint8_t)temperature;
    set_humidity = (uint8_t)humidity;
    debug_print("Loaded set_temperature=");
    debug_print(set_temperature);
    debug_print(" set_humidity=");
    debug_println(set_humidity);
}
