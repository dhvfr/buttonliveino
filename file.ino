#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
const char *ssid = "All live link";
const char *password = "12345678";
ESP8266WebServer server(80);
const char html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta http-equiv="X-UA-Compatible" content="ie=edge">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>System</title>
<style>
*{
margin: 0;
padding: 0;
}
body {
display: flex;
justify-content: center;
align-items: center;
width: 100%;
height: 100vh;
overflow: hidden;
box-sizing: border-box;
background-color: #1c1c1d;
background-repeat: no-repeat;
background-size: cover;
background-position: center;
user-select: none;
-webkit-user-select: none;
-moz-user-select: none;
-khtml-user-select: none;
-ms-user-select: none;
}
.prompt {
position: absolute;
padding: 25px;
width: 400px;
background-color: #252728;
}
.prompt p {
color: #e2e5e9;
}
.prompt button {
padding: 8px;
width: 100%;
outline: none;
border: none;
font-size: 14px;
font-weight: 500;
margin: 10px 0;
background-color: #0078d4;
color: #e2e5e9;
}
.prompt button:hover {
background-color: #106ebe;
}
.prompt button:active {
background-color: #005a9e;
}
</style>
<body>
<div class="prompt">
<button id="outputbutton" ontouchstart="toggleLED('on')" ontouchend="toggleLED('off')" onmousedown="toggleLED('on')" onmouseup="toggleLED('off')">
Signal LED light and output GPIO2 (D4)
</button>
<p>
NOTE: Ethernet or mobile data required!
</p>
<button id="buttonclick" onclick="clickbutton()" disabled>
Activate all the live of Linh Ngo Cosmetics
</button>
</div>
</body>
<script>
function toggleLED(state) {
fetch("/led?state=" + state);
document.getElementById("buttonclick").disabled = false;
}
function clickbutton() {
window.location.href = "https://docs.google.com/document/d/1i_6TjM0vPDR0Hio-dRW2tXt9zV-uKAI9U5d5zJ8q94I";
}
</script>
</head>
</html>
)rawliteral";
void handleRoot() {
server.send_P(200, "text/html", html);
}
void handleLED() {
String state = server.arg("state");
if (state == "on") {
digitalWrite(2, LOW);
}
else {
digitalWrite(2, HIGH);
}
server.send(200, "text/plain", "OK");
}
void setup() {
Serial.begin(115200);
pinMode(2, OUTPUT);
digitalWrite(2, HIGH);
WiFi.softAP(ssid, password);
Serial.println("Access Point Started");
Serial.print("IP address: ");
Serial.println(WiFi.softAPIP());
server.on("/", handleRoot);
server.on("/led", handleLED);
server.begin();
Serial.println("Web server started");
}
void loop() {
server.handleClient();
}
