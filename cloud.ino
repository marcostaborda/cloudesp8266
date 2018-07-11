#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <DNSServer.h> 
#include <WiFiManager.h>  
#include <FS.h>
#include <ArduinoJson.h>
typedef struct { 
  uint8_t code;
  char* value;
} langDictionary;
const langDictionary myDictionaryArr[] {
    {0,"tornado"},
    {1,"tempestade tropical"},
    {2,"furacão"},
    {3,"tempestade severa"},             // severe thunderstorms
    {4,"trovoadas"},                     // thunderstorms
    {5,"chuva e neve"},                  // mixed rain and snow
    {6,"chuva e granizo fino"},          // mixed rain and sleet
    {7,"neve e granizo fino"},           // mixed snow and sleet
    {8,"garoa gélida"},                  // freezing drizzle
    {9,"garoa"},                         // drizzle
    {10,"chuva gélida"},                  // freezing rain
    {11,"chuvisco"},                      // showers
    {12,"chuva"},                         // showers
    {13,"neve em flocos finos"},          // snow flurries
    {14,"leve precipitação de neve"},     // light snow showers
    {15,"ventos com neve"},               // blowing snow
    {16,"neve"},                          // snow
    {17,"chuva de granizo"},              // hail
    {18,"pouco granizo"},                 // sleet
    {19,"pó em suspensão"},               // dust
    {20,"neblina"},                       // foggy
    {21,"névoa seca"},                    // haze
    {22,"enfumaçado"},                    // smoky
    {23,"vendaval"},                      // blustery
    {24,"ventando"},                      // windy
    {25,"frio"},                          // cold
    {26,"nublado"},                       // cloudy
    {27,"muitas nuvens (noite)"},         // mostly cloudy (night)
    {28,"muitas nuvens (dia)"},           // mostly cloudy (day)
    {29,"parcialmente nublado (noite)"},  // partly cloudy (night)
    {30,"parcialmente nublado (dia)"},    // partly cloudy (day)
    {31,"céu limpo (noite)"},             // clear (night)
    {32,"ensolarado"},                    // sunny
    {33,"tempo bom (noite)"},             // fair (night)
    {34,"tempo bom (dia)"},               // fair (day)
    {35,"chuva e granizo"},               // mixed rain and hail
    {36,"quente"},                        // hot
    {37,"tempestades isoladas"},          // isolated thunderstorms
    {38,"tempestades esparsas"},          // scattered thunderstorms
    {39,"tempestades esparsas"},          // scattered thunderstorms
    {40,"chuvas esparsas"},               // scattered showers
    {41,"nevasca"},                       // heavy snow
    {42,"tempestades de neve esparsas"},  // scattered snow showers
    {43,"nevasca"},                       // heavy snow
    {44,"parcialmente nublado"},          // partly cloudy
    {45,"chuva com trovoadas"},           // thundershowers
    {46,"tempestade de neve"},            // snow showers
    {47,"relâmpagos e chuvas isoladas"},  // isolated thundershowers
    {3200,"não disponível"}               // not available
  };
//init variables 
const char* city;
String cidade;
const char* country;
const char* region;
int temp;
int code;
const char hostname[] = "query.yahooapis.com";
const char* HTML PROGMEM = "<!DOCTYPE html><html lang=\"pt-br\"><head> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"/> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1, maximum-scale=1.0\"/> <title>Clima Cloud</title> <link href=\"https://fonts.googleapis.com/icon?family=Material+Icons\" rel=\"stylesheet\"> <link href=\"materialize.min.css\" type=\"text/css\" rel=\"stylesheet\" media=\"screen,projection\"/> <link href=\"style.css\" type=\"text/css\" rel=\"stylesheet\" media=\"screen,projection\"/> <style>#loading{background-color: #bd4932;height: 100%;width: 100%;position: fixed;z-index: 1;margin-top: 0px;top: 0px;}#loading-center{width: 100%;height: 100%;position: relative;}#loading-center-absolute{position: absolute;left: 50%;top: 50%;height: 200px;width: 200px;margin-top: -100px;margin-left: -100px;}#object{width: 80px;height: 80px;background-color: #FFF;-webkit-animation: animate 1s infinite ease-in-out;animation: animate 1s infinite ease-in-out;margin-right: auto;margin-left: auto;margin-top: 60px;}@-webkit-keyframes animate{0%{-webkit-transform: perspective(160px);}50%{-webkit-transform: perspective(160px) rotateY(-180deg);}100%{-webkit-transform: perspective(160px) rotateY(-180deg) rotateX(-180deg);}}@keyframes animate{0%{transform: perspective(160px) rotateX(0deg) rotateY(0deg); -webkit-transform: perspective(160px) rotateX(0deg) rotateY(0deg);}50%{transform: perspective(160px) rotateX(-180deg) rotateY(0deg); -webkit-transform: perspective(160px) rotateX(-180deg) rotateY(0deg) ;}100%{transform: perspective(160px) rotateX(-180deg) rotateY(-180deg); -webkit-transform: perspective(160px) rotateX(-180deg) rotateY(-180deg);}}</style></head><body class=\"blue\" id=\"body\"> <div class=\"section no-pad-bot\" id=\"index-banner\"> <div class=\"container\"> <br><br><h1 class=\"header center amber-text lighten-2-text\">Tempo Loko</h1> <div class=\"row center\"> <h6 class=\"header col s12 white-text\">Cidade Atual: <span id=\"cidade\">Sem Cidade</span>, <span id=\"country\"></span> - <span id=\"region\"></span></h6> </div><div class=\"row center\" id=\"change\"> <a id=\"changeCity\" class=\"btn-large waves-effect waves-light orange\">Trocar Cidade</a> </div><div id=\"divcidade\" class=\"row center hide\"> <div class=\"col s12 white-text\"> <div class=\"row\"></div><div class=\"input-field inline\"> <input id=\"city\" type=\"text\" class=\"white-text\"> <label for=\"city\" id=\"cityLabel\">Insira a cidade aqui</label> </div></div></div><div class=\"row center hide\" id=\"options\"> <a id=\"savecity\" class=\"btn-large waves-effect waves-light orange\">Salvar</a> <a id=\"savestatus\" class=\"btn-large waves-effect waves-light orange hide\">Salvando ...</a> <a id=\"cancelcity\" class=\"btn-large waves-effect waves-light red\">Cancelar</a> </div><div class=\"row center\"> <h5 class=\"header col s12 white-text\">Temperatura agora: <span id=\"temp\">Sem Temperatura</span> C</h5> <h5 class=\"header col s12 white-text\">Atualmente <span id=\"condition\"></span></h5> </div></div><div id=\"loading\"> <div id=\"loading-center\"> <div id=\"loading-center-absolute\"> <div id=\"object\"></div></div></div></div></div><script src=\"jquery-2.1.1.min.js\"></script> <script src=\"materialize.min.js\"></script> <script>$(window).load(function(){$.getJSON('/config.json').done(function(data){$(\"#cidade\").text(data.city); $(\"#region\").text(data.region); $(\"#country\").text(data.country); $(\"#temp\").text(data.temp); $(\"#condition\").text(data.condition); $(\"#loading\").fadeOut(500);});}); $(\"#changeCity\").click(function(){if($(\"#divcidade\").hasClass(\"hide\")){$(\"#options\").removeClass(\"hide\"); $(\"#divcidade\").removeClass(\"hide\"); $(\"#change\").addClass(\"hide\");}}); $(\"#cancelcity\").click(function(){if($(\"#change\").hasClass(\"hide\")){$(\"#options\").addClass(\"hide\"); $(\"#divcidade\").addClass(\"hide\"); $(\"#change\").removeClass(\"hide\"); $(\"#city\").val(\"\"); M.updateTextFields();}}); $(\"#savecity\").click(function(){if($(\"#change\").hasClass(\"hide\")){if($(\"#city\").val()==\"\"){M.toast({html: 'Confira o campo cidade', classes: \"red\"});}else{$(\"#cancelcity\").addClass(\"hide\"); $(\"#savecity\").addClass(\"hide\"); $(\"#savestatus\").removeClass(\"hide\"); $.ajax({type: 'POST', url: '/setalocal', data:{'local': $(\"#city\").val()}, success: function(msg){alert('Alterado com sucesso');}, complete: function(){setTimeout(function(){$.getJSON('/config.json').done(function(data){$(\"#cidade\").text(data.city); $(\"#region\").text(data.region); $(\"#country\").text(data.country); $(\"#temp\").text(data.temp); $(\"#condition\").text(data.condition); $(\"#options\").addClass(\"hide\"); $(\"#divcidade\").addClass(\"hide\"); $(\"#city\").val(\"\"); M.updateTextFields(); $(\"#cancelcity\").removeClass(\"hide\"); $(\"#savecity\").removeClass(\"hide\"); $(\"#savestatus\").addClass(\"hide\"); $(\"#change\").removeClass(\"hide\");});},2000);}});}}}); </script> </body></html>";
const int port = 80;
//set pinos
const int RED = 5;
const int GREEN = 0;
const int BLUE = 4;
//html index
bool trocou = false;
bool first = true;
ESP8266WebServer server(80);

void setup(){
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  analogWrite(BLUE, 0);
  analogWrite(GREEN, 0);
  analogWrite(RED, 0);

  Serial.begin(115200);
  startWiFi();
  startServer();
  if (!loadConfig()) {
    Serial.println("Falha ao carregar as configs");
  } else {
    Serial.println("Config Defualt carregadas");
  }
  server.begin();
}

void loop() {
  server.handleClient();
  if (trocou) {
    setColourRgb(0,0,0);
    setColourRgb(255,255,255);
  }
  setJson(consult());
  delay(2000);
}
//metodos
String consult(){
  String jsonWeather;
  HTTPClient http;
  String url = "/v1/public/yql?q=select%20%20link%2Clocation.city%2Clocation.country%2Clocation.region%2Citem.condition.temp%2Citem.condition.code%20from%20weather.forecast%20where%20woeid%20in%20(select%20woeid%20from%20geo.places(1)%20where%20text%3D\""+urlencode(cidade)+"\")and%20u=%27c%27&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys";
  http.begin(hostname,port,url);
  int httpCode = http.GET();
  if (httpCode) {
    if (httpCode == 200) {
      jsonWeather = http.getString();
    }else{
       Serial.println("CODE ERROR:" + httpCode);
    }
  }else{
      Serial.println("Erro na requisicao http");
  }
  Serial.println("Fechar conexao");
  http.end();
  return jsonWeather;
}
void startWiFi() {
  
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");
  
/*
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi..");
  }
*/
  Serial.println(WiFi.localIP());
}
void setJson(String jsonWeather){
  DynamicJsonBuffer jsonBuffer(4000);

  JsonObject& root = jsonBuffer.parseObject(jsonWeather);

  if (!root.success()) {
    Serial.println("Falha na conversao do json");
  }else{
      JsonObject& query = root["query"];
      JsonObject& query_results_channel = query["results"]["channel"];
      const char* query_results_channel_link = query_results_channel["link"];
      JsonObject& query_results_channel_location = query_results_channel["location"];
      
      city = query_results_channel_location["city"];
      country = query_results_channel_location["country"];
      region = query_results_channel_location["region"];
      
      temp = query_results_channel["item"]["condition"]["temp"];
      code = query_results_channel["item"]["condition"]["code"];
      condition = myDictionaryArr[code].value;
      Serial.printf("Temperatura atual: %d%\n", temp);
      Serial.print("Condição atual: ");
      Serial.println(myDictionaryArr[code].value);
      Serial.println(city);
      if(first){
        saveConfig();
        first = false;
      }
      if(trocou){
        saveConfig();
        trocou = false;
      }
      if (temp >= 26){
        setColourRgb(0,0,0);
        setColourRgb(255,0,0);
      } else if(temp < 26 && temp >= 18 ){
        setColourRgb(0,0,0);
        setColourRgb(0,255,0);
      }else if(temp < 18){
        setColourRgb(0,0,0);
        setColourRgb(0,0,255);
      }
  } 
}
void startServer(){
  SPIFFS.begin();
  server.serveStatic("/materialize.min.css", SPIFFS, "/materialize.min.css", "max-age=86400");
  server.serveStatic("/config.json", SPIFFS, "/config.json");
  server.serveStatic("/style.css", SPIFFS, "/style.css", "max-age=86400");
  server.serveStatic("/jquery-2.1.1.min.js", SPIFFS, "/jquery-2.1.1.min.js", "max-age=86400"); 
  server.serveStatic("/materialize.min.js", SPIFFS, "/materialize.min.js", "max-age=86400");
  server.on("/mudalocal", mudalocal);
  server.on("/setalocal", setLocal);
}
bool loadConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Falha ao abrir config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file muito grande");
    return false;
  }
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    Serial.println("Falha ao converter o config file");
    return false;
  }
  
  city = json["city"];
  cidade = city;
  country = json["country"];
  region = json["region"];
  temp = json["temp"];
  condition = json["condition"];

  return true;
}

bool saveConfig() {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  
  json["city"] = city;
  json["country"] = country;
  json["region"] = region;
  json["temp"] = temp;
  json["condition"] = condition;

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Falha ao escrever no config.json");
    return false;
  }
  
  json.printTo(configFile);
  return true;
}

void mudalocal() {
  setColourRgb(0,0,0);
  setColourRgb(255,255,255);
  server.send(200, "text/html", HTML);
}
void setLocal() {
  setColourRgb(0,0,0);
  setColourRgb(0,255,0);
  cidade = server.arg("local");
  server.send(200, "text/javascript", "Ok");
  trocou = true;
}
void setColourRgb(unsigned int red, unsigned int green, unsigned int blue) {
  analogWrite(RED, (red*1023/255));
  analogWrite(GREEN, green*1023/255);
  analogWrite(BLUE, blue*1023/255);
}
String urlencode(String str)
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i =0; i < str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
      yield();
    }
    return encodedString;
}
