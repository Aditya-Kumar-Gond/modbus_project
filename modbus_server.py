import time
from pyModbusTCP.server import ModbusServer
from threading import Thread

def print_data_periodically(server):
    while True:
        # Reading the holding registers, coils, input registers, and discrete inputs.
        hr = server.data_bank.get_holding_registers(0, 10)
        cr = server.data_bank.get_coils(0, 10)
        ir_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        server.data_bank.set_input_registers(0, ir_list)
        dr_list = [True, False, True, False, True, False, True, False, True, False]
        server.data_bank.set_discrete_inputs(0, dr_list)

        # Print the current values
        print(f"Coils: {cr}")
        print(f"Holding Registers: {hr}")
        print(f"Input Registers: {ir_list}")
        print(f"Discrete Inputs: {dr_list}")
        
        # Sleep for 1 second before the next print
        time.sleep(1)

def run_modbus_server():
    server = ModbusServer(host="0.0.0.0", port=5021, no_block=True)
    print("Starting Modbus server...")

    # Start the data print thread with corrected argument passing
    thread = Thread(target=print_data_periodically, args=(server,), daemon=True)
    thread.start()

    try:
        server.start()  # Start the Modbus server
        while True:
            time.sleep(1)  # Keep the server running, and let other threads do their work
    except KeyboardInterrupt:
        print("Stopping Modbus server...")
        server.stop()

if __name__ == "__main__":
    run_modbus_server()
