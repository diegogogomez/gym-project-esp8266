/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * Example sketch/program showing how to read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the ID/UID, type and any data blocks it can read. Note: you may see "Timeout in communication" messages
 * when removing the PICC from reading distance too early.
 * 
 * If your reader supports it, this sketch/program will read all the PICCs presented (that is: multiple tag reading).
 * So if you stack two or more PICCs on top of each other and present them to the reader, it will first output all
 * details of the first and then the next PICC. Note that this may take some time as all data blocks are dumped, so
 * keep the PICCs at reading distance until complete.
 * 
 * @license Released into the public domain.
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#include <stdio.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
SoftwareSerial portOne(2, 3);

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  pinMode(7, OUTPUT);
	Serial.begin(115200);		// Initialize serial communications with the PC
  portOne.begin(115200);
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  inputString.reserve(200);
}

void loop() {
	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}

  unsigned long num4;
  unsigned long num3;
  unsigned long num2;
  unsigned long num1;
  //hex_num =  mfrc522.uid.uidByte[0] << 24;
  //hex_num += mfrc522.uid.uidByte[1] << 16;
  //hex_num += mfrc522.uid.uidByte[2] <<  8;
  num4 = mfrc522.uid.uidByte[0];
  num3 = mfrc522.uid.uidByte[1];
  num2 = mfrc522.uid.uidByte[2];
  num1 = mfrc522.uid.uidByte[3];
  mfrc522.PICC_HaltA(); 

  char buffer1 [50];
  char buffer2 [50];
  char buffer3 [50];
  char buffer4 [50];
  sprintf (buffer4, "%lu" , num4 );
  sprintf (buffer3, "%lu" , num3 );
  sprintf (buffer2, "%lu" , num2 );
  sprintf (buffer1, "%lu" , num1 );
  
  if(num1 != -1){
    Serial.print("A:"); Serial.print(num1);
    Serial.print(",B:"); Serial.print(num2);
    Serial.print(",C:"); Serial.print(num3);
    Serial.print(",D:"); Serial.println(num4);
    /*
    Serial.print(",D:"); Serial.println(num4);*/
    
    portOne.print("A:"); portOne.print(buffer1);
    portOne.print(",B:"); portOne.print(buffer2);
    portOne.print(",C:"); portOne.print(buffer3);
    portOne.print(",D:"); portOne.println(buffer4);
  }

  portOne.listen();
  while(1) {

    while(portOne.available() > 0) {
      char inChar = (char)portOne.read();
      inputString += inChar;
      if (inChar == 'Z') {
        stringComplete = true;
      }
      if( stringComplete ) {
        Serial.println(inputString);
        break;
      }
    }
    
    inputString = "";
    
    if( stringComplete ) {
      digitalWrite(7, HIGH);
      stringComplete = false;
      break;
    }
    
    
  }
  
  // Serial.write(hex_num);
   // portOne.write(hex_num);
}

/*void serialEvent() {
  while (portOne.available()) {
    // get the new byte:
    char inChar = (char)portOne.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == 'Z') {
      stringComplete = true;
    }
  }
}*/
