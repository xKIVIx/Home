// payloads by default (on/off)
const char* RELAY_ON = "1";
const char* RELAY_OFF = "0";

class CRelay {
public:
    /*
       Конструктор
       @param stateTopic - топик где публикуется состояние реле. Пример "relay/1/status"
       @param commandTopic - топик запрос на который меняет статус реле
       @param pin - пин на который подключено реле
       @param isOn - статус реле false = выключено, true = включено
    */
    CRelay( String stateTopic, String commandTopic, uint8_t pin ) {
        stateTopic_ = stateTopic;
        commandTopic_ = commandTopic;
        pin_ = pin;
        isOn_ = false;
        #ifdef DEBUG
        Serial.println( "Relay ( pin " + String ( pin_ ) + ") is create" );
        #endif
    };

    /*
       Включает или выключает реле в зависимости от предыдущего состояния
       @return текущее состояние
    */
    String switchState() {
        if ( !isOn_ ) {
            isOn_ = true;
            digitalWrite( pin_, HIGH );
            #ifdef DEBUG
            Serial.println( String( "INFO: Turn relay on... pin: " + String ( pin_ ) ) );
            #endif
            return RELAY_ON;

        } else {
            isOn_ = false;
            digitalWrite( pin_, LOW);
            #ifdef DEBUG
            Serial.println( String( "INFO: Turn relay off... pin: " + String ( pin_ ) ) );
            #endif
            return RELAY_OFF;
      }
    };

    /*
       Обработчик запроса
       @param p_topic - вызывающий топик
       @param payload - тело запроса
       @return - true = если запрос обработан, false = запрос не обработан
    */
    bool handle ( char* p_topic, String payload ) {
        if ( String( commandTopic_ ).equals( p_topic )) {
            // test if the payload is equal to "ON" or "OFF"
            if ( payload.equals( String( RELAY_ON ) ) ) {
                    if ( !isOn_ )
                        switchState();
                    return true;
                } else if ( payload.equals( String( RELAY_OFF ) ) ) {
                    if ( isOn_ )
                        switchState();
                    return true;
                }
              };
              return false;
            };

    /*
       Функция иницилизации реле.
    */
    void init() {
        pinMode(pin_, OUTPUT);
        digitalWrite( pin_, LOW);
        #ifdef DEBUG
        Serial.println( "Relay ( pin " + String ( pin_ ) + ") is init" );
        #endif
    };
    
    /*
        Возвращает состояние реле
      */
    bool getState () {
        return isOn_;
    }
    
    const String & getStateTopic (){
        return stateTopic_;
    }
private:
    String stateTopic_,
           commandTopic_;
    uint8_t pin_;
    bool isOn_;
};