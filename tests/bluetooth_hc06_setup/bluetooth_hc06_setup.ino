#define SerialPC Serial
#define SerialBluetooth Serial2
#define ledPin 13

String NEXT_DIRECTION_CHANGE = String("NEXT_DIRECTION_CHANGE");

/* Global variables */
String lastBluetoothMessage;
String bluetoothMessageBuffer = "";

String nextDirectionChange = "";
String nextDirectionChangeDistance = "";
String distanceToDestination = "";
String timeToDestination = "";
String errorLevel = "";
String text = "";
String bluetoothStatus = "";

void readSerialBluetooth();
void writeSerialBluetooth();
void processBluetoothCommand(String command);

void setup()
{
  /* Init led */
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  /* Serial connections initialization */
  SerialPC.begin(9600); // PC
  SerialBluetooth.begin(9600); // Bluetooth Card

  /* Setup bluetooth module */
  writeSerialBluetooth(String("AT"));
  delay(1000);
  writeSerialBluetooth(String("AT+NAMEINC_Group3"));
  delay(1000);
  writeSerialBluetooth(String("AT+PIN4242"));
  delay(1000);
}

void loop() {
  /*
  while(Serial.available()) {//while there is data available on the serial monitor
    message+=char(Serial.read());//store string from serial command
  }
  if(!SerialPC.available()) {//This comes from the console
    if(message!="") {//if data is available
      SerialPC.println("<" + message); //show the data
      SerialBluetooth.write(message.c_str());
      if(message=="L0") digitalWrite(ledPin, LOW);    // set the LED off
      if(message=="L1") digitalWrite(ledPin, HIGH);    // set the LED on
      message=""; //clear the data
    }
  }
  */

  readSerialBluetooth();
  processBluetoothCommand();
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
    SerialPC.println("Bluetooth > " + command);
    // $poi,1000,r,2,135,0;
    if (command.substring(1, 4) == "poi") {
      int distance, sortie, angle, erreur;
      char type;
      sscanf(command.c_str(), "$poi,%d,%c,%d,%d,%d;", &distance, &type, &sortie, &angle, &erreur);
      SerialPC.println(distance);
      SerialPC.println(sortie);
      SerialPC.println(angle);
      SerialPC.println(erreur);
      SerialPC.println(type);
    }
    lastBluetoothMessage = "";
  }
}


