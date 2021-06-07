#include <SPI.h>
#include <SD.h>

#define FILE_NAME_BASE   "CBLOG"
#define FILE_EXT         ".log"
#define MAX_LOGS         10
#define MAX_FILE_SIZE    1000     /* 1KB */

int logCount = 001;
String currentFileName = "";

String genFileName() {
  char buffer[256];
  sprintf(buffer, "%03d", logCount);
  
  String strLogCount = String(buffer);
  String newFileName = FILE_NAME_BASE + strLogCount + FILE_EXT;
  
  logCount++;
  
  if (logCount > MAX_LOGS) {
    logCount = 1;
  }

  if(SD.exists(newFileName)) {
    SD.remove(newFileName);
  }

  return newFileName;
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
  currentFileName = genFileName();
}

void loop() {
  String dataString = "";

  for (int analogPin = 0; analogPin < 3; analogPin++) {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ",";
    }
  }

  File dataFile = SD.open(currentFileName, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    if (dataFile.size() >= MAX_FILE_SIZE) {
      currentFileName = genFileName();
    }
    dataFile.close();
  }
  else {
    Serial.println("error opening datalog.txt");
  }
}
