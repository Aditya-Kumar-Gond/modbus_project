from pyModbusTCP.client import ModbusClient

MODBUS_SERVER_IP = "192.168.212.171"
MODBUS_SERVER_PORT = 5020

# Create a Modbus client instance
client = ModbusClient(host=MODBUS_SERVER_IP, port=MODBUS_SERVER_PORT)

# Open connection to the Modbus server
cn = client.open()

# Check if connection is successful
if cn:
    print("Connection to Modbus server established.")
    
    # Read 10 holding registers starting from address 0
    result = client.read_holding_registers(0, 10)
    coil_result = client.read_coils(0, 10)
    # Check if the result is a valid list (indicating success)
    if result:
        print(f"Register Values: {result}")
        print(f"coil Values: {coil_result}")
        client.write_single_register(0,100)
        client.write_single_coil(9,True)
    else:
        print("Error reading holding registers.")
    
    # Close the connection to the Modbus server
    client.close()

else:
    print("Failed to connect to Modbus server.")
