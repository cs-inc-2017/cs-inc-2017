#include "bluetooth.h"

#define SerialPC Serial
#define SerialBluetooth Serial2
#define ledPin 13

/* Global variables */
String lastBluetoothMessage = "";
String bluetoothMessageBuffer = "";

float target_latitude;
float target_longitude;
float current_latitude;
float current_longitude;

void startBluetooth() {
        // this is the magic trick for scanf to support float
        asm (".global _scanf_float");

        /* Init led */
        pinMode(ledPin, OUTPUT);
        digitalWrite(ledPin, HIGH);

        /* Serial connections initialization */
        SerialPC.begin(9600); // PC
        SerialBluetooth.begin(9600); // Bluetooth Card

        /* Setup bluetooth module */
        writeSerialBluetooth(String("AT"));
        delay(1000); // let the bluetooth module process the command
        writeSerialBluetooth(String("AT+NAMEINC_Group3"));
        delay(1000);
        writeSerialBluetooth(String("AT+PIN4242"));
        delay(1000);
}


void writeSerialBluetooth(String string) {
        SerialPC.println("Bluetooth < " + string);
        SerialBluetooth.write(string.c_str());
}

void readSerialBluetooth() {
        while(SerialBluetooth.available()) {
                char readChar = char(SerialBluetooth.read());
                if (bluetoothMessageBuffer != "" || readChar == '$') {
                        if (bluetoothMessageBuffer == "") {
                                SerialPC.println("");
                        }
                        bluetoothMessageBuffer += readChar;
                } else {
                        SerialPC.print(readChar);
                }

                // Break if last char is ;
                if (
                        bluetoothMessageBuffer.length() >= 1 &&
                        bluetoothMessageBuffer.substring(bluetoothMessageBuffer.length() - 1) == ";"
                        ) {
                        lastBluetoothMessage = bluetoothMessageBuffer;
                        bluetoothMessageBuffer = "";
                        SerialPC.println("Bluetooth > " + lastBluetoothMessage);
                        break;
                }
        }
}

void processBluetoothCommand() {
        String command = lastBluetoothMessage;
        if (command != "" && command.substring(command.length() - 1) == ";") {
                // $poi,1000,r,2,135,0;
                lastBluetoothCommandMillis = millis();
                if (command.substring(1, 4) == "poi") {
                        int distance, sortie, angle, erreur, ttd, dtd; char type;
                        char streetBuffer [16];
                        sscanf(command.c_str(), "$poi,%d,%c,%d,%d,%d,%d,%d,%[^;]s;", &distance, &type, &sortie, &angle, &erreur, &ttd, &dtd, streetBuffer);
                        currentDistance = distance;
                        currentExit = sortie;
                        currentAngle = angle;
                        currentError = erreur;
                        currentType = type;
                        if (ttd != timeToDestination || dtd != distanceToDestination) {
                                arrivalTextNeedsRefresh = true;
                        }
                        timeToDestination = ttd;
                        distanceToDestination = dtd;
                        if (strcmp(streetBuffer, nextStreet) != 0) {
                                nextStreetNeedsRefresh = true;
                                strcpy(nextStreet, streetBuffer);
                        }
                } else if (command.substring(1, 4) == "pos") {
                        if (command.substring(5, 12) == "current") {
                                sscanf(command.c_str(), "$pos,current,%f,%f;", &current_latitude, &current_longitude);
                        }
                        if (command.substring(5, 11) == "target") {
                                sscanf(command.c_str(), "$pos,target,%f,%f;", &target_latitude, &target_longitude);
                        }
                }
                lastBluetoothMessage = "";
        }
}
