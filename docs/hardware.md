# Hardware utilizado

Este documento descreve os componentes e conexões de hardware empregados no projeto **IoT Heartbeat Monitor – Scenario 2 (MQTT)**.

---

## 1. Microcontrolador ESP32

- Placa: ESP32 DevKit V1.
- Periféricos utilizados:
  - ADC para leitura do sinal analógico.
  - GPIOs para LED e buzzer.
  - Wi-Fi integrado (TCP/IP).

### Pinos usados:

- `ECG_PIN` → **GPIO 34** (entrada analógica)
- `LED_PIN` → **GPIO 25** (saída digital)
- `BUZZER_PIN` → **GPIO 26** (saída digital)

---

## 2. Sensor de ECG (simulado)

No Wokwi, o ECG é simulado por um **potenciômetro**.

### Ligações:
- Lado 1 → 3.3V  
- Lado 2 (meio) → GPIO 34  
- Lado 3 → GND  

Em protótipo real, poderia ser substituído por módulo AD8232 (com isolamento adequado).

---

## 3. LED de Indicação

- LED vermelho 5mm.
- Função: sinalizar anomalia (modo automático) ou override (modo manual).

### Ligações:
- Anodo → resistor 220Ω → GPIO 25  
- Catodo → GND  

---

## 4. Buzzer

- Buzzer ativo 5V.

### Ligações:
- Pino positivo → GPIO 26  
- Pino negativo → GND  

---

## 5. Alimentação

- ESP32 alimentado via **USB** (5V).
- Componentes alimentados pelos pinos **3.3V/5V** do ESP32.

---

## 6. Observações Importantes

- Em aplicações reais com ECG, é obrigatório isolamento elétrico.
- No Wokwi, todas as ligações são virtuais, sem risco.
- Os valores do ADC representam o "batimento" simulado.
