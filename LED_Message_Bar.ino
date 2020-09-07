#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted

#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define MAX_DEVICES 4
#define CLK_PIN   14
#define DATA_PIN  13
#define CS_PIN    15


//network credentials
const char* ssid = "ssid";
const char* password = "password";
String page = "";
ESP8266WebServer server(80);   //instantiate server at port 80 (http port)


// HARDWARE SPI
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// SOFTWARE SPI
//MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

uint8_t scrollSpeed = 60;    // default frame delay value
textEffect_t scrollEffect = PA_SCROLL_LEFT;
textPosition_t scrollAlign = PA_LEFT;
uint16_t scrollPause = 2000; // in milliseconds

// Global message buffers shared by Serial and Scrolling functions
#define  BUF_SIZE  150
char curMessage[BUF_SIZE] = { "" };
char newMessage[BUF_SIZE] = { "Talha & Yahya corp." };
bool newMessageAvailable = true;



void handleSubmit(void)
{
  else if(server.args() > 0 ) {
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "message") {
        int j=0;
        Serial.println(" hello \n");
        Serial.println(sizeof(server.arg(i)));
        while(server.arg(i)[j]!=NULL){
         newMessage[j] = server.arg(i)[j];
         j++;
         }
         newMessage[j]=NULL;
         newMessageAvailable = true;
      }
   }
}
 server.send(200, "text/html", page);
}

void setup()
{
  Serial.begin(9600);
  Serial.print("\n[Parola Scrolling Display]\nType a message for the scrolling display\nEnd message line with a newline");
  
  //the HTML of the web page
  page = "<!Doctype html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><link rel=\"icon\" href=\"data:,\"><style> .body { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;} .input6 { background-color: #195B6A; border: none; color: white; padding: 16px 40px;text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;} .input5 {background-color: #77878A;}</style></head><body><h1 align=\"center\">--LED MATRIX--</h1><form action='192.168.4.1\submit' method='POST'>Message:<br><input class=\"input5\" type=\"text\" id=\"message\" maxlength=\"100\"><br><input class=\"input6\" value='Submit' type='submit'></form></body></html>";
   
  delay(1000);
  Serial.begin(9600);
  WiFi.softAP(ssid, password); //begin WiFi access point
  Serial.println("");
 
  // Wait for connection
  if(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
      }
      
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP()); 
   
  server.on("/", [](){
    server.send(200, "text/html", page);
  });

  server.on("/", handleSubmit);
  
  server.begin();
  Serial.println("Web server started!");
  
  P.begin();
  P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
}

void loop()
{

  if (P.displayAnimate())
  {
    if (newMessageAvailable)
    {
      strcpy(curMessage, newMessage);
      newMessageAvailable = false;
    }
    P.displayReset();
  }
  server.handleClient();
}