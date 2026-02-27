#include <Arduino.h>

#include <ETH.h> // quote to use ETH
#include <WiFi.h>

#include <ElegantOTA.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>
AsyncWebServer server(80);
unsigned long ota_progress_millis = 0;


#define ETH_ADDR        1
#define ETH_POWER_PIN   16 // Do not use it, it can cause conflict during the software reset.
#define ETH_POWER_PIN_ALTERNATIVE 17
#define ETH_MDC_PIN    23
#define ETH_MDIO_PIN   18
#define ETH_TYPE       ETH_PHY_LAN8720
#define ETH_CLK_MODE   ETH_CLOCK_GPIO0_IN
// #define ETH_CLK_MODE   ETH_CLOCK_GPIO17_OUT

#define REL1 17
#define REL2 2
#define REL3 33
#define REL4 32
#define MIXIN 4

int N = 0;  //Schleifenzaehler

WiFiServer serverETH; // Declare server objects

IPAddress local_ip(10,64,94,55);
IPAddress gateway(10,64,94,1);
IPAddress subnet(255,255,255,0);
IPAddress dns1(8,8,8,8);
IPAddress dns2 = (uint32_t)0x00000000;

static bool eth_connected = false;
bool MIXINalt = true;

// WEBSOCKETS start
AsyncWebSocket ws("/ws");
bool ledState1 = 0;
bool ledState2 = 0;
bool ledState3 = 0;
bool ledState4 = 0;

String HTML;
String HTMLb="blue";
String HTMLd="blue";
String HTMLf="blue";
String HTMLh="blue";
String HTMLj="blue";

String HTMLa PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Brainstorm TCP Tally</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }
  h1 {
    font-size: 1.8rem;
    color: white;
  }
  h2{
    font-size: 1.5rem;
    font-weight: bold;
    color: #143642;
  }
  .topnav {
    overflow: hidden;
    background-color: #444444ff;
  }
  body {
    margin: 0;
  }
  .content {
    padding: 30px;
    max-width: 600px;
    margin: 0 auto;
  }
  .card {
    background-color: #F8F7F9;;
    box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
    padding-top:10px;
    padding-bottom:20px;
  }
  .button {
    padding: 15px 50px;
    font-size: 20px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #2b2a2aff;
    border: none;
    border-radius: 5px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0,0,0,0);
   }
   /*.button:hover {background-color: #8d0f0fff}*/
   .button:active {
     background-color: #0f8b8d;
     box-shadow: 2 2px #CDCDCD;
     transform: translateY(2px);
   }
   .state {
     font-size: 1.5rem;
     color:#8c8c8c;
     font-weight: bold;
   }
  </style>
<title>Brainstorm TCP Tally</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
</head>
<body>
  <div class="topnav">
    <h1>Brainstorm Tally 5.4.1</h1>
  </div>
  <div class="content">
    <div class="card">
      <h2>Input - Mixer</h2>
      <div style="background-color: )rawliteral";

// HTMLb ist Mixer OUT
      
String HTMLc= R"rawliteral(
      ; width: 150px; height: 80px; margin: auto;"></div>
      <!-- <p><button id="button" class="button">Toggle</button></p> -->
      <p> <br> </p>
      <h2>Output - Tally 1</h2>
      <div style="background-color: )rawliteral";

// HTMLd Ist Tally 1

String HTMLe= R"rawliteral(
      ; width: 150px; height: 80px; margin: auto;"></div>
      <!-- <p><button id="button" class="button">Toggle</button></p> -->
      <h2>Output - Tally 2</h2>
      <div style="background-color: )rawliteral";

// HTMLf ist Tally 2

String HTMLg = R"rawliteral(
      ; width: 150px; height: 80px; margin: auto;"></div>
      <!-- <p><button id="button" class="button">Toggle</button></p> -->
      <h2>Output - Tally 3</h2>
      <div style="background-color: )rawliteral";

// HTMLh ist Tally 3
      
String HTMLi = R"rawliteral(
      ; width: 150px; height: 80px; margin: auto;"></div>
      <!-- <p><button id="button3" class="button">Toggle</button></p> -->
      <h2>Output - Tally 4</h2>
      <div style="background-color: )rawliteral";

// HTMLj ist Tally 4

String HTMLk = R"rawliteral(
      ; width: 150px; height: 80px; margin: auto;"></div>
      <p> <br> </p>
      <!-- <p><button id="button" class="button">Toggle</button></p> -->
      <a href="/update" class="button">UPDATE</a>
      <a href="/webserial" class="button">WEBSERIAL</a>
    </div>
  </div>
<script>
  function refresh(refreshPeriod)
    {setTimeout('location.reload(true)', refreshPeriod);}
  window.onload = refresh(4000);
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
    var state;
    if (event.data == "1"){
      state = "ON";
    }
    else{
      state = "OFF";
    }
    state = event.data;
    document.getElementById('state').innerHTML = state;
    document.getElementById('state3').innerHTML = state;
  }
  function onLoad(event) {
    initWebSocket();
    initButton();
  }
  function initButton() {
    document.getElementById('button').addEventListener('click', toggle);
  }
  function toggle(){
    websocket.send('toggle');
  }
</script>
</body>
</html>)rawliteral";

void notifyClients() {
  HTML = HTMLa + HTMLb + HTMLc + HTMLd + HTMLe + HTMLf + HTMLg + HTMLh + HTMLi + HTMLj + HTMLk;
  //ws.textAll(HTML);
  }

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "toggle") == 0) {
      ledState1 = !ledState1;
      notifyClients();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      WebSerial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      WebSerial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
  Serial.println(var);
  WebSerial.println(var);
  if(var == "STATE"){
    if (ledState1){
      return "ON";
    }
    else{
      return "OFF";
    }
  }
  if(var == "STATE3"){
    if (ledState3){
      return "ON";
    }
    else{
      return "OFF";
    }
  }
  return String();
}
//WEBSOCKETS Ende

void onOTAStart() {
  // Log when OTA has started
  Serial.println("OTA update started!");
  // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final) {
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
    Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
  }
}

void onOTAEnd(bool success) {
  // Log when OTA has finished
  if (success) {
    Serial.println("OTA update finished successfully!");
  } else {
    Serial.println("There was an error during OTA update!");
  }
  // <Add your own code here>
}

void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
  //if (d == "ON"){
  //  digitalWrite(LED, HIGH);
  //}
  //if (d=="OFF"){
  //  digitalWrite(LED, LOW);
  //}
}


void WiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      Serial.println("ETH Started");
      //set eth hostname here
      ETH.setHostname("esp32-ethernet");
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      Serial.print("ETH MAC: ");
      Serial.print(ETH.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(ETH.localIP());
      if (ETH.fullDuplex()) {
        Serial.print(", FULL_DUPLEX");
      }
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");
      eth_connected = true;
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    default:
      break;
  }
}

void setup() {
  MIXINalt = false;
  pinMode(ETH_POWER_PIN_ALTERNATIVE, OUTPUT);
  pinMode(REL1, OUTPUT);
  digitalWrite(REL1,LOW);
  pinMode(REL2, OUTPUT);
  digitalWrite(REL2,LOW);
  pinMode(REL3, OUTPUT);
  digitalWrite(REL3,LOW);
  pinMode(REL4, OUTPUT);
  digitalWrite(REL4,LOW);
  pinMode(MIXIN, INPUT_PULLUP);
  digitalWrite(ETH_POWER_PIN_ALTERNATIVE, HIGH);
  Serial.begin(115200);
  Serial.println();

  WiFi.onEvent(WiFiEvent);

  ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE, ETH_CLK_MODE); // Enable ETH
  
  ETH.config(local_ip, gateway, subnet, dns1, dns2); // Static IP, leave without this line to get IP via DHCP
    
//  while(!((uint32_t)ETH.localIP())) // Waiting for IP (leave that group of lines to get IP via DHCP)
//  {
//
//  }
  
  Serial.println("Connected");
  Serial.print("IP Address:");
  Serial.println(ETH.localIP());

  serverETH.begin(6000); // Server starts listening on port number 6000

  // Websocket
  initWebSocket();

  // Route for root / web page
  HTML = HTMLa + HTMLb + HTMLc + HTMLd + HTMLe + HTMLf + HTMLg + HTMLh + HTMLi + HTMLj + HTMLk;
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", HTML, processor);
  });

  //server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
  //  request->send(200, "text/plain", "Brainstorm Tally 3.2.0 - use /update for update! - use /webserial for Webserial!");
  //});

  ElegantOTA.begin(&server);    // Start ElegantOTA
  ElegantOTA.setAuth("admin", "admin");
  // ElegantOTA callbacks
  ElegantOTA.onStart(onOTAStart);
  ElegantOTA.onProgress(onOTAProgress);
  ElegantOTA.onEnd(onOTAEnd);

  WebSerial.begin(&server);
  WebSerial.onMessage(recvMsg);
  WebSerial.println("Welcome to WebSerial Monitor!");
  WebSerial.println("Send something from your browser!");

  server.begin();
  Serial.println("HTTP server started");
}

void loop() 
{
    ElegantOTA.loop();
    WebSerial.loop();
    ws.cleanupClients();
    //digitalWrite(ledPin, !ledState1);  //from Websocket
    if (digitalRead(MIXIN) == HIGH)
    {
      if (MIXINalt == false)
      {
        HTMLb = "black";
        Serial.println("Mixer Tally off");
        WebSerial.println("Mixer off");
        digitalWrite(REL1,HIGH);
        Serial.println("Relais 1 off");
        HTMLd = "black";
        WebSerial.println("Relais 1 off");
        digitalWrite(REL2,HIGH);
        HTMLf = "black";
        Serial.println("Relais 2 off");
        WebSerial.println("Relais 2 off");
        digitalWrite(REL3,HIGH);
        HTMLh = "black";
        Serial.println("Relais 3 off");
        WebSerial.println("Relais 3 off");
        digitalWrite(REL4,HIGH);
        HTMLj = "black";
        Serial.println("Relais 4 off");
        WebSerial.println("Relais 4 off");
        notifyClients();
        MIXINalt = true;
      }
    }
    else 
    {
      if (MIXINalt == true)
      {
        MIXINalt = false;
        HTMLb = "red";
        Serial.println("Mixer Tally back on");
        WebSerial.println("Mixer Tally back on");
        if (ledState1 == true)
        {
          Serial.println("Relais 1 on");
          WebSerial.println("Relais 1 on");
          HTMLd = "red";
          digitalWrite(REL1,LOW);
        }
        else
        {
          digitalWrite(REL1,HIGH);
          Serial.println("Relais 1 off");
          HTMLd = "black";
          WebSerial.println("Relais 1 off");
        }
        if (ledState2 == true)
        {
          Serial.println("Relais 2 on");
          WebSerial.println("Relais 2 on");
          HTMLf = "red";
          digitalWrite(REL2,LOW);
        }
        else
        {
          digitalWrite(REL2,HIGH);
          HTMLf = "black";
          Serial.println("Relais 2 off");
          WebSerial.println("Relais 2 off");
        }
        if (ledState3 == true)
        {
          Serial.println("Relais 3 on");
          WebSerial.println("Relais 3 on");
          ledState3=true;
          HTMLh = "red";
          digitalWrite(REL3,LOW);
        }
        else
        {
          digitalWrite(REL3,HIGH);
          HTMLh = "black";
          Serial.println("Relais 3 off");
          WebSerial.println("Relais 3 off");
        }
        if (ledState4 == true)
        {
          Serial.println("Relais 4 on");
          WebSerial.println("Relais 4 on");
          HTMLj = "red";
          digitalWrite(REL4,LOW);
        }
        else
        {
          digitalWrite(REL4,HIGH);
          HTMLj = "black";
          Serial.println("Relais 4 off");
          WebSerial.println("Relais 4 off");
        }
      }
      notifyClients();
    }


    WiFiClient client = serverETH.available(); // Try to create a client object
    if (client) // If current customer is available
    {
        Serial.println("[Client connected]");
        WebSerial.println("[Client connected]");
        String readBuff;
        while (client.connected()) // If the client is connected
        {
            if (client.available() > 0) // If there is readable data
            {
                char c = client.read(); // Read a byte
                                        // You can also use readLine() and other methods

                client.write(c); // Send to client
                //Serial.write(c); // Printing from the serial port
                
                readBuff += c;
                N = N+1;
                if(digitalRead(MIXIN) == LOW)
                {
                  HTMLb="red";
                  if (N == 3)
                  {
                   if (c=='1')
                   {
                      Serial.println("Relais 1 on");
                      WebSerial.println("Relais 1 on");
                      HTMLd = "red";
                      ledState1=true;
                      digitalWrite(REL1,LOW);
                    }
                    else 
                    {
                      ledState1=false;
                      digitalWrite(REL1,HIGH);
                      Serial.println("Relais 1 off");
                      HTMLd = "black";
                      WebSerial.println("Relais 1 off");
                    }
                  }

                  if (N == 4)
                  {
                    if (c=='1')
                    {
                      Serial.println("Relais 2 on");
                      WebSerial.println("Relais 2 on");
                      ledState2=true;
                      HTMLf = "red";
                      digitalWrite(REL2,LOW);
                    }
                    else 
                    {
                      ledState2=false;
                      digitalWrite(REL2,HIGH);
                      HTMLf = "black";
                      Serial.println("Relais 2 off");
                      WebSerial.println("Relais 2 off");
                    }
                  }

                  if (N == 5)
                  {
                    if (c=='1')
                    {
                      Serial.println("Relais 3 on");
                      WebSerial.println("Relais 3 on");
                      ledState3=true;
                      HTMLh = "red";
                      digitalWrite(REL3,LOW);
                    }
                    else 
                    {
                      ledState3=false;
                      digitalWrite(REL3,HIGH);
                      HTMLh = "black";
                      Serial.println("Relais 3 off");
                      WebSerial.println("Relais 3 off");
                    }
                  }

                  if (N == 6)
                  {
                    if (c=='1')
                    {
                      Serial.println("Relais 4 on");
                      WebSerial.println("Relais 4 on");
                      HTMLj = "red";
                      ledState4=true;
                      digitalWrite(REL4,LOW);
                    }
                    else 
                    {
                      ledState4=false;
                      digitalWrite(REL4,HIGH);
                      HTMLj = "black";
                      Serial.println("Relais 4 off");
                      WebSerial.println("Relais 4 off");
                    }
                  }
                }
                else
                {
                  HTMLb="black";
                  if (N == 3)
                  {
                    if (c=='1')
                    {
                      ledState1=true;
                    }
                    else 
                    {
                      ledState1=false;
                    }
                  }
                  if (N == 4)
                  {
                    if (c=='1')
                    {
                      ledState2=true;
                    }
                    else 
                    {
                      ledState2=false;
                    }
                  }
                  if (N == 5)
                  {
                    if (c=='1')
                    {
                      ledState3=true;
                    }
                    else 
                    {
                      ledState3=false;
                    }
                  }                  
                  if (N == 6)
                  {
                    if (c=='1')
                    {
                      ledState4=true;
                    }
                    else 
                    {
                      ledState4=false;
                    }
                  }                  
                }
                if(c == ';') // Received carriage return
                {
                    client.print("Received: " + readBuff); // Send to client
                    Serial.println("Received: " + readBuff); // Printing from the serial port
                    WebSerial.println("Received: " + readBuff); // Printing from the serial port
                    readBuff = "";
                    N = 0;
                    break; // Out of the loop
                }
            }
        }
        WebSerial.print("Status: ");
        WebSerial.print(ledState1);
        WebSerial.print(ledState2);
        WebSerial.print(ledState3);
        WebSerial.println(ledState4);
        //WebSerial.println(TStatus);
        notifyClients();
        client.stop(); //End the current connection
        Serial.println("[Client disconnected]");
        WebSerial.println("[Client disconnected]");
        Serial.println("");
        WebSerial.println("");
    }
}