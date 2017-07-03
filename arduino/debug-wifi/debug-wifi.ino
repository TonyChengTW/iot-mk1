#include <SoftwareSerial.h>

#define WIFI_RX_DPIN      12
#define WIFI_TX_DPIN      13
#define WIFI_POST_INTERVAL 10000
#define SERIAL_BAUD       115200

//*-- IoT Information
#define SSID "Tony_WiFi"
#define PASS "8860225231449"
#define IP "192.168.178.2" // Tony IoT Mark1 IP Address: 122.116.224.14

SoftwareSerial debug( WIFI_RX_DPIN, WIFI_TX_DPIN ); // RX, TX

void setup() {
    Serial.begin( SERIAL_BAUD );
    debug.begin( SERIAL_BAUD );
    debug.println("AT");
    Loading("sent AT");
    connectWiFi();
}

void loop() {

}

boolean connectWiFi()
{
    debug.println("AT+CWMODE=1");
    Wifi_connect();
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

