#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

const char* ssid = "Students";
const char* password = "p4cm4n123";
const uint16_t port = 8888;
const char * host = "192.168.213.138"; 

SoftwareSerial swSer(14, 12, false, 128);

void setup() {
  Serial.begin(115200);
  swSer.begin(115200);

  Serial.println("\nSoftware serial test started");

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {  

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(host, port)) {
        Serial.println("connection failed");
        Serial.println("wait 5 sec...");
        delay(5000);
        return;
  }

  String data = "";
  while (swSer.available()) {
    char character = swSer.read();
    data.concat(character);
    Serial.write(swSer.read());
    yield();
  }

  
  // This will send the request to the server
  //client.print("Send this data to server");

  //read back one line from server
  //String line = client.readStringUntil('\r');
  //client.println(line);

  Serial.println("closing connection");
  client.stop();
    
  Serial.println("wait 5 sec...");
  delay(5000);
}
