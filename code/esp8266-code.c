
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Alles waar de niks moete aanpasse
#define MQTT_HOST "b6hhlb.messaging.internetofthings.ibmcloud.com"
#define MQTT_PORT 1883
#define MQTT_DEVICEID "d:b6hhlb:MQTT-client:desktop-client"
#define MQTT_USER "use-token-auth"
#define MQTT_TOKEN "nKpHBwekSK7F7kDe@Q"
#define MQTT_TOPIC "iot-2/evt/status/fmt/json"
#define MQTT_TOPIC_DISPLAY "iot-2/cmd/change/fmt/json"
const char* ssid = "ucll-projectweek-IoT";
const char* password = "Foo4aiHa";
const char* mqtt_server = "b6hhlb.messaging.internetofthings.ibmcloud.com";

// plaats van pins

const int lightDetector1 = 5;
const int lightDetector2 = 4;
const int rood1 = 14;
const int groen1 = 12;
const int blauw1 = 13;
const int rood2 = 16;
const int groen2 = 0;
const int blauw2 = 2;
const int pijl1 = 15;
const int pijl2 = 3;
const int pijl3 = 10;

// values
int lightDet1 = 0;
int lightDet2 = 0;
int firstplace = 3;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // pins
  pinMode(lightDetector1, INPUT);
  pinMode(lightDetector2, INPUT);
  pinMode(rood1, OUTPUT);
  pinMode(groen1, OUTPUT);
  pinMode(blauw1, OUTPUT);
  pinMode(rood2, OUTPUT);
  pinMode(groen2, OUTPUT);
  pinMode(blauw2, OUTPUT);
  pinMode(pijl1, OUTPUT);
  pinMode(pijl2, OUTPUT);
  pinMode(pijl3, OUTPUT);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
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
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(MQTT_DEVICEID, MQTT_USER, MQTT_TOKEN)) {
      Serial.println("connected");
      if(client.subscribe(MQTT_TOPIC_DISPLAY)){
        Serial.println("Subscribed");
      }
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
//=======================================================================
//========== HIER STOPT DE SETUP, AFBLIJVE ==============================
//=======================================================================

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on
    Serial.println("LED ON");
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off
    Serial.println("LED OFF");
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

    Serial.println("Sending info");
    String payload = "{ \"d\" : {";
    payload += "\"LightDetector1\":\""; payload += lightDet1; payload += "\",";
    payload += "\"LightDetector2\":\""; payload += lightDet2; payload += "\",";
    payload += "\"FirstPlace\":\""; payload += firstplace; payload += "\"";
    payload += "}}";
    Serial.println(payload);

    if (client.publish(MQTT_TOPIC, (char*) payload.c_str())) {
      Serial.println("Publish ok");
    } else {
      Serial.println("Publish failed");
    }
    delay(500);

  // try lightdetectors
  lightDet1 = digitalRead(lightDetector1);
  Serial.println(lightDet1);
  if(lightDet1 == 0){
    digitalWrite(rood1, 0);
    digitalWrite(groen1, 255);
    digitalWrite(blauw1, 0);
    if(firstplace > 1){
      firstplace = 1;
    }
  }else{
    digitalWrite(rood1, 255);
    digitalWrite(groen1, 0);
    digitalWrite(blauw1, 0);
    if(firstplace == 1){
      firstplace = 3;
    }
  }
  lightDet2 = digitalRead(lightDetector2);
  Serial.println(lightDet2);
  if(lightDet2 == 0){
    digitalWrite(rood2, 0);
    digitalWrite(groen2, 255);
    digitalWrite(blauw2, 0);
    if(firstplace > 2){
      firstplace = 2;
    }
  }else{
    digitalWrite(rood2, 255);
    digitalWrite(groen2, 0);
    digitalWrite(blauw2, 0);
    if(firstplace == 2){
      firstplace = 3;
    }
  }

  if(firstplace == 1){
    digitalWrite(pijl1, HIGH);
    digitalWrite(pijl2, LOW);
    digitalWrite(pijl3, LOW);
  }else if(firstplace == 2){
    digitalWrite(pijl1, HIGH);
    digitalWrite(pijl2, HIGH);
    digitalWrite(pijl3, LOW);
  }else if(firstplace == 3){
    digitalWrite(pijl1, HIGH);
    digitalWrite(pijl2, HIGH);
    digitalWrite(pijl3, HIGH);
  }
}

// ik bne hier ook niet trots op, "If it works, dont touch it"
