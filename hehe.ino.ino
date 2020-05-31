#include "MQ135.h"
//#include<ESP8266WiFi.h>
#define ANALOGPIN A0    //  Define Analog PIN on Arduino Board
MQ135 gasSensor = MQ135(ANALOGPIN);
#define pin1 D1
#define pin2 D2
#define pin3 D3
const char* ssid="Ser Fuyah";
const char* password="123456789o";
WiFiServer=Server(80);
void setup()
{
Serial.print("Connecting to ");
  Serial.println(ssid);
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
  server.begin();

  float rzero = gasSensor.getRZero();
  delay(3000);
  Serial.print("MQ135 RZERO Calibration Value : ");
  Serial.println(rzero);
  pinMode(pin1,OUTPUT);
  pinMode(pin2,OUTPUT);
  pinMode(pin3,OUTPUT);
}

void loop() {
  WiFiStrength = WiFi.RSSI();
  String status = "";  
  //String request=clientt.readStringUntil('\r');
  //clientt.flush();
  float ppm = gasSensor.getPPM();
  delay(1000);
 if(ppm < 600)
 {
    digitalWrite(pin1,HIGH);
    digitalWrite(pin2,LOW);
    digitalWrite(pin3,LOW);
    status = "good";
 }
 
 else if(ppm >=600 && ppm <800)
 {
    digitalWrite(pin2,HIGH);
    digitalWrite(pin1,LOW);
    digitalWrite(pin3,LOW);
    status = "average";
 }

 else
 {
    digitalWrite(pin3,HIGH);
    digitalWrite(pin2,LOW);
    digitalWrite(pin1,LOW);
    status = "critical";
 }
 
  Serial.print("CO2 ppm value : ");
  Serial.println(ppm);
  Serial.print("STATUS: ");
  serial.println(status);

WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");

  client.println("<html>");
  client.println(" <head>");
  client.println("<title>APMS</tiltle>");
  client.println("<style>body{background-image: url(\"bgimg.jpg\"); background-position: center; background-repeat: no-repeat; background-size: cover;}</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<div style=\"padding:50px; display:inline-block;\">");
  client.println("<h1 style=\"padding:15px;text-align:center; font-size:370%; background: rgba(0, 0, 0, 0.3); color:White; border-radius:10px;\">AIR POLLUTION MONITORING SYSTEM</h1>");
  client.println("<br><br><br><br><br>");
  client.println("<div style=\"font-size:150%;display:inline\">PPM of Air: </div>");
  client.println("<div style=\"display:inline;font-size:150%;\"><div id=\"ppm\" style=\"display:inline;\"></div> ppm</div><br><br><br>");
  client.println("<div style=\"font-size:150%;display:inline\">Air Quality: </div>");
  client.println("<b><div id=\"status\" style=\"display:inline;font-size:150%;\"></div></b>");
  client.println("</div>");
  client.print("<script> window.setInterval(function(){ document.getElementById('ppm').innerHTML = ");
  client.print(ppm);
  client.println(";}, 1000);");
  client.print("<script> window.setInterval(function(){ document.getElementById('status').innerHTML = ");
  client.print(status);
  client.println(";}, 1000);</script>");
  client.print("WiFi Signal Strength: ");
  client.println(WiFiStrength);
  client.print("CO2 PPM is: ");
  client.println(ppm);
  client.println(status);
  client.println("</body>");
  client.println("</html>");
}
