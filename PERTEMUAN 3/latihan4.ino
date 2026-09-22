#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <DHT.h>

const char *ssid = "505";
const char *password = "16112006";
const char *serverName = "http://10.148.32.121/relay/on"; // Sesuaikan IP Server teman Anda

const byte dhtPin = 2; // Pin data DHT11 (GPIO 2 / D4)
DHT dht(dhtPin, DHT22);

void setup() {  
  Serial.begin(115200);  
  dht.begin();
  
  WiFi.mode(WIFI_STA);  
  WiFi.begin(ssid, password); 
  
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500); 
    Serial.print("."); 
  }  
  Serial.println("\nClient Terhubung ke Wi-Fi!");  
} 

void loop() {  
  // Membaca suhu dari sensor DHT
  float t = dht.readTemperature();

  // Memeriksa apakah pembacaan sensor valid
  if (isnan(t)) {
    Serial.println("Gagal membaca dari sensor DHT!");
    delay(2000);
    return;
  }

  Serial.print("Suhu Saat Ini: ");
  Serial.print(t);
  Serial.println(" °C");

  // Logika Latihan 4: Kirim HTTP GET HANYA JIKA suhu > 35°C
  if ((WiFi.status() == WL_CONNECTED) && (t > 35.0)) {  
    WiFiClient client;  
    HTTPClient http;  
      
    http.begin(client, serverName); 
    int httpResponseCode = http.GET();  
      
    Serial.print("Suhu > 35°C! HTTP Response code: ");  
    Serial.println(httpResponseCode); 
      
    http.end(); 
    
    // Cooldown 10 detik agar tidak membanjiri server
    delay(10000); 
  } 
  
  // Jeda pembacaan sensor setiap 2 detik
  delay(2000);  
}