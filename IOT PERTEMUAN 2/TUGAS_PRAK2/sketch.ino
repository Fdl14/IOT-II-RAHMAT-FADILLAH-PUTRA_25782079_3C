#include <DHT.h>

const byte ldrPin   = 34;   
const byte dhtPin   = 27;   
const byte relayPin = 26;   
const byte ledPin   = 2;    

#define DHTTYPE DHT22       
DHT dht(dhtPin, DHTTYPE);

const bool RELAY_ACTIVE_LOW = true;

const float SUHU_BATAS_ATAS = 34.0;   
const int   LDR_BATAS_GELAP = 1200;   

void setRelay(bool aktif) {
  if (RELAY_ACTIVE_LOW) {
    digitalWrite(relayPin, aktif ? LOW : HIGH);
  } else {
    digitalWrite(relayPin, aktif ? HIGH : LOW);
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);  
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  
  setRelay(false);
  digitalWrite(ledPin, LOW);

  dht.begin();
  Serial.println("=== Sistem Smart Warehouse dimulai ===");
}

void loop() {
  int   ldrValue = analogRead(ldrPin);
  float suhu = dht.readTemperature();


  if (isnan(suhu)) {
    Serial.println("Gagal membaca data dari sensor DHT!");
    delay(1000);
    return;
  }

  
  bool kondisiBuruk = (suhu > SUHU_BATAS_ATAS) || (ldrValue < LDR_BATAS_GELAP);

  if (kondisiBuruk) {
    setRelay(true);
    digitalWrite(ledPin, HIGH);
    Serial.println("Peringatan: Aktuator Aktif!");
  } else {
    setRelay(false);
    digitalWrite(ledPin, LOW);
    Serial.println("Kondisi Aman");
  }


  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.print(" C | LDR (ADC): ");
  Serial.println(ldrValue);
  Serial.println("--------------------------------------");

  delay(1000);
}