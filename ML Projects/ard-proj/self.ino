#include <ESP8266WiFi.h>

const char* ssid = "IoT";
const char* password = "password";

WiFiServer server(80);

int sensorPin = 9;

int led1 = 2;
int led2 = 16;
boolean led1Status = false;
boolean led2Status = false;

int sensorStatus = 0;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(sensorPin, INPUT);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);

  Serial.begin(9600);

  WiFi.softAP(ssid, password);

  server.begin();
  Serial.println("Access Point started");
}

void loop() {
  if(digitalRead(sensorPin)==1){
    sensorStatus = 1;
    Serial.println("Detected Movement");
    delay(250);
  }
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<html>");
            client.println("<head><title>IoT Buglary System</title>");
            client.println("<style>button {font-size: 2em;text-align: center; margin: 0.5em; padding: 1em 2em; border-radius: 0.5em; color: white; background-color: #4CAF50; border: none;}h1,p{text-align: center;}");
            client.println("</style></head>");
            client.println("<body>");
            client.println("<h1><u><b>IoT Based Buglary Detection System</b></u></h1>");
            client.println("<p>Toggle the Lights using the given buttons.<p/>");
            client.println("<p>LED 1: <a href=\"/LED1_ON\"><button>OFF</button></a> <a href=\"/LED1_OFF\"><button>ON</button></a></p>");
            client.println("<p>LED 2: <a href=\"/LED2_ON\"><button>OFF</button></a> <a href=\"/LED2_OFF\"><button>ON</button></a></p>");
            client.println("<H1><u>Buglary Detection</u></H1>");
            client.println("<p>Alert indicates presence of some movement in the given perimeter.</p>");
            client.println("<p><a href=\"/RESET\"><button>RESET</button></a></p>");
            client.println("<p><a href=\"/SET\"><button>SET</button></a></p>");
            if(sensorStatus==0){
              client.println("<h1>Status : No Alert</h1>");
            } else {
              client.println("<h1>Status : Alert</h1>");
            }
            client.println("</body>");
            client.println("</html>");
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        if (currentLine.endsWith("GET /LED1_ON")) {
          digitalWrite(led1, HIGH);
          led1Status = true;
        } else if (currentLine.endsWith("GET /LED1_OFF")) {
          digitalWrite(led1, LOW);
          led1Status = false;
        } else if (currentLine.endsWith("GET /LED2_ON")) {
          digitalWrite(led2, HIGH);
          led2Status = true;
        } else if (currentLine.endsWith("GET /LED2_OFF")) {
          digitalWrite(led2, LOW);
          led2Status = false;
        } else if (currentLine.endsWith("GET /RESET")) {
          sensorStatus = 0;
        } else if (currentLine.endsWith("GET /SET")) {
          sensorStatus = 1;
        }
      }
    }

    client.stop();
    Serial.println("Client disconnected");
  }
}
