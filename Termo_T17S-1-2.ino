#include <iarduino_OLED_txt.h>
#include <BME280I2C.h>
#include <Wire.h>
#define SERIAL_BAUD 115200
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
boolean butt_flag = 0;
boolean butt;
boolean led_flag = 0;


float temp(NAN), hum(NAN), pres(NAN), PR;
const char* ssid = "MGTS_GPON_7F5C";
const char* password = "192RTU_qpg173";
WiFiServer server(80);                  //Port 80
BME280I2C bme;    
iarduino_OLED_txt myOLED(0x3C);

extern uint8_t MediumFontRus[];
boolean lamp1;     
boolean lamp2;
     


void setup()
{
  
  myOLED.begin();
  pinMode(14, INPUT_PULLUP); // кнопка вкл
  pinMode(12, OUTPUT); // 
  pinMode(13, OUTPUT); //
  Serial.begin(SERIAL_BAUD);



  
  // Let's connect to wifi network 
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password); 
  Wire.begin();
 while (WiFi.status() != WL_CONNECTED) //Waiting for the responce of wifi network
 {
    delay(500);
    Serial.print(".");
 }
    Serial.println("");
    Serial.println("Connection Successful");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());   //Getting the IP address at which our webserver will be created
    Serial.println("Type the above IP address into a browser search bar");
    server.begin();                   //Starting the server



  
  while(!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }
 

}


void loop()
{
   butt = !digitalRead(14);
if (butt == 1 && butt_flag == 0) {
  butt_flag = 1;
  Serial.println("press");
  led_flag = !led_flag;
  digitalWrite (12, led_flag);
  digitalWrite (13, led_flag);
  delay(100);
}
if (butt == 0 && butt_flag == 1) {
  butt_flag = 0;
  Serial.println("realeased");
}


  lamp1= digitalRead(12);
  lamp2= digitalRead(13);
  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_Pa);
   bme.read(pres, temp, hum, tempUnit, presUnit);
   PR=pres*0.00750062;
   Serial.print("Temp: ");
   Serial.print(temp);
   Serial.print("C");
   Serial.print("\t\tHumidity: ");
   Serial.print(hum);
   Serial.print("% RH");
   Serial.print("\t\tPressure: ");
   Serial.print(PR);
   Serial.println(" mm");
   myOLED.clrScr();  // Чистим экран.
   myOLED.clrScr(); 
   myOLED.setFont(MediumFontRus);  // включаем шрифт                      
   myOLED.print(F("Svetlachok"),    1,      1); 
   myOLED.print(F("Temp="),    0,      3); 
   myOLED.print((temp),    60,      3);
   myOLED.print(F("Himi="),    0,      5);
   myOLED.print((hum),    60,      5); 
   myOLED.print(F("Pres="),    0,      7); 
   myOLED.print((PR),    60,      7);


WiFiClient client = server.available();   //Checking for incoming clients

    
            client.print("<html><title>Svetlyachok_1</title></html>");
            client.print("<body bgcolor=\"#E6E6FA\"><h1 style=\"text-align: center; color: blue\">Svetlyachok_1</h1>");
            client.print("<p style=\"text-align: center; font-size:150% \">Temperature is: ");
            client.print(temp);
            client.print(" C");
            client.print("<br/>Humidity is: ");
            client.print(hum);
            client.print(" %");
            client.print("<br/>Pressure is: ");
            client.print(PR);
            client.print(" mm");
            client.print("<br/>Button: ");
            client.print(led_flag);
            client.print("<br/>Lamp_1: ");
            client.print(lamp1);
            client.print("<br/>Lamp_2: ");
            client.print(lamp2);
            client.print("</p></body>");
            client.print("<meta http-equiv='Refresh' content='5' />");
 










   
   
   delay(1200); 
}
