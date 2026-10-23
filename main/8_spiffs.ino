void appendResetLog(const String &msg) {
    // SPIFFS may not be mounted yet (this runs early in setup)
    if (!SPIFFS.begin(true)) {
        return;
    }

    // Keep the log from growing without bound: wipe if it gets too big
    File check = SPIFFS.open(RESET_LOG_PATH, FILE_READ);
    if (check) {
        size_t size = check.size();
        check.close();
        if (size > RESET_LOG_MAX_SIZE) {
            SPIFFS.remove(RESET_LOG_PATH);
        }
    }

    File file = SPIFFS.open(RESET_LOG_PATH, FILE_APPEND);
    if (!file) {
        return;
    }
    file.println(msg);
    file.close();
}

String readResetLog() {
    if (!SPIFFS.begin(true)) {
        return "SPIFFS mount failed";
    }
    File file = SPIFFS.open(RESET_LOG_PATH, FILE_READ);
    if (!file) {
        return "";
    }
    String content = file.readString();
    file.close();
    return content;
}

void clearResetLog() {
    if (!SPIFFS.begin(true)) {
        return;
    }
    SPIFFS.remove(RESET_LOG_PATH);
}

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
