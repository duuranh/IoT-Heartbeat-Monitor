# 📘 Guia de Simulação – Wokwi  
## IoT Heartbeat Monitor – Scenario 2 (MQTT)

Este guia explica como executar o projeto **IoT Heartbeat Monitor** no simulador **Wokwi**, incluindo importação, execução, teste de MQTT e interação com os atuadores.

---

## 📁 Arquivos da Simulação

A pasta `wokwi/` contém:

- **diagram.json**  
  Contém todo o circuito eletrônico (ESP32, potenciômetro, LED e buzzer).

- **libraries.txt**  
  Lista de bibliotecas carregadas automaticamente pelo Wokwi.

- **sketch.ino**  
  Código-fonte principal do ESP32 (Scenario 2 – MQTT).

---

## ▶️ Como Abrir o Projeto no Wokwi

1. Acesse o simulador:  
   **https://wokwi.com**

2. Clique em **"New Project"**  
   Escolha o modelo **ESP32 Arduino**.

3. No editor:
   - Delete qualquer arquivo criado automaticamente.
   - Clique em **File → Upload File** e envie:
     - `diagram.json`
     - `libraries.txt`

4. O circuito será montado automaticamente na tela.

---

## 💾 Inserindo o Código

1. Abra o arquivo `sketch.ino` gerado pelo Wokwi.  
2. Apague todo o conteúdo.  
3. Cole o código localizado em:  
   `codigo-esp32/sketch.ino`

Esse é o código oficial do projeto, contendo:

- conexão Wi-Fi simulada  
- conexão MQTT com o broker `test.mosquitto.org`  
- publicação do ECG  
- lógica de alerta  
- controles MQTT (LED_ON, BUZZER_AUTO, RESET etc.)

---

## ▶️ Executando a Simulação

1. Clique no botão **"Play"** (triângulo verde).  
2. Serão exibidas:
   - a simulação com o circuito  
   - a janela de **Serial Monitor**

3. No Serial Monitor você verá:
   - Conexão à rede virtual Wokwi-GUEST  
   - Conexão ao broker MQTT  
   - Valores analógicos do ECG  
   - Alterações no LED e buzzer  
   - Avisos de alerta quando o valor estiver fora da faixa `500–2000`

4. Gire o **potenciômetro**:
   - O valor lido em ADC mudará  
   - Se entrar em faixa anômala:
     - LED acende  
     - Buzzer ativa após alguns segundos  
     - É publicado um alerta MQTT  

---

## 📡 Testando com um Cliente MQTT Externo

Mesmo no Wokwi, a conexão MQTT é **real**.

Use o cliente **MQTTX** ou similar:

### 🔌 Conexão:
- **Host:** `test.mosquitto.org`  
- **Porta:** `1883`  
- **Protocolo:** MQTT sem TLS

### 📥 Assinar:
- `iot/monitor/ecg`
- `iot/monitor/alert`

### 📤 Enviar comandos:
- Tópico: `iot/monitor/cmd`

### Comandos disponíveis:
- `RESET`  
- `LED_ON`  
- `LED_OFF`  
- `LED_AUTO`  
- `BUZZER_ON`  
- `BUZZER_OFF`  
- `BUZZER_AUTO`

O efeito é imediato na simulação (LED/buzzer respondem na hora).

---

## ⚠️ Observações Importantes

- O Wokwi usa Wi-Fi virtual → **não precisa senha real**  
- Não use MQTT com TLS no Wokwi (não é suportado)  
- Apesar da simulação, o MQTT atinge um broker real  
- Diagrama completo do fluxo está em:  
  `docs/comunicacao_mqtt.md`

---

## ✔️ Fim do Guia

Sua simulação está pronta para ser utilizada, demonstrada ou apresentada para o professor.
