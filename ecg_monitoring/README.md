# Projeto de Monitoramento Cardíaco com NodeMCU ESP8266 e Sensor AD8232

Este repositório contém a documentação completa, código-fonte e instruções para reproduzir o projeto de monitoramento cardíaco utilizando o NodeMCU ESP8266 e o sensor AD8232.

## Descrição do Funcionamento

Este projeto visa monitorar sinais cardíacos usando um sensor de ECG AD8232 e transmitir os dados em tempo real para uma plataforma de nuvem utilizando o protocolo MQTT. O NodeMCU ESP8266 processa os sinais do sensor e se conecta à internet via Wi-Fi, permitindo o monitoramento remoto dos sinais cardíacos.

## Software Desenvolvido

O código-fonte desenvolvido para o NodeMCU ESP8266 está disponível neste repositório, incluindo:

- Leitura dos sinais do sensor AD8232.
- Detecção de desconexão dos eletrodos.
- Conexão com a rede Wi-Fi.
- Envio dos dados do ECG para o broker MQTT.

### Estrutura do Código

- **ecg_monitoring.ino**: Código principal do projeto, incluindo configuração de Wi-Fi, leitura do sensor e envio de dados via MQTT.

## Hardware Utilizado

### Componentes

- **NodeMCU ESP8266**: Microcontrolador com Wi-Fi integrado.
- **Sensor de Eletrocardiograma AD8232**: Sensor para monitoramento dos sinais cardíacos.
- **Breadboard e Jumpers**: Para montagem e conexões.
- **Eletrodos**: Para captar os sinais elétricos do coração.

### Especificações das Peças

- **NodeMCU ESP8266**:
  - Alimentação: 3.3V
  - Conexão: Wi-Fi 802.11 b/g/n
- **Sensor AD8232**:
  - Alimentação: 3.3V
  - Conexões: OUTPUT, LO+, LO-, 3.3V, GND


## Documentação das Interfaces e Protocolos

### MQTT

O projeto utiliza o protocolo MQTT para comunicação com a plataforma de nuvem. A biblioteca PubSubClient é usada para gerenciar as conexões MQTT.

### Wi-Fi

O NodeMCU ESP8266 se conecta à internet via Wi-Fi, utilizando a biblioteca ESP8266WiFi para gerenciar a conexão.

### Estrutura das Mensagens MQTT

- **Tópico para envio de dados do sensor**: `/v1.6/devices/monitor_ecg`
- **Formato da mensagem**:
  ```json
  {
    "ecg_sensor": {
      "value": <valor_do_sensor>,
      "timestamp": <timestamp>
    }
  }

### Comunicação/Controle via Internet

Este projeto possui comunicação e controle via internet utilizando o protocolo TCP/IP para se conectar à rede Wi-Fi e o protocolo MQTT para transmissão dos dados do sensor.

## Como Reproduzir

### Passos para Montagem

1. **Conecte o NodeMCU ESP8266 ao sensor AD8232**:
   - `3.3V` do NodeMCU ao `3.3V` do AD8232
   - `GND` do NodeMCU ao `GND` do AD8232
   - `A0` do NodeMCU ao `OUTPUT` do AD8232
   - `GPIO10 (S3)` do NodeMCU ao `LO+` do AD8232
   - `GPIO9 (S2)` do NodeMCU ao `LO-` do AD8232


### Passos para Configuração

1. Clone este repositório:
   ```sh
   git clone https://github.com/giselemascarenhas/ecg_arduino_project.git
   cd seu_repositorio

1. Abra o **ecg_monitoring.ino** no Arduino IDE.

2. Atualize as credenciais de Wi-Fi, token do Ubidots, e outros parâmetros necessários.

3. Faça o upload do código para o NodeMCU.

### Passos para Operação

1. Conecte os eletrodos ao corpo do paciente.
2. Ligue o NodeMCU e verifique a conexão com a rede Wi-Fi.
3. Observe as leituras do sensor no monitor serial ou na plataforma Ubidots.

### Links Úteis

- [Documentação do MQTT](http://mqtt.org/)
- [Ubidots](https://ubidots.com/)

