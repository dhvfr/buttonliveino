#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
const char *ssid = "Remote GPIO 2";
const char *password = "12345678";
const int outputPin = D4;
ESP8266WebServer server(80);
const char systempage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta http-equiv="X-UA-Compatible" content="ie=edge">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>
System
</title>
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
font-family: Consolas, monaco, monospace;
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
color: #32cd32;
}
.prompt button {
padding: 8px;
width: 100%;
outline: none;
border: none;
font-family: Consolas, monaco, monospace;
font-size: 14px;
font-weight: 500;
margin: 10px 0;
background-color: #3b3d3e;
color: #32cd32;
}
.prompt button:hover {
background-color: #1c1c1d;
}
.prompt button:active {
background-color: #000000;
}
</style>
<body>
<div class="prompt">
<button id="outputbutton" ontouchstart="toggleLED('on')" ontouchend="toggleLED('off')" onmousedown="toggleLED('on')" onmouseup="toggleLED('off')">
Signal LED light and output GPIO2 (D4)
</button>
<form method="POST" action="/off">
<button type="submit">
Shut down system
</button>
</form>
<button onclick="location.reload();">
Update memory status now
</button>
<p>
Free heap: %FREE_HEAP% bytes
</p>
<p>
Heap fragmentation: %HEAP_FRAG%%
</p>
<p>
Max free block size: %MAX_BLOCK% bytes
</p>
</div>
</body>
<script>
function toggleLED(state) {
fetch("/led?state=" + state);
}
</script>
</head>
</html>
)rawliteral";
String processor(const String& var) {
if (var == "FREE_HEAP") return String(ESP.getFreeHeap());
if (var == "HEAP_FRAG") return String(ESP.getHeapFragmentation());
if (var == "MAX_BLOCK") return String(ESP.getMaxFreeBlockSize());
return String();
}
void handleRoot() {
String html = systempage;
html.replace("%FREE_HEAP%", processor("FREE_HEAP"));
html.replace("%HEAP_FRAG%", processor("HEAP_FRAG"));
html.replace("%MAX_BLOCK%", processor("MAX_BLOCK"));
server.send(200, "text/html", html);
}
void handleLED() {
String state = server.arg("state");
if (state == "on") {
digitalWrite(outputPin, LOW);
}
else {
digitalWrite(outputPin, HIGH);
}
server.send(200, "text/html", "OK");
}
void handleOff() {
server.send(200, "text/plain", "System is shuting down...");
delay(1000);
ESP.deepSleep(0);
}
void setup() {
Serial.begin(115200);
pinMode(outputPin, OUTPUT);
digitalWrite(outputPin, HIGH);
WiFi.softAP(ssid, password);
while (WiFi.status() != WL_CONNECTED) delay(500);
Serial.println("Access Point Started");
Serial.print("IP address: ");
Serial.println(WiFi.softAPIP());
server.on("/", handleRoot);
server.on("/led", handleLED);
server.on("/off", handleOff);
server.begin();
Serial.println("Web server started");
}
void loop() {
server.handleClient();
}
