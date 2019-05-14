#include <WiFi.h>

//Needed to log into the Wifi
const char* ssid = ""; //fill in SSID here
const char* password = ""; //fill in password here

WiFiServer server(80); //80 is the default HTTP port

String request; //stores HTTP request

String output39State = "off";
const int output39 = 39;

int moistlvl = analogRead(A0); //Reads the soil sensor

void setup() {
  Serial.begin(115200); 
  pinMode(output39, OUTPUT);
  digitalWrite(output39, LOW);

  // Connection to Wifi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();  
  if (client) {
    String currentLine = "";                
    while (client.connected()) {           
      if (client.available()) {           
        char c = client.read();    
        Serial.write(c);     
        request += c;
        if (c == '\n') {                   
          if (currentLine.length() == 0) {
            
            // turns the GPIOs on and off
            if (request.indexOf("GET /39/on") >= 0) {
              Serial.println("GPIO 39 on");
              output39State = "on";
              digitalWrite(output39, HIGH);
            }
            
            // HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: monospace; display: inline-block; margin: 0px auto; text-align: center; top:50%; left:50%;}");
            client.println(".button { background-color: #86dfe8; border: none; color: white; padding: 16px 20px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Body
            client.println("<body><h1>Spider Plant</h1>");
                  
            if (moistlvl >= 2900){
              digitalWrite();
              client.println("<p>Your plant needs watering!</p>");
              client.println("<p>You should water it</p>");
              client.println("<p><button class=\"button\">Water</button></a></p>");
            } else {
              client.println("<p>Your plant is fine!</p>");
            }
            client.println("</body></html>");
            
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') { 
          currentLine += c;  
        }
      }
    }
    request = "";
    client.stop();
  }
}
