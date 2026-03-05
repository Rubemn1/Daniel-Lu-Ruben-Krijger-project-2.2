#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

TinyGPSPlus gps;
HardwareSerial GPS(2);   // UART2 (TX2/RX2)

// instellingen
const unsigned long SAMPLE_INTERVAL = 2000; // 2 seconden
const int TOTAL_SAMPLES = 5;

unsigned long lastSampleTime = 0;
int sampleCount = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("GPS TEST: 5 metingen, elke 2 seconden");
  Serial.println("====================================");

  // ESP32 DevKit: RX2 = GPIO16, TX2 = GPIO17
  GPS.begin(9600, SERIAL_8N1, 16, 17);
}

void loop() {
  // GPS data inlezen
  while (GPS.available()) {
    gps.encode(GPS.read());
  }

  // elke 2 seconden een meting
  if (millis() - lastSampleTime >= SAMPLE_INTERVAL && sampleCount < TOTAL_SAMPLES) {
    lastSampleTime = millis();
    sampleCount++;

    printMeasurement(sampleCount);
  }

  // stop na 5 metingen
  if (sampleCount >= TOTAL_SAMPLES) {
    Serial.println();
    Serial.println("TEST AFGEROND.");
    while (true);
  }
}

void printMeasurement(int nr) {
  Serial.println();
  Serial.print("Meting #");
  Serial.println(nr);
  Serial.println("------------------------------------");

  Serial.print("Fix: ");
  Serial.println(gps.location.isValid() ? "JA" : "NEE");

  Serial.print("Tijd (UTC): ");
  if (gps.time.isValid()) {
    Serial.printf("%02d:%02d:%02d\n",
                  gps.time.hour(),
                  gps.time.minute(),
                  gps.time.second());
  } else {
    Serial.println("N/A");
  }

  Serial.print("Latitude: ");
  Serial.println(gps.location.isValid() ? String(gps.location.lat(), 6) : "N/A");

  Serial.print("Longitude: ");
  Serial.println(gps.location.isValid() ? String(gps.location.lng(), 6) : "N/A");

  Serial.print("Snelheid (km/h): ");
  Serial.println(gps.speed.isValid() ? String(gps.speed.kmph(), 2) : "N/A");

  Serial.print("Koers (deg): ");
  Serial.println(gps.course.isValid() ? String(gps.course.deg(), 1) : "N/A");

  Serial.print("Satellieten: ");
  Serial.println(gps.satellites.isValid() ? String(gps.satellites.value()) : "N/A");

  Serial.print("HDOP: ");
  Serial.println(gps.hdop.isValid() ? String(gps.hdop.hdop(), 2) : "N/A");
}