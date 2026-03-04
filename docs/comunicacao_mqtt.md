# Comunicação MQTT

Este documento detalha como o projeto utiliza o protocolo **MQTT** sobre TCP/IP para comunicação entre o ESP32 e clientes MQTT externos.

---

## 1. Broker MQTT Utilizado

- Broker público: **test.mosquitto.org**
- Porta: **1883**
- Transporte: **TCP/IP**
- Sem TLS (modo acadêmico)
- Configurado no código via:
  ```cpp
  const char* mqtt_server = "test.mosquitto.org";

## 2. Tópicos MQTT do Projeto
   
### 2.1. Publicação: iot/monitor/ecg
- Direção: ESP32 → broker
- Função: publicar o valor analógico lido (ECG simulado)
- Frequência: ~1 segundo
- Payload: texto simples
- Ex:
  ```cpp
  1532

### 2.2. Publicação: iot/monitor/alert

- Direção: ESP32 → broker
- Função: enviar alerta quando há anomalia prolongada
- Acionamento: valor fora de 500–2000 por alguns segundos
- Payload: texto
- Ex:
  ```cpp
  Alert: abnormal heart measurement detected

### 2.3. Assinatura: iot/monitor/cmd
- Direção: cliente MQTT → ESP32
- Função: controle manual e reset
- Comandos aceitos: 
  - RESET
  - LED_ON, LED_OFF, LED_AUTO
  - BUZZER_ON, BUZZER_OFF, BUZZER_AUTO

## 3. Fluxo MQTT Completo
- ESP32 conecta ao Wi-Fi.
- ESP32 conecta ao broker MQTT.
- Assina iot/monitor/cmd.
- Publica o valor do ECG em iot/monitor/ecg.
- Se detectar anomalia por tempo suficiente:
  - Liga LED e buzzer.
  - Publica alerta em iot/monitor/alert.
- Se receber comandos MQTT:
  - Altera o comportamento dos atuadores.
  - RESET retorna para modo automático.

## 4. QoS e Observações de Segurança
- QoS padrão: 0 (at most once)
- Broker público → sem garantias de entrega
- Em aplicações médicas reais:
  - Usar TLS
  - Autenticação
  - Broker privado
  - QoS 1 ou 2

## 5. Testando com um Cliente MQTT (ex.: MQTTX)
- Conexão:
  - Host: test.mosquitto.org
  - Port: 1883
- Assinar:
  - iot/monitor/ecg
  - iot/monitor/alert
- Publicar comandos:
  - Tópico: iot/monitor/cmd
- Payload:
  - LED_ON, BUZZER_OFF, RESET, etc.
