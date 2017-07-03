#include <DHT.h>
#include <SoftwareSerial.h>

#define DHT_DPIN          2
#define DHTTYPE           DHT11
#define SENSOR_MQ2_APIN   0
#define SENSOR_MQ7_APIN   1
#define SENSOR_MQ135_APIN 2
#define WIFI_RX_DPIN      12
#define WIFI_TX_DPIN      13
#define WIFI_POST_INTERVAL 5000
#define SERIAL_BAUD       115200

//*-- IoT Information
#define SSID "Tony_WiFi"
#define PASS "8860225231449"
#define IP "184.106.153.149" // ThingSpeak IP Address: 184.106.153.149

int sensor_mq2_avalue;
int sensor_mq7_avalue;
int sensor_mq135_avalue;
float sensor_temperature_dvalue;
float sensor_humidity_dvalue;

//*-- DHT11---
DHT dht(DHT_DPIN, DHTTYPE); // Initialize DHT sensor

SoftwareSerial debug( WIFI_RX_DPIN, WIFI_TX_DPIN ); // RX, TX
// 使用 GET 傳送資料的格式
// GET /update?key=[THINGSPEAK_KEY]&field1=[data 1]&filed2=[data 2]...;
String GET = "GET /update?key=M9WACHOGWKSX4IU9";

void setup() {
    Serial.begin( SERIAL_BAUD );
    debug.begin( SERIAL_BAUD );
    sendDebug("AT");
    Loding("sent AT");
    connectWiFi();
    
    dht.begin();
}
void loop() {
    sensor_mq2_avalue = analogRead(SENSOR_MQ2_APIN);
    sensor_mq7_avalue = analogRead(SENSOR_MQ7_APIN);
    sensor_mq135_avalue = analogRead(SENSOR_MQ135_APIN);
    
    sensor_humidity_dvalue = dht.readHumidity();
    sensor_temperature_dvalue = dht.readTemperature();
    
    delay(WIFI_POST_INTERVAL);
    Serial.println("+-------------------------------------+");
    Serial.print("MQ2:");
    Serial.println(sensor_mq2_avalue,DEC);
    Serial.print("MQ7:");
    Serial.println(sensor_mq7_avalue,DEC);
    Serial.print("MQ135:");
    Serial.println(sensor_mq135_avalue,DEC);
    Serial.print("Temperature:");
    Serial.println(sensor_temperature_dvalue);
    Serial.print("Humidity:");
    Serial.println(sensor_humidity_dvalue);
    Serial.println("");
    SentOnCloud( sensor_mq2_avalue, sensor_mq7_avalue, sensor_mq135_avalue, sensor_temperature_dvalue , sensor_humidity_dvalue );
}
boolean connectWiFi()
{
    debug.println("AT+CWMODE=1");
    Wifi_connect();
}

void SentOnCloud( int F1, int F2, int F3, int F4, int F5 )
{
    // 設定 ESP8266 作為 Client 端
    String cmd = "AT+CIPSTART=\"TCP\",\"";
    cmd += IP;
    cmd += "\",80";
    sendDebug(cmd);
    if( debug.find( "Error" ) )
    {
        Serial.print( "RECEIVED: Error\nExit1" );
        return;
    }
    cmd = GET + "&field1=" + F1 + "&field2=" + F2 + "&field3=" + F3 + "&field4=" + F4 + "&field5=" + F5;
    debug.print( "AT+CIPSEND=" );
    debug.println( String(cmd.length()) );
    Serial.print("cmd.length= "); //偵錯
    Serial.println( String(cmd.length()) ); //偵錯
    Serial.print("debug s1: cipsend? "); //偵錯
    Serial.println(debug.println( "AT+CIPSEND?" )); //偵錯
    //delay(2000);
    if(debug.find( ">" ) )
    {
        Serial.print("debug: >");
        Serial.print(cmd);
        debug.print(cmd);
    }
    else
    {
        Serial.println( "AT+CIPCLOSE" );
        debug.print( "AT+CIPCLOSE" );
    }
    if( debug.find("OK") )
    {
        Serial.println( "Send: OK" );
    }
    else
    {
        Serial.println( "Send: Error\nExit3" );
    }
}
void Wifi_connect()
{
    String cmd="AT+CWJAP=\"";
    cmd+=SSID;
    cmd+="\",\"";
    cmd+=PASS;
    cmd+="\"";
    sendDebug(cmd);
    Loding("Wifi_connect");
}
void Loding(String state){
    for (int timeout=0 ; timeout<10 ; timeout++)
    {
      if(debug.find("OK"))
      {
          Serial.println("RECEIVED: OK");
          break;
      }
      else if(timeout==9){
        Serial.print( state );
        Serial.println(" fail...\nExit2");
      }
      else
      {
        Serial.print("Wifi Loading...");
        delay(500);
      }
    }
}
void sendDebug(String cmd)
{
    Serial.print("SEND: ");
    Serial.println(cmd);
    debug.println(cmd);
} 
