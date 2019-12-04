#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

ESP8266WebServer server(80);
DHT dht(0, DHT22);

void setup() {
  Serial.begin(115200);
  WiFi.begin("Diniz_Wifi","988905855");

  Serial.println("");
  Serial.print("Conectando");

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
//Minha Rede, Configurar para a rede do LABARC    
//Configurações do IP fixo. Você pode alterar conforme a sua rede
 IPAddress ip(192, 168, 25, 5);
 IPAddress gateway(192, 168, 25, 1);
 IPAddress subnet(255, 255, 255, 0);
 Serial.print("Configurando IP fixo para : ");
 Serial.println(ip);

 //Envia a configuração
 WiFi.config(ip, gateway, subnet);

//Mostramos no Monitor Serial o ip com o qual o esp8266 se conectou para ver 
//se está de acordo com o que configuramos
 Serial.println("");
 Serial.println("Connectado");
 Serial.print ("IP: ");
 Serial.println(WiFi.localIP());

//quando houver um get no http://SEUIP/temperature ,vai fazer get
 server.on("/temperature", HTTP_GET, getTemperature);

//quando houver um get no http://SEUIP/humidity ,vai fazer get
 server.on("/humidity",HTTP_GET,getHumidity);

 //quando houver um get no http://SEUIP/monitor ,vai fazer um get e mostrar o monitor
 server.on("/monitor",HTTP_GET,showMonitor);
 
 server.onNotFound(onNotFound);
 //Inicializamos o server que criamos na porta 80
  server.begin();
  Serial.println("Servidor HTTP iniciado"); 
}


void loop() {
  server.handleClient();
}
//retorna erro caso insira um endereço invalido
void onNotFound() {
  server.send(404,"text/plain","Not Found");
}

//pega a temperatura do sensor e envia o json com o resultado.
void getTemperature() {
  float t = dht.readTemperature();

  String json = "{\"temperature\":"+String(t)+"}";
  server.send(200,"application/json",json);
}
//pega a humidade do sensor e envia o json com o resultado
void getHumidity() {
  float h = dht.readTemperature();

  String json = "{\"humidity\":"+String(h)+"}";
  server.send(200,"application/json",json);
}
//cria o html e o envia.
void showMonitor()
{
  String html =
  "<html>"
  "<head>"
    "<meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no'/>"
    "<title>DHT Monitor</title>"
    "<style type='text/css'>"
      "body{"
            "padding: 35px;"
            "background-color: #222222;"
      "}"
      "h1{"
        "color: #FFFFFF;"
        "font-family: sans-serif;"
      "}"
      "p{"
        "color: #EEEEEE;"
        "font-family: sans-serif;"
            "font-size:18px;"
      "}"
   "</style>"
  "</head>"
  "<body>"
   "<h1>DHT Monitor</h1>"
   "<p id='temperature'>Temperature: </p>"
   "<p id='humidity'>Humidity: </p>"
 "</body>"
 "<script type='text/javascript'>"
   "refresh();" 
   "setInterval(refresh, 5000);"
   "function refresh()"
   "{"
     "refreshTemperature()"
     "refreshHumidity();"
   "}"
   "function refreshTemperature()"
   "{"
     "var xmlhttp = new XMLHttpRequest();"
     "xmlhttp.onreadystatechange = function() {"
       "if (xmlhttp.readyState == XMLHttpRequest.DONE && xmlhttp.status == 200){"
         "document.getElementById('temperature').innerHTML = 'Temperature: ' + JSON.parse(xmlhttp.responseText).temperature + 'C';"
       "}"
     "};"
     "xmlhttp.open('GET', 'http://192.168.2.8/temperature', true);"
     "xmlhttp.send();"
   "}"
   "function refreshHumidity()"
    "{"
      "var xmlhttp = new XMLHttpRequest();"
      "xmlhttp.onreadystatechange = function() {"
        "if (xmlhttp.readyState == XMLHttpRequest.DONE && xmlhttp.status == 200){"
          "document.getElementById('humidity').innerHTML = 'Humidity: ' + JSON.parse(xmlhttp.responseText).humidity + '%';"
        "}"
      "};"
      "xmlhttp.open('GET', 'http://192.168.2.8/humidity', true);"
      "xmlhttp.send();"
    "}"
  "</script>"
  "</html>";
  server.send(200, "text/html", html);
}
