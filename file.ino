#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
const char *ssid = "System";
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
font-family: Consolas, monaco, monospace;
background-color: #1f1f1f;
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
background-color: #2b2b2b;
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
background-color: #3a3a3a;
color: #ffffff
}
.prompt button:hover {
background-color: #252728;
}
.prompt button:active {
background-color: #1f1f1f;
}
</style>
<body>
<div class="prompt">
<button onclick="outerpage()">
Required ethernet or mobile data
</button>
<p id="freeheap">
Free Heap: ...
</p>
<p id="maxblock">
Max Free Block Size: ...
</p>
<p id="frag">
Heap Fragmentation: ...
</p>
</div>
</body>
<script>
function outerpage() {
window.open("https://www.facebook.com")
}
function fetchmemory() {
fetch('/memory')
.then(response => response.json())
.then(data => {
document.getElementById("freeheap").innerText = "Free Heap: " + data.freeheap + " byte";
document.getElementById("maxblock").innerText = "Max Free Block Size: " + data.maxblock + " bytes"
document.getElementById("frag").innerText = "Heap Fragmentation: " + data.frag + " %"
})
}
setInterval(fetchMemory, 1000)
fetchMemory()
</script>
</head>
</html>
)rawliteral";
void handleRoot() {
server.send(200, "text/html", html);
}
void handleMemory() {
String json = "{";
json += "\"freeheap\":" + String(ESP.getFreeHeap()) + ",";
json += "\"maxblock\":" + String(ESP.getMaxFreeBlockSize()) + ",";
json += "\"frag\":" + String(ESP.getHeapFragmentation());
json += "}";
server.send(200, "application/json", json);
}
void setup() {
Serial.begin(115200);
WiFi.softAP(ssid, password);
Serial.println("Access Point Started");
Serial.print("IP address: ");
Serial.println(WiFi.softAPIP().toString());
server.on("/", handleRoot);
server.on("/memory", handleMemory);
server.begin();
Serial.println("Web server started");
}
void loop() {
digitalWrite(LED_BUILTIN, LOW);
delay(125);
digitalWrite(LED_BUILTIN, HIGH);
delay(125);
server.handleClient();
}
