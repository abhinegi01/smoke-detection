#define BLYNK_TEMPLATE_ID "TMPL3Jdf1c5GO"
#define BLYNK_TEMPLATE_NAME "FireAlarm"
#define BLYNK_AUTH_TOKEN "KSqL7gCRyvZ91W1bDOfLgi5XesN66ins"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "MISHRAJII";
char pass[] = "1234567890";

#define MQ2_PIN 35
#define FLAME_PIN 27
#define BUZZER_PIN 13

BlynkTimer timer;
int mq2Threshold = 85; // Adjust after testing

void checkFireOrGas() {
  int gasValue = analogRead(MQ2_PIN);
  int flameValue = digitalRead(FLAME_PIN);

  bool fireDetected = (flameValue == LOW);
  bool gasDetected = (gasValue > mq2Threshold);

  // Send sensor values to Blynk app
  Blynk.virtualWrite(V0, gasValue);
  Blynk.virtualWrite(V1, fireDetected ? 1 : 0);

  Serial.print("Gas: "); Serial.print(gasValue);
  Serial.print(" Flame: "); Serial.print(flameValue);
  Serial.print(" Fire Detected: "); Serial.print(fireDetected);
  Serial.print(" Gas Detected: "); Serial.println(gasDetected);

  if (fireDetected || gasDetected) {
    digitalWrite(BUZZER_PIN, HIGH);
    Blynk.logEvent("fire", fireDetected ? "🔥 Fire detected!" : "⚠️ Gas detected!");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(FLAME_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  Blynk.begin(auth, ssid, pass);

  timer.setInterval(1000L, checkFireOrGas);
}

void loop() {
  Blynk.run();
  timer.run();
}
