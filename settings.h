///// ФАЙЛ НАСТРОЕК //////

// Параметры компиляции //

// Для того чтобы включить вывод подробных логов добавить #define DEBUG
#define DEBUG

// Параметры wifi //

#define WIFI_SSID "SkyNet_Home"
#define WIFI_PASSWORD "emperor12"

// Параметры MTQIT сервера //

#define MQTT_CLIENT_ID "relays"
#define MQTT_SERVER_IP "192.168.1.103"
#define MQTT_SERVER_PORT 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""

// Параметры реле//
// <топик состояния>,<топик переключения состояния>,<pin>
#define RELAY_1 "relay/1/status","relay/1/switch",5
#define RELAY_2 "relay/2/status","relay/2/switch",4
#define RELAY_3 "relay/3/status","relay/3/switch",0
#define RELAY_4 "relay/4/status","relay/4/switch",2
#define RELAY_5 "relay/5/status","relay/5/switch",14
#define RELAY_6 "relay/6/status","relay/6/switch",12
#define RELAY_7 "relay/7/status","relay/7/switch",13
#define RELAY_8 "relay/8/status","relay/8/switch",15

// Параметры датчиков температуры //
// Пин подключения шины
#define TEMP_PIN 3
