#include <SPI.h>
#include <SD.h>

#define FILE_NAME_BASE   "CBLOG"
#define MAX_LOGS         999

int logCount = 1;

String genFileName() {
  char buffer[256];
  sprintf(buffer, "%03d", logCount);
  
  String strLogCount = String(buffer);
  
  logCount++;
  
  return FILE_NAME_BASE + strLogCount;
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");
  
  if (!SD.begin()) {
    Serial.println("Card failed, or not present");
    while (1);
  }
  Serial.println("card initialized.");
}

void loop() {
  delay(1000);
 
  String dataString = "";

  for (int analogPin = 0; analogPin < 3; analogPin++) {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ",";
    }
  }

  File dataFile = SD.open(genFileName(), FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);
  }
  else {
    Serial.println("error opening datalog.txt");
  }
}
