
void motor_init()
{
  pinMode(D0,OUTPUT);
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
  pinMode(D3,OUTPUT);
}

static void motorCtl(unsigned char ctl)
{
  digitalWrite(D0,ctl&0x01);
  digitalWrite(D1,(ctl>>1)&0x01);
  digitalWrite(D2,(ctl>>2)&0x01);
  digitalWrite(D3,(ctl>>3)&0x01);  
}
void motor_left(int t1)
{
  /*motorCtl(0x02);
  delay(t1);
  motorCtl(0x09);
  delay(t1);
  motorCtl(0x0C);
  delay(t1);
  motorCtl(0x06);
  delay(t1);*/
  motorCtl(B00000101);delay(t1);
  motorCtl(B00001001);delay(t1);
  motorCtl(B00001010);delay(t1);
  motorCtl(B00000110);delay(t1);
  motorCtl(B00000101);delay(t1);
  motorCtl(B00001001);delay(t1);
  motorCtl(B00001010);delay(t1);
  motorCtl(B00000110);delay(t1);
  motorCtl(B00000000);
}
void motor_right(int t1)
{
  /*motorCtl(0x02);
  delay(t1);
  motorCtl(0x09);
  delay(t1);
  motorCtl(0x0C);
  delay(t1);
  motorCtl(0x06);
  delay(t1);*/
  motorCtl(B00000110);delay(t1);
  motorCtl(B00001010);delay(t1);
  motorCtl(B00001001);delay(t1);
  motorCtl(B00000101);delay(t1);
  motorCtl(B00000110);delay(t1);
  motorCtl(B00001010);delay(t1);
  motorCtl(B00001001);delay(t1);
  motorCtl(B00000101);delay(t1);
  motorCtl(B00000000);
}
void setup() {
  // put your setup code here, to run once:
  motor_init();
  Serial.begin(115200);
  Serial.println("init ok");
}

void loop() {
  motor_left(5);
  motor_left(5);

  delay(1000); 
  motor_right(5);
  motor_right(5);

  delay(1000); 
}
