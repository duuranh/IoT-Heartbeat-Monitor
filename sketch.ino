// IoT Heartbeat Monitor - Scenario 2 (MQTT)
// Author: Alecsei Ivanov da Costa e Víctor Céspedes Duran
// OBJETOS INTELIGENTES CONECTADOS

#include <WiFi.h>
#include <PubSubClient.h>

#define ECG_PIN 34
#define LED_PIN 25
#define BUZZER_PIN 26

const int LOW_THRESHOLD = 500;
const int HIGH_THRESHOLD = 2000;

unsigned long anomalyStart = 0;
bool alertPublished = false;

// Controle manual via MQTT
bool manualLedControl = false;
bool manualBuzzerControl = false;

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);

// ---------------- WIFI ----------------
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi NOT connected (Wokwi network error)");
  }
}

// --------------- CALLBACK MQTT ------------------
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida no tópico: ");
  Serial.println(topic);

  String msg = "";
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  msg.trim();
  Serial.print("Payload: ");
  Serial.println(msg);

  // ------ RESET COMPLETO ------
  if (msg.equalsIgnoreCase("RESET")) {
    manualLedControl = false;
    manualBuzzerControl = false;
    anomalyStart = 0;
    alertPublished = false;

    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);

    Serial.println("### RESET COMPLETO: sistema voltou ao modo automático ###");
    return; // Sai da callback para evitar conflitos
  }

  // ------ Comandos para LED ------
  if (msg.equalsIgnoreCase("LED_ON")) {
    manualLedControl = true;
    digitalWrite(LED_PIN, HIGH);
    Serial.println("### LED LIGADO via MQTT ###");
  }
  else if (msg.equalsIgnoreCase("LED_OFF")) {
    manualLedControl = true;
    digitalWrite(LED_PIN, LOW);
    Serial.println("### LED DESLIGADO via MQTT ###");
  }
  else if (msg.equalsIgnoreCase("LED_AUTO")) {
    manualLedControl = false;
    Serial.println("### LED voltou ao modo automático ###");
  }

  // ------ Comandos para BUZZER ------
  else if (msg.equalsIgnoreCase("BUZZER_ON")) {
    manualBuzzerControl = true;
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("### BUZZER LIGADO via MQTT ###");
  }
  else if (msg.equalsIgnoreCase("BUZZER_OFF")) {
    manualBuzzerControl = true;
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println("### BUZZER DESLIGADO via MQTT ###");
  }
  else if (msg.equalsIgnoreCase("BUZZER_AUTO")) {
    manualBuzzerControl = false;
    Serial.println("### BUZZER voltou ao modo automático ###");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32Client_Alecsei")) {
      Serial.println("connected!");
      client.subscribe("iot/monitor/cmd");
      Serial.println("Subscribed to iot/monitor/cmd");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void publish_ecg(int value) {
  char msg[50];
  snprintf(msg, sizeof(msg), "%d", value);
  client.publish("iot/monitor/ecg", msg);
}

void publish_alert(const char* txt) {
  client.publish("iot/monitor/alert", txt);
}

// --------------------- SETUP -----------------------
void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(mqtt_callback);

  Serial.println("Scenario 2: MQTT simulation started.");
}

// ---------------------- LOOP ------------------------
void loop() {

  // MQTT + WiFi
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      reconnect();
    }
    client.loop();
  }

  // Leitura do ECG
  int sinal = analogRead(ECG_PIN);
  Serial.print("ECG analog value: ");
  Serial.println(sinal);

  bool abnormal = (sinal < LOW_THRESHOLD) || (sinal > HIGH_THRESHOLD);

  // ----------------- CONTROLE DO LED -----------------
  if (!manualLedControl) {
    digitalWrite(LED_PIN, abnormal ? HIGH : LOW);
  }

  // ----------------- LÓGICA DE ALERTA -----------------
  if (abnormal) {

    if (anomalyStart == 0) {
      anomalyStart = millis();
      alertPublished = false;
    }

    // Após 2 segundos → buzzer toca e fica constante
    if (!manualBuzzerControl && millis() - anomalyStart >= 2000) {
      digitalWrite(BUZZER_PIN, HIGH);

      if (!alertPublished && client.connected()) {
        publish_alert("Alert: abnormal heart measurement detected");
        alertPublished = true;
      }
    }

  } else {

    // Se não estiver com override manual, buzzer desliga
    if (!manualBuzzerControl) {
      digitalWrite(BUZZER_PIN, LOW);
    }

    anomalyStart = 0;
    alertPublished = false;
  }

  // ---------------- PUBLICAÇÃO DO ECG ------------------
  if (client.connected()) {
    publish_ecg(sinal);
  }

  delay(1000);
}
