/*
	Esp8266 Websockets Client

	This sketch:
        1. Connects to a WiFi network
        2. Connects to a Websockets server
        3. Sends the websockets server a message ("Hello Server")
        4. Prints all incoming messages while the connection is open

	Hardware:
        For this sketch you only need an ESP8266 board.

	Created 15/02/2019
	By Gil Maimon
	https://github.com/gilmaimon/ArduinoWebsockets

*/

#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>

const char* ssid = "MiFibra-12E8"; //Enter SSID
const char* password = "jcpersandra"; //Enter Password
const char* websockets_server_host = "192.168.1.143"; //Enter server adress
const uint16_t websockets_server_port = 9898; // Enter server port

using namespace websockets;

WebsocketsClient client;
const short int BUILTIN_LED1 = 2; //GPIO2 = 2 resistencia 1k conectado catodo al pin pio2 y el anodo +3,3v
bool led_estado=false;
// This function checks if a message is a text message with 
// the content "LED" 
bool isActivarLed(websockets::WebsocketsMessage& msg) {
  if(msg.isText()) {
    return msg.data() == "LED:A";
  } else {
    return false;
  }
}

void setup() {
    Serial.begin(115200);
    // inicializar GPIO 2 como salida 
    pinMode(BUILTIN_LED1, OUTPUT); 
    // Connect to wifi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    }

    // Check if connected to wifi
    if(WiFi.status() != WL_CONNECTED) {
        Serial.println("No Wifi!");
        return;
    }

    Serial.println("Connected to Wifi, Connecting to server.");
    // try to connect to Websockets server
     bool connected = client.connect(websockets_server_host, websockets_server_port, "/");
    if(connected) {
        Serial.println("Connected!");
        client.send("connected client");
    } else {
        Serial.println("Not Connected!");
    }
    
    
    // run callback when messages are received
    
    client.onMessage([&](WebsocketsMessage message) {
        Serial.print("Got Message: ");
        Serial.println(message.data());
        bool recibido=isActivarLed(message);
        if(recibido){
          digitalWrite(BUILTIN_LED1, LOW);
          led_estado=true;
        }else{
           digitalWrite(BUILTIN_LED1, HIGH);
           led_estado=false;
        }
        Serial.println(recibido);
     
    });
     
}

void loop() {
    // let the websockets client check for incoming messages
    if(client.available()) {
        client.poll();
    }
    delay(500);
}
