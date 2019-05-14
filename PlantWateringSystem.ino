#include <WiFi.h>

//const ints are just ints that don't change
//Needed to log into the Wifi
const char* ssid = ""; //fill in SSID here
const char* password = ""; //

WiFiServer server(80); //80 is the default HTTP port

String request; //stores HTTP request

String output39State = "off";
const int output39 = 39;

int moistlvl = analogRead(A0); //Reads the soil sensor

void setup() {
  Serial.begin(115200); //opens serial monitor port
  // Initialize the output variables as outputs
  pinMode(output39, OUTPUT);
  // Set outputs to LOW
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
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        request += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
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
    request = "";
    client.stop();
  }
}
