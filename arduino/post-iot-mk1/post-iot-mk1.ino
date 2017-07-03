#include <DHT.h>
#include <SoftwareSerial.h>

#define DHT_DPIN          2
#define DHTTYPE           DHT11
#define SENSOR_MQ2_APIN   0
#define SENSOR_MQ7_APIN   1
#define SENSOR_MQ135_APIN 2
#define WIFI_RX_DPIN      12
#define WIFI_TX_DPIN      13
#define WIFI_POST_INTERVAL 10000
#define SERIAL_BAUD       115200

//*-- IoT Information
#define SSID "Tony_WiFi"
#define PASS "8860225231449"
#define IP "192.168.178.2" // Tony IoT Mark1 IP Address: 122.116.224.14
int sensor_mq2_avalue;
int sensor_mq7_avalue;
int sensor_mq135_avalue;
float sensor_temperature_dvalue;
float sensor_humidity_dvalue;

//*-- DHT11---
DHT dht(DHT_DPIN, DHTTYPE); // Initialize DHT sensor
SoftwareSerial debug( WIFI_RX_DPIN, WIFI_TX_DPIN ); // RX, TX

void setup() {
    Serial.begin( SERIAL_BAUD );
    debug.begin( SERIAL_BAUD );
    debug.println("AT+RST");
    debug.println("AT");
    Loading("sent AT");
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
    // SentOnCloud( sensor_mq2_avalue, sensor_mq7_avalue, sensor_mq135_avalue, sensor_temperature_dvalue , sensor_humidity_dvalue );
    SentOnCloud();
}
boolean connectWiFi()
{
    debug.println("AT+CWMODE=1");
    Wifi_connect();
}

void SentOnCloud()
{
    // 設定 ESP8266 作為 Client 端
    String cmd = "AT+CIPSTART=\"TCP\",\"";
    cmd += IP;
    cmd += "\",7878";
    //String cmd = "AT+CIPSTART=\"TCP\",\"192.168.178.2\",7878";
    debug.println(cmd);
    // cmd = "";  // 清除 cmd 變數內容
    if( debug.find( "Error" ) )
    {
        Serial.print( "RECEIVED: Error\nExit1" );
        return;
    }

// Tony - IoT Mark1 POST
    String body = "{\"measurement\":\"temperature\",\"value\":40,\"location\":\"Gobin-4F12\",\"place\":\"My Desktop\"}";
    String header = "POST /v1/iot-mk1 HTTP/1.1\r\n";

    // header += "User-Agent: Tony KTduino UNO 1/1.0\r\n";
    header += "Content-Type: application/json\r\n";
    header += "Content-Length: " + String(body.length()) + "\r\n";
    header += "\r\n";
    String cmd2 = header + body;
    
    Serial.print("debug: cmd2=");
    Serial.println(cmd2);
    Serial.print("debug: cmd2 length=");
    Serial.println(cmd2.length());
    
    debug.print( "AT+CIPSEND=" );
    debug.println( cmd2.length() );
    delay(1000);  //等待 大於符號 回應
    if(debug.find( ">" ) )
    {
        Serial.print("debug: >");
        Serial.print(cmd2);
        debug.print(cmd2);
    }
    else
    {
        Serial.println( "AT+CIPCLOSE" );
        debug.println( "AT+CIPCLOSE" );   // 關閉 IP 連線
    }

    if( debug.find("OK") )
    {
        Serial.println( "Send: OK" );
    }
    else
    {
        Serial.println( "Send: Sent Head+Body Failed \nExit3" );
        // Serial.print("debug s2: cipsend?= "); //偵錯
        // Serial.println(debug.println( "AT+CIPSEND?" )); //偵錯
    }
}
void Wifi_connect()
{
    String cmd="AT+CWJAP=\"";  //連線指定之基地台 (Join AP)
    cmd+=SSID;
    cmd+="\",\"";
    cmd+=PASS;
    cmd+="\"";
    debug.println(cmd);
    Loading("Wifi_connect");
}
void Loading(String state){
    for (int timeout=0 ; timeout<10 ; timeout++)
    {
      if(debug.find("OK"))
      {
          Serial.println("RECEIVED: OK");
          break;
      }
      else if(timeout==9){
        Serial.print( state );
        Serial.println(" Can not Wifi connect: AT+CWJAP. Timeout and giveup\nExit2");
        break;
      }
      else
      {
        Serial.print("Wifi Loading...");
        delay(2000);
      }
    }
}

