#define SerialPC Serial
#define SerialBluetooth Serial1
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
    bluetoothMessageBuffer+=char(SerialBluetooth.read());
    
    if (
      bluetoothMessageBuffer.length() >= 4 &&
      bluetoothMessageBuffer.substring(bluetoothMessageBuffer.length() - 4) == " END"
    ) {
      lastBluetoothMessage = bluetoothMessageBuffer.substring(0, bluetoothMessageBuffer.length() - 4);
      bluetoothMessageBuffer = "";
      SerialPC.println("Bluetooth > " + lastBluetoothMessage);
      break;
    }
  }
}

void processBluetoothCommand() {
  String command = lastBluetoothMessage;
  if (command != "" && command.substring(0, 4) == "SET ") {
    command = command.substring(4);
    if (command.substring(0, NEXT_DIRECTION_CHANGE.length()) == NEXT_DIRECTION_CHANGE) {
      nextDirectionChange = command.substring(NEXT_DIRECTION_CHANGE.length() + 1);
      lastBluetoothMessage = "";
      SerialPC.println("SET NEXT_DIRECTION_CHANGE = " + nextDirectionChange);
    }
  }
}


