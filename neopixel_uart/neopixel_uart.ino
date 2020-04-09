#include <Adafruit_NeoPixel.h>

#define LED_COUNT 50
#define LED_PIN 5
#define DELAYVAL 500
Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  pixels.begin();
  Serial.println("sys inited");
}
char buf;
char rgb[1024];
int bStart = -2;
int len = 0;
int ii=0;
void loop(){
  int ret = Serial.read(&buf,1);
  if(ret == 1){
    if(bStart == -2 && buf == 0xFE){//start
        //Serial.printf("start ");   
        bStart = -1;
    }else if(bStart == -1){
        len = buf;
        //Serial.printf("len=%d ",len);   
        bStart = 0;
    }else if(bStart >= 0 && bStart < 255){
        rgb[bStart] = buf;
        bStart++;
        //Serial.printf("start=%d ",bStart);   
        if(bStart == len){
          bStart = -2;
          //do send
          //Serial.printf("%d len= %d\n",ii++,len);
          for(int i=0; i<LED_COUNT; i++) {
            pixels.setPixelColor(i, pixels.Color(rgb[i*3], rgb[i*3+1], rgb[i*3+2]));
          }
          pixels.show();
        }
    }
  }
  
/*  pixels.clear();
  for(int i=0; i<LED_COUNT; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    pixels.show();
    delay(DELAYVAL);
  }*/
}
