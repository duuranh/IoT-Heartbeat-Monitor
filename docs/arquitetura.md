# Arquitetura do Sistema – IoT Heartbeat Monitor (MQTT)

Este documento descreve a arquitetura geral do projeto **IoT Heartbeat Monitor – Scenario 2 (MQTT)**, utilizado para monitorar remotamente um sinal cardíaco simulado e enviar alertas via MQTT.

---

## 1. Visão em Camadas

### 1.1. Camada de Dispositivo (Edge)
- Microcontrolador: **ESP32**
- Sensoriamento:
  - Leitura analógica do “ECG” por um potenciômetro no pino GPIO 34.
- Atuadores:
  - LED (GPIO 25)
  - Buzzer (GPIO 26)
- Funções:
  - Processamento local de anomalias.
  - Controle automático ou manual dos atuadores.
  - Publicação de dados e alertas.
  - Recebimento de comandos MQTT.

### 1.2. Camada de Comunicação
- Protocolo: **Wi-Fi (TCP/IP)**
- Aplicação: **MQTT**
- Broker: `test.mosquitto.org` (público)
- Características:
  - Comunicação leve e contínua.
  - Uso de tópicos para publicação/assinatura.

### 1.3. Camada de Aplicação
- Cliente MQTT externo (ex.: **MQTTX**)
- Funções:
  - Visualização dos valores de ECG.
  - Recebimento de alertas.
  - Envio de comandos para controle remoto de LED/buzzer.

---

## 2. Fluxo de Funcionamento do Sistema

1. O ESP32 inicializa e configura os pinos.
2. Conecta à rede Wi-Fi.
3. Conecta ao broker MQTT.
4. Assina o tópico de comandos:  
   - `iot/monitor/cmd`
5. Publica periodicamente:
   - Valores de ECG → `iot/monitor/ecg`
6. Verifica continuamente se o valor está dentro da faixa:
   - Normal: LED e buzzer desligados (modo automático).
   - Anormal:
     - LED é ligado imediatamente.
     - Após ~2s, o buzzer é ativado.
     - Publica alerta → `iot/monitor/alert`.
7. Caso chegue algum comando MQTT:
   - Executa override manual (LED_ON, BUZZER_OFF, etc.).
   - `RESET` retorna para o modo automático.

---

## 3. Possíveis Extensões

- Dashboard web para visualização em tempo real.
- Banco de dados (cloud) para histórico.
- Uso de MQTT com TLS e autenticação.
- Substituição do potenciômetro por sensor AD8232 real.

---

## 4. Objetivo Acadêmico

O projeto demonstra:
- Arquitetura básica de IoT.
- Processamento embarcado.
- Comunicação via Internet usando MQTT.
- Monitoramento remoto e controle distribuído.
