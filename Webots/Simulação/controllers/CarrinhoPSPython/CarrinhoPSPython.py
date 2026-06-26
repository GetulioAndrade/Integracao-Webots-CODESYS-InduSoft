from controller import Robot
from pyModbusTCP.client import ModbusClient

# Definição do passo de tempo da simulação
TIME_STEP = 6

# Inicializa o objeto do Robô
robot = Robot()

# Definir e configurar os motores
roda_direita = robot.getDevice("roda_direita")
roda_esquerda = robot.getDevice("roda_esquerda")

# Posição configurada como INFINITO para controle por velocidade
roda_direita.setPosition(float('inf'))
roda_esquerda.setPosition(float('inf'))

# Definir os sensores de ultrassom (caso queira printar no console do Webots)
ultrassom1 = robot.getDevice("ds_ultrassom1")
ultrassom2 = robot.getDevice("ds_ultrassom2")
ultrassom3 = robot.getDevice("ds_ultrassom3")

ultrassom1.enable(TIME_STEP)
ultrassom2.enable(TIME_STEP)
ultrassom3.enable(TIME_STEP)

# Configuração do Cliente Modbus TCP
modbus_client = ModbusClient(host="192.168.0.1", port=502, auto_open=True, auto_close=True)

print("Tentando conectar ao Servidor Modbus do CODESYS...")

# Loop principal da simulação
while robot.step(TIME_STEP) != -1:
    
    # Lê os valores das variáveis MotorDireito e MotorEsquerdo direto do CODESYS
    registradores = modbus_client.read_holding_registers(0, 2)
    
    # Obter os valores dos sensores apenas para monitoramento no Webots
    distancia_ultrassom1 = ultrassom1.getValue()
    distancia_ultrassom2 = ultrassom2.getValue()
    distancia_ultrassom3 = ultrassom3.getValue()
    
    if registradores:
        # Desempacota os valores recebidos do mapeamento do CODESYS
        valor_dir_bruto = registradores[0] # Correspondente ao %QW0
        valor_esq_bruto = registradores[1] # Correspondente ao %QW1
        
        # Converte de volta para float (Ex: 40 do PLC vira 4.0 rad/s no Webots)
        velocidade_dir = valor_dir_bruto / 10.0
        velocidade_esq = valor_esq_bruto / 10.0
        
        # Injeta as velocidades comandadas pelo CODESYS
        roda_direita.setVelocity(velocidade_dir)
        roda_esquerda.setVelocity(velocidade_esq)
        
        print(f"Modbus Ativo -> Esq: {velocidade_esq:.1f} | Dir: {velocidade_dir:.1f} | Sensores -> 1E: {distancia_ultrassom1:.1f} 2C: {distancia_ultrassom2:.1f}")
    else:
        # Se perder comunicação, desliga os motores por segurança
        roda_direita.setVelocity(0.0)
        roda_esquerda.setVelocity(0.0)
        print("Erro de comunicação: Certifique-se de que o PLC do CODESYS está em modo RUN com Start PLC!")