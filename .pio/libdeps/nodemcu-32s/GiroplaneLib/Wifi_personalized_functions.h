#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

char ssid[] = "IZZI-2124" ; 
char pass[] = "ECA9404C2124" ;
int status = WL_IDLE_STATUS;     // the Wifi radio's status

WebSocketsClient webSocket;

StaticJsonDocument<200> jsonDocTx;

bool wsconnected = false;
bool mensaje_recibido = true; 

void printWifiData() {

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);
}


void hexdump(const void *mem, uint32_t len, uint8_t cols = 16) {
	const uint8_t* src = (const uint8_t*) mem;
	Serial.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
	for(uint32_t i = 0; i < len; i++) {
		if(i % cols == 0) {
			Serial.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
		}
		Serial.printf("%02X ", *src);
		src++;
	}
	Serial.printf("\n");
}


void taskStatus(void)
{
  String output;  
  output = "";

  struct Quaternion Quat ; // Declarar cuaternio
  Quat = getQuaternion() ; //Calcular el cuaternio actual

  jsonDocTx.clear();
  jsonDocTx["QW"] = Quat.QW;
  jsonDocTx["QX"] = Quat.QX;
  jsonDocTx["QY"] = Quat.QY;
  jsonDocTx["QZ"] = Quat.QZ; 
  
  serializeJson(jsonDocTx, output);
  webSocket.sendTXT(output);  
}

float limite_inferior(float time0, int i){
  float limite = time0 + 100 + i*30  ; 
  return limite ; 
}

float limite_superior(float time0, int i){
  float limite = time0 + 100 + i*30 + 1 ; 
  return limite ; 
}

void Send_each_second(void){
while(1){
  webSocket.loop();
  float time0 = millis() ;
  int i = 1 ;
  while(wsconnected == true){
    webSocket.loop(); 
    float current_time = millis() ; 
    if(current_time >= limite_inferior(time0, i) && current_time <= limite_superior(time0, i)){
      taskStatus() ; 
      //Serial.println(current_time) ;
      i++ ; 
    }
    if(current_time > (limite_superior(time0, i) + 50)){
      break ; 
    } 
    //delay(10) ; 
  }
  delay(50); 
  }
}

void Send_constinously(void){
  webSocket.loop();
  if(wsconnected == true){
    webSocket.loop(); 
    if ( imu.fifoAvailable() )
  {
    // Use dmpUpdateFifo to update the ax, gx, mx, etc. values
    if ( imu.dmpUpdateFifo() == INV_SUCCESS)
    {
      taskStatus() ; 
  }
  }
  }
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

	switch(type) {
		case WStype_DISCONNECTED:
			Serial.printf("[WSc] Disconnected!\n");
      wsconnected = false;
			break;
		case WStype_CONNECTED:
			Serial.printf("[WSc] Connected to url: %s\n", payload);
      wsconnected = true;
			// send message to server when Connected
			webSocket.sendTXT("Connected from ESP32"); 
      mensaje_recibido = false;
			break;
		case WStype_TEXT:
			// Serial.printf("[WSc] get text: %s\n", payload);
      mensaje_recibido = true; 
			// send message to server
			// webSocket.sendTXT("message here");
			break;
		case WStype_BIN:
			Serial.printf("[WSc] get binary length: %u\n", length);
			hexdump(payload, length);

			// send data to server
			// webSocket.sendBIN(payload, length);
			break;
		case WStype_ERROR:			
		case WStype_FRAGMENT_TEXT_START:
		case WStype_FRAGMENT_BIN_START:
		case WStype_FRAGMENT:
		case WStype_FRAGMENT_FIN:
			break;
	}
}

void taskWifi(void)
{

  while (status != WL_CONNECTED) {

    Serial.print("Attempting to connect to WPA SSID: ");

    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);

  }

  Serial.print("You're connected to the network");

  printWifiData();

  Serial.println(WiFi.localIP());

  webSocket.begin("192.168.0.5", 8080, "/");

  Serial.println("Websocket begin finihed"); 
  //webSocket.begin();

  webSocket.onEvent(webSocketEvent) ;

  webSocket.setReconnectInterval(50);  
}




