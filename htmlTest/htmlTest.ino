/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <WiFi.h>
#include <Wire.h>



static uint8_t _i2caddr = 0x1A;
static inline uint8_t i2cread(void) {
  #if ARDUINO >= 100
  return Wire.read();
  #else
  return Wire.receive();
  #endif
}
static inline void i2cwrite(uint8_t x) {
  #if ARDUINO >= 100
  Wire.write((uint8_t)x);
  #else
  Wire.send(x);
  #endif
}
void writeReg(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(_i2caddr);
  i2cwrite((uint8_t)reg);
  i2cwrite((uint8_t)(value));
  Wire.endTransmission();
}
uint8_t readReg(uint8_t reg) {
    Wire.beginTransmission(_i2caddr);
    i2cwrite(reg);
    Wire.endTransmission(false); // MMA8451 + friends uses repeated start!!
    Wire.requestFrom(_i2caddr, 1);
    if (! Wire.available()) return -1;
    return (i2cread());
}
int readBytes(uint8_t reg,uint8_t buf,int len){
    Wire.beginTransmission(_i2caddr);
    i2cwrite(reg);
    Wire.endTransmission(false); // MMA8451 + friends uses repeated start!!
    Wire.requestFrom(_i2caddr, 1);
    if (! Wire.available()) return -1;
    for(int i=0;i<len;i++){
      buf[i] = i2cread();
    }
    Wire.endTransmission(true);
}
uint8_t tmpArray[32*32];
void ir_init(){
  uint8_t ret;
  Serial.printf("lets start init ir-sensor\n");
  tmpArray[32] = 0xFF;
  tmpArray[164] = 0xFF;
  writeReg(0x01,0x01);
  writeReg(0x03,0x0C);
  writeReg(0x04,0x0C);
  writeReg(0x05,0x0C);
  writeReg(0x06,0x14);
  writeReg(0x07,0x0C);
  writeReg(0x08,0x0C);
  writeReg(0x09,0x88);
  writeReg(0x01,0x09);
  ret = readReg(0x02);
  Serial.printf("read1  = %02x\n",ret);
  delay(40);//30ms
  ret = readReg(0x02);
  Serial.printf("read2  = %02x\n",ret);
}
void ir_sleep(){
  writeReg(0x01,0x00);
}
void ir_getFrame(){
  readBytes(0x0A,tmpArray,1024);
}
WiFiServer server(80);
String header;

void setup() {
  Serial.begin(115200);
  WiFi.softAP("IR-Scanner","88888888");
  Wire.begin(32, 31);
  ir_init();
  /*
  const char* ssid     = "z24";
  const char* password = "ihoment123";
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  */
  server.begin();
}
char color[7];
void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {                             // If a new client connects,
    //Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /26/on") >= 0) {
            } else if (header.indexOf("GET /26/off") >= 0) {
            } else if (header.indexOf("GET /27/on") >= 0) {
            } else if (header.indexOf("GET /27/off") >= 0) {
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            // Web Page Heading
            client.println("<body><center><h1>IR-Scanner</h1></center>");
            //client.println("<script language=\"JavaScript\">setTimeout(function(){location.reload()},1000);</script>");
            client.println("<head><meta http-equiv=\"refresh\" content=\"1\"></head>");
            
            client.println("<center><table border=\"0\">");
            for(int i=0;i<32;i++){//1000*30
              client.println("<tr>");
              for(int j=0;j<32;j++){
                sprintf(color,"%02x0000",tmpArray[i*32 + j]);
                client.print("<td height=20 width=20 bgcolor=#");
                client.print(color);
                client.println("></td>");
              }
              client.println("</tr>");
            }
            client.println("</table></center>");
            
            
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
