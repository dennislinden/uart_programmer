#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

/*
 * Output the address bits and outputEnable signal using shift registers.
 */
void setAddress(int address, bool outputEnable) {
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}


/*
 * Read a byte from the EEPROM at the specified address.
 */
byte readEEPROM(int address) {
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, INPUT);
  }
  setAddress(address, /*outputEnable*/ true);

  byte data = 0;
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1) {
    data = (data << 1) + digitalRead(pin);
  }
  return data;
}


/*
 * Write a byte to the EEPROM at the specified address.
 */
void writeEEPROM(int address, byte data) {
  setAddress(address, /*outputEnable*/ false);
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, OUTPUT);
  }

  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }
  digitalWrite(WRITE_EN, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_EN, HIGH);
  delay(10);
}


/*
 * Read the contents of the EEPROM and print them to the serial monitor.
 */
void printContents() {
  for (int base = 0; base <= 2000; base += 16) {
    byte data[16];
    for (int offset = 0; offset <= 15; offset += 1) {
      data[offset] = readEEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }
}

int address = 0;
void programEEpromSingleByte(byte data, int sizeData){
  // Program data bytes
 

  Serial.print("Programming EEPROM\n");
  char bufAddress[10];
  sprintf(bufAddress, "Address: %d\n", address);
  Serial.print(bufAddress);
  
  char bufData[20];
  sprintf(bufData, "Data: %d\n", data);
  Serial.print(bufData);
  
  writeEEPROM(address, data);
  address += 1;
  if (address % 64 == 0) {
    Serial.print(".");
  }
  Serial.println("\ndone");
}

void setup() {
  // put your setup code here, to run once:
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  Serial.begin(57600);
}

bool lenDataSet = false;
bool addressStartSet = false;
int addressStart = 0;
int lenData = 0;
bool addressSetUsed = false;

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()) {
    int data_rcvd = Serial.read();   // read one byte from serial buffer and save to data_rcvd
    Serial.println("Reading EEPROM");
    Serial.println(address);
    Serial.println(lenData);

    if (address == 0 && addressStartSet == false){
      addressStart = data_rcvd;
      address += 1;
      addressStartSet = true;
      return;
    }
    if(address == 1 && lenDataSet == false){
      lenData = data_rcvd;
      address += 1;
      lenDataSet = true;
      return;
    }

    if(addressSetUsed == false){
      address = addressStart;
      addressSetUsed = true;
    }
    
    char buf1[20];
    sprintf(buf1, "address: %d\n", address);
    Serial.print(buf1);

    char buf[10];
    sprintf(buf, "%d", data_rcvd);
    Serial.println(buf);
    programEEpromSingleByte(data_rcvd, sizeof(data_rcvd));
    printContents();
    
FIND OUT HOW TO WRITE ALL BYTES SHOULD BE 16kb so 2000bytes (hex 0x7D0)
    char buf3[20];
    sprintf(buf3, "address: %d\n", address);
    Serial.print(buf3);

    char buf4[20];
    sprintf(buf4, "lenData: %d\n", lenData);
    Serial.print(buf4);
    
    // reset address if all data has been send
    if(address >= lenData+addressStart-2){
      Serial.println("Reset all");
      address = 0;
      addressStart = 0;
      lenDataSet = false;
      lenData = 0;
      addressStartSet = false;
      addressSetUsed = false;
    }
  }
}