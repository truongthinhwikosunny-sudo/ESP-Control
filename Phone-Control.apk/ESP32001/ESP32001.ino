#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP32_Phone_Control";
const char* password = "12345678";

WebServer server(80);

String getHTML() {
  String html = "";
  html += "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>ESP32 Control</title>";
  html += "<style>";
  html += "body { font-family: Arial; text-align: center; background: #f4f4f4; padding-top: 50px; }";
  html += ".btn { padding: 15px 30px; font-size: 18px; color: white; border: none; border-radius: 5px; margin: 10px; }";
  html += ".on { background: #4CAF50; }";
  html += ".off { background: #f44336; }";
  html += ".card { background: white; padding: 20px; border-radius: 10px; display: inline-block; box-shadow: 0 4px 8px rgba(0,0,0,0.1); }";
  html += "</style>";
  html += "<script>";
  html += "function sendCmd(pin, state){";
  html += "fetch('/control?pin=' + pin + '&state=' + state)";
  html += ".then(res => res.text())";
  html += ".then(data => console.log(data));";
  html += "}";
  html += "</script>";

  html += "</head><body>";

  html += "<h1>ESP32 Control Panel</h1>";
  html += "<div class='card'>";

  html += "<h3>GPIO 23</h3>";
  html += "<button class='btn on' onclick=\"sendCmd(23,'on')\">ON</button>";
  html += "<button class='btn off' onclick=\"sendCmd(23,'off')\">OFF</button>";

  html += "<hr>";

  html += "<h3>GPIO 18</h3>";
  html += "<button class='btn on' onclick=\"sendCmd(18,'on')\">ON</button>";
  html += "<button class='btn off' onclick=\"sendCmd(18,'off')\">OFF</button>";
  html += "</div></body></html>";

  return html;
}

void handleRoot() {
  server.send(200, "text/html", getHTML());
}

void handleControl() {

  if (server.hasArg("pin") && server.hasArg("state")) {

    int pinNum = server.arg("pin").toInt();
    String state = server.arg("state");

    pinMode(pinNum, OUTPUT);
    digitalWrite(pinNum, (state == "on") ? HIGH : LOW);

     server.send(200, "text/plain", "OK");
  }
  else {
    server.send(400, "text/plain", "Sai lenh");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // IP
  IPAddress local_IP(192,168,10,1);
  IPAddress gateway(192,168,10,1);
  IPAddress subnet(255,255,255,0);

  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ssid, password);

  Serial.println("WiFi Started!");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/control", handleControl);

  server.begin();
}

void loop() {
  server.handleClient();
}
