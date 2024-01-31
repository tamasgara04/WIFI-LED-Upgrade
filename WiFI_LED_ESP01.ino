#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const bool serial = false;
const char* ssid = "WiFI LED";
const char* password = "12345678";

ESP8266WebServer server(80);

int Rbri,Gbri,Bbri,Totalbri,Robri,Gobri,Bobri;
int Rled = 1;
int Gled = 2;
int Bled = 3;

void setup() {  
  if(serial){
    Serial.begin(115200);
  }else{
  pinMode(1, FUNCTION_3); 
  pinMode(3, FUNCTION_3);
  }
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.print("Access Point created with SSID: ");
  Serial.println(ssid);
  
  server.on("/", handleRoot);
  server.on("/network", handleNetwork);
  server.on("/connect", handleConnect);
  server.on("/settings", handleSettings);
  
  server.begin();
}

void loop() {
  server.handleClient();
  int rpwm = map(Robri,0,100,0,1024);
  int gpwm = map(Gobri,0,100,0,1024);
  int bpwm = map(Bobri,0,100,0,1024);
  if(!serial){
    analogWrite(Rled,rpwm);
    analogWrite(Gled,gpwm);
    analogWrite(Bled,bpwm);
  }
  /*
  Serial.print("R led pwm- ");Serial.println(rpwm);
  Serial.print("G led pwm- ");Serial.println(gpwm);
  Serial.print("B led pwm- ");Serial.println(bpwm);
  */
}

void handleNetwork() {
  String response = "";
  response += "<html><head><link rel='stylesheet' href='https://cdn.jsdelivr.net/npm/water.css@2/out/water.css'></head><body>";
  response += "<h1>";
  response += WiFi.SSID();
  response += "</h1></body></html>";
  response += "<form method='post' action='/connect'>";
  response += "<select name='ssid'>";
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; ++i) {
    response += "<option value='";
    response += WiFi.SSID(i);
    response += "'>";
    response += WiFi.SSID(i);
    response += "</option>";
  }
  response += "</select><br>";
  response += "<input type='password' name='password' placeholder='Password'><br>";
  response += "<input type='submit' value='Connect'>";
  response += "</form>";
  response += "<a href='/'>Home</a>";
  response += "</body></html>";
  server.send(200, "text/html", response);
}

bool connectToNet(String ssid, String password){
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  int count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    count++;
    if(count >= 30){
      Serial.println("");
      Serial.println("Timeout");
      Serial.println("Failed to Connect to:");
      Serial.println(ssid);
      return false;
      break;
      }
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}

void handleConnect() {
  String lastSSID = WiFi.SSID();
  String lastPWD = WiFi.psk();
  String ssid = server.arg("ssid");
  String password = server.arg("password");
  
  server.sendHeader("Location", "/network");
  server.send(301, "text/plain", "");
  
  bool net_status = connectToNet(ssid, password);
  
  if(net_status){
    WiFi.mode(WIFI_STA);
  }else{
    net_status = connectToNet(lastSSID, lastPWD);
    if(net_status){
      String response = "<html<head><link rel='stylesheet' href='https://cdn.jsdelivr.net/npm/water.css@2/out/water.css'></head>><body><h1>Failed to connect to network:";
      response += ssid;
      response += "</h1></body></html>";
      server.send(200, "text/html", response);
      WiFi.mode(WIFI_STA);
    }else{
      WiFi.mode(WIFI_AP);
    }
  }
}

void handleRoot() {
     char temp[1500];
     snprintf ( temp, 5000,"<html>\
    <head>\
    <link rel='stylesheet' href='https://cdn.jsdelivr.net/npm/water.css@2/out/water.css'>\
      <title>Tamas Leds</title>\
    </head>\
    <body>\
      <h1>WiFI LEDs Tamas</h1>\
      <form action='/' method='POST'>\
      Red: <input type='number' name='Red' value='%02d'>\<br>\
      Green: <input type='number' name='Green' value='%02d'>\<br>\
      Blue: <input type='number' name='Blue' value='%02d'>\<br>\
      Brightness: <input type='number' name='Brightness' value='%02d'>\<br>\
      <input type='submit' value='Submit'>\<br>\  
    </form>\
    <form action='/' method='POST'>\
      <input type='hidden' name='Red' value='100'>\
      <input type='hidden' name='Green' value='100'>\
      <input type='hidden' name='Blue' value='100'>\
      <input type='hidden' name='Brightness' value='100'>\
      <input type='submit' value='ON'>\
    </form>\
    <form action='/' method='POST'>\
      <input type='hidden' name='Red' value='1'>\
      <input type='hidden' name='Green' value='1'>\
      <input type='hidden' name='Blue' value='1'>\
      <input type='hidden' name='Brightness' value='1'>\
      <input type='submit' value='OFF'>\
    </form>\
    <input type='submit' onclick='location.reload()' value='Reload'>\ Press this after submiting settings!\<br>\
    <a href='/settings'>Settings</a>\ <br>\
    <a href='/network'>Network Settings</a>\
    </body>\
  </html>",
      Rbri,Gbri,Bbri,Totalbri
     );
  server.send(200, "text/html", temp);
  
  //Sending Webpage submitted value to calculation.
  Get_String(String(server.arg("Red")),String(server.arg("Green")),String(server.arg("Blue")),String(server.arg("Brightness")));
}

void Get_String(String R,String G,String B,String Bri){
  int a,b,c,d;
  a = R.toInt();
  b = G.toInt();
  c = B.toInt();
  d = Bri.toInt();
  Serial.print(a);Serial.print(",");
  Serial.print(b);Serial.print(",");
  Serial.print(c);Serial.print(",");
  Serial.println(d);
  if(d>0) Totalbri = constrain(d,0,100);
  if(a>0) Rbri = constrain(a,0,100); 
  if(b>0) Gbri = constrain(b,0,100);
  if(c>0) Bbri = constrain(c,0,100); 
  Serial.print("R:- ");
  Serial.println(Rbri);
  Serial.print("G:- ");
  Serial.println(Gbri);
  Serial.print("B:- ");
  Serial.println(Bbri);
  Serial.print("Brightness:- ");
  Serial.println(Totalbri);
  Robri = Rbri*Totalbri/100;
  Gobri = Gbri*Totalbri/100;
  Bobri = Bbri*Totalbri/100;
  Serial.print("Ro:- ");
  Serial.println(Robri);
  Serial.print("Go:- ");
  Serial.println(Gobri);
  Serial.print("Bo:- ");
  Serial.println(Bobri);
  Serial.print("Brightness:- ");
  Serial.println(Totalbri);  
}

void handleSettings(){
  char response[2000];
  sprintf(response, "<html>\
    <head>\
      <link rel='stylesheet' href='https://cdn.jsdelivr.net/npm/water.css@2/out/water.css'>\
      <title>Tamas Leds</title>\
    </head>\
    <body>\
      <h1>Pin Settings</h1>\
      <form action='/settings' method='post'>\
        Red LED Pin: <input type='number' name='RedLED' value='%d'>\ <br>\
        Green LED Pin: <input type='number' name='GreenLED' value='%d'>\ <br>\
        Blue LED Pin: <input type='number' name='BlueLED' value='%d'>\ <br>\
        <input type='submit' onclick='location.reload()' value='Submit'>\
      </form>\
      <input type='submit' onclick='location.reload()' value='Reload'>\ Press this after submiting settings!\<br>\
      <a href='/'>Done</a>\
      </body></html>", Rled, Gled, Bled);
  server.send(200, "text/html", response);
  if(atoi(server.arg("RedLED").c_str()) != 0 && atoi(server.arg("GreenLED").c_str()) != 0 && atoi(server.arg("BlueLED").c_str()) != 0){
    Rled = atoi(server.arg("RedLED").c_str());
    Gled = atoi(server.arg("GreenLED").c_str());
    Bled = atoi(server.arg("BlueLED").c_str());
  }
  Serial.print("Red LED PIN:"); Serial.println(Rled);
  Serial.print("Green LED PIN:"); Serial.println(Gled);
  Serial.print("Blue LED PIN:"); Serial.println(Bled);
  if(!serial){
    pinMode(Rled,OUTPUT);
    pinMode(Gled,OUTPUT);
    pinMode(Bled,OUTPUT);
  }
}
