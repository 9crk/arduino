#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>

#define LED_COUNT 50
#define LED_PIN 5
#define DELAYVAL 500
Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void wifi_go()
{
#if 0  
  WiFi.mode(WIFI_STA);
  WiFi.begin("z24", "ihoment123");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.print("connected, address=%s\n");
  Serial.print(WiFi.localIP());
#else
   WiFi.softAP("zhouhua");
   Serial.print(WiFi.softAPIP());
#endif
}

#define MAX_SRV_CLIENTS 10
WiFiServer server(12346);
WiFiClient clt;
void server_go()
{
  server.begin();
  server.setNoDelay(true);
}
void setup() {
  Serial.begin(115200);
  pixels.begin();
  wifi_go();
  server_go();
  Serial.println("sys inited");
}
uint8_t buf[1024];
timeval tv;
long tt,rt,lt;
int bShow=0;
void loop(){
  #if 1
  if(server.hasClient()){
    clt = server.available();
    Serial.println("connected");
  }
  if (clt.available()) {
    int ret = clt.read(buf,2);
    if(ret > 0 && buf[0] == 0xFE){
       int len = buf[1];
       ret = clt.read(buf,len);
       if(ret == len){
          Serial.printf("%d\n",ret);
          for(int i=0; i<LED_COUNT; i++) {
             pixels.setPixelColor(i, pixels.Color(buf[i*3], buf[i*3+1], buf[i*3+2]));
          }
          pixels.show();
       }
    }
  }

  /*gettimeofday(&tv, NULL);
  tt = tv.tv_sec*1000+tv.tv_usec;
  bShow = 0;
  long delta = tt - lt;
  if(delta > 200){
    lt = tt;
    Serial.printf("%ld \n",tt);
    pixels.show();
    bShow = 1;
  }else{
    Serial.printf(".");
  }*/
  #else
 
  
  pixels.clear();
  for(int i=0; i<LED_COUNT; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    pixels.show();
    delay(DELAYVAL);
  }
  #endif
}
