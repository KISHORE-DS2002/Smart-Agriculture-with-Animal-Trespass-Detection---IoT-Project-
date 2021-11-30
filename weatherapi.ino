#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP_Mail_Client.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#define WIFI_SSID "DSK"
#define WIFI_PASSWORD "DSK@2002"
#define SMTP_server "smtp.gmail.com"
#define SMTP_Port 465
#define sender_email "kandippapesanum2002@gmail.com"
#define sender_password "kandippa@2021"
#define Recipient_email "kishoresrinivasan2002@gmail.com"
SoftwareSerial mySerial (D1,D2);//D2-Rx D1-Tx
SMTPSession smtp;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Your Domain name with URL path or IP address with path
String openWeatherMapApiKey = "6a2b87586e510bcb466121a95edf15e0";
// Replace with your country code and city
String city = "Chennai";
String countryCode = "IN";

// THE DEFAULT TIMER IS SET TO 10 SECONDS FOR TESTING PURPOSES
// For a final application, check the API call limits per hour/minute to avoid getting blocked/banned
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 10 seconds (10000)
unsigned long timerDelay = 10000;

String jsonBuffer;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  Serial.println("");


   //Send HTML message
  //Serial.print("Connected to WiFi network with IP Address: ");
  //Serial.println(WiFi.localIP());
 
  //Serial.println("Timer set to 10 seconds (timerDelay variable), it will take 10 seconds before publishing the first reading.");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  // Send an HTTP GET request
  // millis() -> Returns the number of milliseconds since the Arduino board began running the current program.
  if ((millis() - lastTime) > timerDelay) {
    // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;

      //c_str() -> Returns a pointer to an array that contains a null-terminated sequence of characters 
      jsonBuffer = httpGETRequest(serverPath.c_str());
      //Serial.println(jsonBuffer);
      //The JSON.parse() method parses a string and returns a JavaScript object.
      JSONVar myObject = JSON.parse(jsonBuffer);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
      //Serial.print("JSON object = ");
      //Serial.println(myObject);
      //String ani = "Animal Tresspass!";
      //readStringUntil() reads characters from a stream into a String, The function terminates if the terminator character is detected or it times out (see setTimeout()).
      String msg = mySerial.readStringUntil('\r');
      Serial.print(msg);
      Serial.print("\nTemperature: ");
      Serial.println(myObject["main"]["temp"]);
      int temp = int(myObject["main"]["temp"]); 
       /* Declare the message class */
      Serial.print("Pressure: ");
      Serial.println(myObject["main"]["pressure"]);
      int pres = int(myObject["main"]["pressure"]); 
      Serial.print("Humidity: ");
      Serial.println(myObject["main"]["humidity"]);
      int humi = int(myObject["main"]["humidity"]);
      Serial.print("Wind Speed: ");
      Serial.println(myObject["wind"]["speed"]);
      int wisp = int(myObject["wind"]["speed"]);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      if(msg.indexOf("An") >= 0)
      {
        smtp.debug(1);
         ESP_Mail_Session session;


         session.server.host_name = SMTP_server ;
         session.server.port = SMTP_Port;
         session.login.email = sender_email;
         session.login.password = sender_password;
         session.login.user_domain = "";
      
         /* Declare the message class */
         SMTP_Message message;
      
      
         message.sender.name = "ESP8266";
         message.sender.email = sender_email;
         message.subject = "ESP8266 Testing Email";
         message.addRecipient("AgroSmart",Recipient_email);
      
         //Send HTML message
         String htmlMsg = "Animal Trespass Alert!";
         message.html.content = htmlMsg.c_str();
         message.text.charSet = "us-ascii";
         //7bit simply means "My data consists only of US-ASCII characters, which only use the lower 7 bits for each character."
         message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit; 
      
      
        if (!smtp.connect(&session))
          return;
      
      
        if (!MailClient.sendMail(&smtp, &message))
          Serial.println("Error sending Email, " + smtp.errorReason());
      }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      
      if(temp>=400 || (temp>=250 && humi<=90))
      {
        //Use the debug functionality of the class to see what's going on in your connections.
         smtp.debug(1);
         ESP_Mail_Session session;


         session.server.host_name = SMTP_server ;
         session.server.port = SMTP_Port;
         session.login.email = sender_email;
         session.login.password = sender_password;
         session.login.user_domain = "";
      
         /* Declare the message class */
         SMTP_Message message;
      
      
         message.sender.name = "SmartAgro";
         message.sender.email = sender_email;
         message.subject = "Field Situation";
         message.addRecipient("User",Recipient_email);
      
         //Send HTML message
         String htmlMsg = "Temperature Hike! | Value in Degree Celcius: " + String(temp) + "\nHumidity Deficiency! | Value in Percentage: " + String(humi) + "\nPressure: " + String(pres) + "\nWind Speed: " + String(wisp);
         message.html.content = htmlMsg.c_str();
         message.text.charSet = "us-ascii";
         //7bit simply means "My data consists only of US-ASCII characters, which only use the lower 7 bits for each character."
         message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit; 
      
      
        if (!smtp.connect(&session))
          return;
      
      
        if (!MailClient.sendMail(&smtp, &message))
          Serial.println("Error sending Email, " + smtp.errorReason());
      }
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//The httpGETRequest() function makes a request to OpenWeatherMap and it retrieves a string with a JSON object that contains all the information 
//about the weather for your city.
String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    //Serial.print("HTTP Response code: ");
    //Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
