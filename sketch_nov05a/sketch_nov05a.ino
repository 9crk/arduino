void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  analogWriteFreq(50);
  analogWriteRange(1024);
  analogWrite(D7, 512);
}

int i = 1;
void loop() {
  // put your main code here, to run repeatedly:
   i++;
   if(i==1023){
      i=1;
   }
   analogWrite(D7, i);
   Serial.printf("i=%d\n",i);
   delay(100);
}
