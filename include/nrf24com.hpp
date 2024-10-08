#ifndef __NRF24COM_HPP
#define __NRF24COM_HPP

#include <nRF24L01.h>
#include <RF24.h>
#include "data.hpp"
#include "utils.hpp"

#define CE_PIN PB12
#define CSN_PIN PA4

#define IRQ_PIN PB0

const byte Address[2][5] = {{ 0xAB, 0x8F, 0xDE, 0x9C, 0x37 }, { 0xAB, 0x8F, 0xDE, 0x9C, 0x38 }};


RF24 radio(CE_PIN, CSN_PIN);

char RXdata[128];
char TXdata[32];

String temp_string = "";
bool waiting_for_second_part = false;

bool newDataFromNRF = false;

void newRadioData();

void radioSetup() {
    radio.begin();

    radio.setChannel(125);

    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_HIGH);

    radio.openWritingPipe(Address[1]);
    radio.openReadingPipe(1, Address[0]);

    radio.maskIRQ(1, 1, 0);
    attachInterrupt(IRQ_PIN , newRadioData, FALLING);
    
    radio.startListening();
    delayMicroseconds(50);
}

bool radioTX(dataToNRF dataToNRFStruct) {
  String joinDataString = "<" + String(dataToNRFStruct.mode) + "," + String(dataToNRFStruct.pwml) +
                          "," + String(dataToNRFStruct.pwmr) + "," + String(dataToNRFStruct.startStop) +
                          "," + String(dataToNRFStruct.returnHome) + ">";

  bool rslt = false;

  if (joinDataString.length() > 30) {
    String parts[2];

    parts[0] = joinDataString.substring(0, 30);
    parts[1] = joinDataString.substring(30);

    parts[0] += "|";
    parts[1] = "|" + parts[1];

    radio.stopListening();
    delayMicroseconds(50);

    for (int i = 0; i < 2; i++) {
      parts[i].toCharArray(TXdata, sizeof(TXdata));
      int data_len = parts[i].length();

      rslt = radio.write(TXdata, data_len);
    }
  } else {
    joinDataString.toCharArray(TXdata, sizeof(TXdata));
    radio.stopListening();
    delayMicroseconds(50);
    rslt = radio.write(TXdata, sizeof(TXdata));
  }

  delayMicroseconds(50);
  radio.startListening();
  delayMicroseconds(50);

    return rslt;
}

void parseData(const char* input, dataFromNRF& result) {
  char* token = strtok(const_cast<char*>(input), ",");

  result.lat = atof(token);

  token = strtok(nullptr, ",");
  result.lon = atof(token);

  token = strtok(nullptr, ",");
  result.heading = atof(token);

  token = strtok(nullptr, ",");
  result.velocity = atof(token);

  token = strtok(nullptr, ",");
  result.numWaypoints = atoi(token);

  token = strtok(nullptr, ",");
  result.battery = atof(token);

  token = strtok(nullptr, ",");
  result.sonic = atoi(token);

  token = strtok(nullptr, ",");
  result.calibration = atoi(token);
}

String processString(String message) {
  if (message.startsWith("<")) {
    if (message.endsWith(">")) {
      message = message.substring(1, message.length() - 1);
      waiting_for_second_part = false;

      return message;
    }

    else if (message.endsWith("|")) {
      message = message.substring(1, message.length() - 1);
      temp_string = message;
      waiting_for_second_part = true;

    }

  } else if (message.startsWith("|")) {
    if (!waiting_for_second_part) {
      return "";

    } else if (message.endsWith(">")) {
      message = message.substring(1, message.length() - 1);
      temp_string += message;

      waiting_for_second_part = false;

      return temp_string;

    } else {
      waiting_for_second_part = false;
    }
  }
  
  return "";
}

void newRadioData() {
  newDataFromNRF = true;
}

void getRadioData() {
    if(radio.available() && newDataFromNRF) {
      radio.read(&RXdata, sizeof(RXdata));

      newDataFromNRF = false;
      String readData;

      readData = processString(String(RXdata));

      if(readData.length() > 0) {
          parseData(readData.c_str(), dataFromNRFStruct);
      }
  }
}

#endif //__NRF24COM_HPP