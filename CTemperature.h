#include <OneWire.h>
struct STemperatureInfo {
    String topic, data;
};
class CTemperature {
    public:
    /* Конструктор
        @param pin - пин шины дачиков темпереатуры
    */
    CTemperature ( uint8_t pin ):ds(pin){
    }
    STemperatureInfo handle( char* p_topic, bool & isError ) {
        byte addr[8];
        STemperatureInfo result;
        result.topic = "temp/";

        ds.reset_search();
        while (true)
        {
            if ( !ds.search(addr)) {
               isError = true;
               return STemperatureInfo();
            }
            String tmp_topic_name = result.topic;
            for (byte i = 0; i < 8; i++) {
              tmp_topic_name = tmp_topic_name + String (addr[i], HEX) + "_";
            }
            if ( tmp_topic_name.equals (p_topic)) {
              result.topic = tmp_topic_name;
              break;
            }
        }

        byte type_s;
        byte data[12];
        float celsius;

        if (OneWire::crc8(addr, 7) != addr[7]) {
            Serial.println("CRC is not valid!");
            isError = true;
            return STemperatureInfo();
        }
        
        // the first ROM byte indicates which chip
        switch (addr[0]) {
            case 0x10:
                type_s = 1;
                break;
            case 0x28:
                type_s = 0;
                break;
            case 0x22:
                //  Serial.println("  Chip = DS1822");
                type_s = 0;
                break;
            default:
                Serial.println("Device is not a DS18x20 family device.");
                isError = true;
                return STemperatureInfo();
        }

        ds.reset();
        ds.select(addr);
        ds.write(0x44, 1);

        delay(1000);

        ds.reset();
        ds.select(addr);
        ds.write(0xBE);

        for (byte i = 0; i < 9; i++) {
            data[i] = ds.read();
        }
        OneWire::crc8(data, 8);
        int16_t raw = (data[1] << 8) | data[0];
        if (type_s) {
            raw = raw << 3;
            if (data[7] == 0x10) {
                raw = (raw & 0xFFF0) + 12 - data[6];
            }
        } else {
            byte cfg = (data[4] & 0x60);

            if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
            else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
            else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms

        }


        celsius = (float)raw / 16.0;

        Serial.println(celsius);

        char temperaturenow [15];
        dtostrf(celsius, 7, 3, temperaturenow); //// convert float to char
        
        result.data = temperaturenow;
        return result;
    }
    private:
        OneWire ds;
};
