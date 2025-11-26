String x;
uint8_t y;
uint8_t z;

void Nextion_send_Str(String command) {
    // Send the command
    nextionSerial.print(command);
    
    // Send the three terminating bytes
    nextionSerial.write(0xFF);
    nextionSerial.write(0xFF);
    nextionSerial.write(0xFF);
}
void Nextion_send_Int(String dev, int data) {
    nextionSerial.print(dev);
    nextionSerial.print("=");
    nextionSerial.print(data);
    nextionSerial.write(0xFF);
    nextionSerial.write(0xFF);
    nextionSerial.write(0xFF);
}
void Nextion_read(){
    nextion_action = nextionSerial.readString();
    
}
int Nextion_getNumber(String command) {
  // Send the command to the Nextion display
  nextionSerial.print("get ");
  nextionSerial.print(command);
  nextionSerial.write(0xFF);
  nextionSerial.write(0xFF);
  nextionSerial.write(0xFF);
  /*
  // Wait for data to arrive
  while (nextionSerial.available() < 4);

  // Read the incoming number value (Nextion returns numbers as 4 bytes)
  int numberValue = 0;
  numberValue |= nextionSerial.read() << 24;
  numberValue |= nextionSerial.read() << 16;
  numberValue |= nextionSerial.read() << 8;
  numberValue |= nextionSerial.read();
  */
  uint8_t numberValue = 0;
  return numberValue;
}

uint8_t Nextion_readVal(String input, uint8_t position){
    String value = getValue(input, '|', position); // Gets "90"
    // Convert to integers
    uint8_t number = value.toInt();
    return number;
}

void Nextion_setScreen(uint8_t init_temp, uint8_t init_humi){
    Nextion_send_Int("p_main.n1.val", init_temp);
    Nextion_send_Int("p_main.n2.val", init_humi);

    Nextion_send_Str("page p_main");

    //Nextion_send_Int("p_live.action_temp.pco", 35921);
    //Nextion_send_Int("p_live.run_time.pco", 35921);
    //Nextion_send_Int("p_live.tim_sec.val", dispay_sec);
}

void Nextion_displayTemperature(int8_t value){
    Nextion_send_Int("p_main.curent_temp.val", value);
}

void Nextion_displayHumidity(uint8_t value){
    Nextion_send_Int("p_main.curent_hum.val", value);
}


String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}