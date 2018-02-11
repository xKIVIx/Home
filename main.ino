#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "settings.h"
#include "CRelay.h"
#include "CTemperature.h"

#define MQTT_VERSION MQTT_VERSION_3_1_1

WiFiClient wifiClient;
PubSubClient client (wifiClient);


/////////////////////////// RELAY ////////////////////////////////////////////
// Количесвто реле
const uint8_t countRelays = 8;
// Список реле
CRelay relays [countRelays] = {
    CRelay ( RELAY_1 ),
    CRelay ( RELAY_2 ),
    CRelay ( RELAY_3 ),
    CRelay ( RELAY_4 ),
    CRelay ( RELAY_5 ),
    CRelay ( RELAY_6 ),
    CRelay ( RELAY_7 ),
    CRelay ( RELAY_8 )
};

bool relaysHandle (char* p_topic, String payload) {
    // handle message topic
    for (uint8_t id = 0 ; id < countRelays; id++) {
        #ifdef DEBUG
        Serial.println("Handle relay "+id);
        #endif
        if ( relays[id].handle ( p_topic, payload ) ) {
            if ( relays[id].getState() ) {
                client.publish( relays[id].getStateTopic().c_str(), 
                                RELAY_ON );
                #ifdef DEBUG
                Serial.println(relays[id].getStateTopic());
                #endif
            } else {
                client.publish( relays[id].getStateTopic().c_str(), 
                                RELAY_OFF );
                #ifdef DEBUG
                Serial.println(relays[id].getStateTopic());
                #endif
            }
            #ifdef DEBUG
            Serial.println("ok");
            #endif
            return true;
        }
        #ifdef DEBUG
        Serial.println("none");
        #endif
    }
    return false;
};

void initRelays()
{
    for (uint8_t id = 0; id < countRelays; id++)
        relays[id].init();
};
/////////////////////////////////////// Temperature //////////////////////////////////////////

CTemperature temperature (TEMP_PIN);

/////////////////////////////////////// MAIN FUNCTIONS //////////////////////////////////////////

void callback(char* p_topic, byte* p_payload, unsigned int p_length) {
    // concat the payload into a string
    String payload;
    for (uint8_t i = 0; i < p_length; i++) {
        payload.concat((char)p_payload[i]);
    }
    #ifdef DEBUG
    Serial.println("Request topic");
    Serial.println(p_topic);
    #endif
    if (relaysHandle( p_topic, payload ))
        return;
    bool isError = false;
    #ifdef DEBUG
    Serial.println("Temperature handle");
    #endif
    STemperatureInfo tempInfo = temperature.handle( p_topic, isError );
    if (!isError){
      client.publish( tempInfo.topic.c_str(), 
                      tempInfo.data.c_str() );
      return;
    }
    Serial.println("Unknown topic");
    Serial.println(p_topic);
    return;
};

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("INFO: Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
            Serial.println("INFO: connected");
            // Once connected, publish an announcement...

        } else {
            Serial.print("ERROR: failed, rc=");
            Serial.print(client.state());
            Serial.println("DEBUG: try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
};

void setup() {
    // init the serial
    Serial.begin(115200);

    // init the WiFi connection
    #ifdef DEBUG
    Serial.print("INFO: Connecting to ");
    #endif
    
    WiFi.mode(WIFI_STA);
    #ifdef DEBUG
    Serial.println(WIFI_SSID);
    #endif
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    #ifdef DEBUG
    Serial.println("");
    Serial.println("INFO: WiFi connected");
    Serial.print("INFO: IP address: ");
    Serial.println(WiFi.localIP());
    #endif
    // init the MQTT connection
    client.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
    client.setCallback(callback);

    // init the relays
    initRelays();
};

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
};
