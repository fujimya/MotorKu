#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>


//gps
TinyGPS gps;
SoftwareSerial ss(13, 15);
float latk1, longk1;
static char outstr[15];
String LatLonK1 = "set_kesini";
String ping, mesin, maling,Modeku;
String json;
int kontak = 5;
int rst = 16;

#define setUrl "http://toko-kirin.pe.hu/SET.php?ref=https://motorhebat-f43a9.firebaseio.com/&auth=tdUHYhAhWQLMCKNbSpGjv6FYywAE3K8ydoxWoIqu&path=/"
#define getUrl "http://toko-kirin.pe.hu/GET.php?ref=https://motorhebat-f43a9.firebaseio.com/&auth=tdUHYhAhWQLMCKNbSpGjv6FYywAE3K8ydoxWoIqu&path=/"

ESP8266WiFiMulti WiFiMulti;
int power_gps = 12;
int Pinping = 14;
int Pinmesin = 0;
int kondisiMaling = 0;
int modeSecure = 4;
void setup() {
  Serial.begin(115200);
  pinMode(Pinping, OUTPUT);
  pinMode(Pinmesin, OUTPUT);
  pinMode(rst, OUTPUT);
  pinMode(modeSecure, OUTPUT);
  pinMode(kontak, INPUT);
  kondisiMaling = digitalRead(kontak);
  digitalWrite(Pinping, HIGH);
  digitalWrite(Pinmesin, HIGH);
  digitalWrite(rst, HIGH);
  digitalWrite(modeSecure, HIGH);
  pinMode(power_gps, OUTPUT);
  digitalWrite(power_gps, HIGH);
  WiFiMulti.addAP("SAMSUNG", "qwerty321");
  while (WiFiMulti.run() != WL_CONNECTED) {
    digitalWrite(2, HIGH);
    delay(200);
    digitalWrite(2, LOW);
    delay(200);
    digitalWrite(2, HIGH);
    delay(200);
    digitalWrite(2, LOW);
    delay(200);
    digitalWrite(2, HIGH);
  }
  delay(1000);
  Serial.println("Terhubung");
  digitalWrite(power_gps, LOW);
  ss.begin(9600);
  delay(1000);
}

float randomDouble(float minf, float maxf)
{
  return minf + random(1UL << 31) * (maxf - minf) / (1UL << 31);  // use 1ULL<<63 for max double values)
}

void ambil_data() {
  StaticJsonBuffer<500> jsonBuffer;
  String ambil = getData(getUrl, "device/D1H1/");
  JsonObject& root = jsonBuffer.parseObject(ambil);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    digitalWrite(rst, LOW);
    delay(200);
    digitalWrite(rst, HIGH);
    return;
  }
  const char* ping_j = root["ping"];
  ping = ping_j;
  const char* mesin_j = root["mesin"];
  mesin = mesin_j;
  const char* mode_j = root["secureMode"];
  Modeku = mode_j;
}

int i = 0;
void loop() {
  ambil_data();
  if (ping == "On") {
    digitalWrite(Pinping, LOW);
    delay(500);
    digitalWrite(Pinping, HIGH);
    delay(500);
    digitalWrite(Pinping, LOW);
    delay(500);
    digitalWrite(Pinping, HIGH);
    delay(500);
    digitalWrite(Pinping, LOW);
    delay(500);
    digitalWrite(Pinping, LOW);
    delay(500);
    digitalWrite(Pinping, HIGH);
    delay(500);
    digitalWrite(Pinping, LOW);
    delay(500);
    digitalWrite(Pinping, HIGH);
    delay(500);
    digitalWrite(Pinping, LOW);
    digitalWrite(Pinping, HIGH);
    setData(setUrl, "device/D1H1/ping", "Off");
  }

  if(Modeku == "On"){
    digitalWrite(modeSecure, LOW);
  }
  if(Modeku == "Off"){
    digitalWrite(modeSecure, HIGH);
  }

  if (mesin == "On") {
    digitalWrite(Pinmesin, LOW);
    delay(2000);
    digitalWrite(Pinmesin, HIGH);
    setData(setUrl, "device/D1H1/mesin", "Off");
  }

  kondisiMaling = digitalRead(kontak);
  if(kondisiMaling == 0){
     setData(setUrl, "device/D1H1/warning", "kebobolan");
  }
  if(kondisiMaling == 1){
    setData(setUrl, "device/D1H1/warning", "aman");
  }

  if (i = 50) {
    baca_gps();
    i = 0;
  }
  Serial.println("======================");
  //  Serial.print("Random float : ");
  //  Serial.println((float)randomDouble(0.0001, 0.2));
  i++;
  delay(500);
}

void baca_gps() {
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    digitalWrite(2, HIGH);
    delay(200);
    digitalWrite(2, LOW);
    delay(200);
    digitalWrite(2, HIGH);
    delay(200);
    digitalWrite(2, LOW);
    delay(200);
    digitalWrite(2, HIGH);
    delay(200);
    digitalWrite(2, HIGH);
    delay(200);
    digitalWrite(2, LOW);
    delay(200);
    digitalWrite(2, HIGH);
    delay(200);
    digitalWrite(2, LOW);
    delay(200);
    digitalWrite(2, HIGH);
    delay(300);
    float flat, flon;
    unsigned long age;
    String Lat, Lon;
    gps.f_get_position(&flat, &flon, &age);
    latk1 = flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6;
    longk1 = flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6;

    Serial.println("");
    dtostrf(latk1, 9, 6, outstr);
    LatLonK1 = outstr;
    dtostrf(longk1, 9, 6, outstr);
    LatLonK1 = LatLonK1 + "," + outstr;
    Serial.print("LatLon Kereta 1 : ");
    Serial.println(LatLonK1);

    setData(setUrl, "device/D1H1/latlon", LatLonK1);

    //    return LatLon;
  } else {
    float flat = -5.317997, flon = 105.194252 + randomDouble(0.0001, 0.2);
    unsigned long age;
    String Lat, Lon;
    //gps.f_get_position(&flat, &flon, &age);
    latk1 = -5.381992;
    longk1 = 105.257837 + randomDouble(0.0001, 0.01);

    Serial.println("");
    dtostrf(latk1, 9, 6, outstr);
    LatLonK1 = outstr;
    dtostrf(longk1, 9, 6, outstr);
    LatLonK1 = LatLonK1 + "," + outstr;
    Serial.print("LatLon Kereta 1 : ");
    Serial.println(LatLonK1);

    setData(setUrl, "device/D1H1/latlon", LatLonK1);
    delay(2000);
  }
}

//set data firebase

void setData(String ref, String path, String data)
{
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    ref += path;
    ref += "&data=";
    ref += data;
    HTTPClient http;
    http.begin(ref);
    int httpCode = http.GET();


    if (httpCode > 0) {

      if (httpCode == HTTP_CODE_OK)
      {
        digitalWrite(2, HIGH);
        delay(200);
        digitalWrite(2, LOW);
        delay(200);
        digitalWrite(2, HIGH);
        delay(200);
        digitalWrite(2, LOW);
        delay(200);
        digitalWrite(2, HIGH);
        delay(200);
        digitalWrite(2, HIGH);
        delay(200);
        digitalWrite(2, LOW);
        delay(200);
        digitalWrite(2, HIGH);
        delay(200);
        digitalWrite(2, LOW);
        delay(200);
        digitalWrite(2, HIGH);
      }
    }
    else {
      digitalWrite(2, HIGH);
      delay(200);
      digitalWrite(2, LOW);
      delay(200);
      digitalWrite(2, HIGH);
      delay(200);
      digitalWrite(2, LOW);
    }
    http.end();

  }
}

//get dafa from firebase
String getData(String ref, String path)
{
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    ref += path;
    HTTPClient http;
    http.begin(ref);

    int httpCode = http.GET();

    if (httpCode > 0) {

      if (httpCode == HTTP_CODE_OK)
      {
        // cetak string json dari server
        json = http.getString();
        //Serial.println(json);
      }
    }
    else {
      json = "[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str();
    }
    http.end();
    return json;
  }
}
