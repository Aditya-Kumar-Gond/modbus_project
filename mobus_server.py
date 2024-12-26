from pymodbus.client import ModbusTcpClient

MODBUS_SERVER_IP = "192.168.137.215"
MODBUS_SERVER_PORT = 5020

client = ModbusTcpClient(MODBUS_SERVER_IP,port=MODBUS_SERVER_PORT)

cn = client.connect()

if cn :
    print("Connection to Modbus server")
    result = client.read_holding_registers(0,10)
    if not result.isError():
        print(f"Register Value : {result.registers}")  
    else :
        print("Error reading register")
    
    client.close()
else :
    print("Failed to connect to modbus server")