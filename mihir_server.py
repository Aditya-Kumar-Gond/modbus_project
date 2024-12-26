from pyModbusTCP.server import ModbusServer, DataBank


class CustomDataBank(DataBank):
    def init(self):
        super().init()
        self.set_holding_registers(0, [0, 1, 0, 0])
        self.set_input_registers(0, [0, 1, 0, 1, 0])
        self.set_coils(0, [False, True, False])
        self.set_discrete_inputs(0, [True] * 10)

    def on_holding_registers_change(self, address, from_value, to_value, srv_info):
        print(f"Holding register at address {address} changed from {from_value} to {to_value}")

    def on_coils_change(self, address, from_value, to_value, srv_info):
        print(f"Coil at address {address} changed from {from_value} to {to_value}")


server = ModbusServer(host="0.0.0.0", port=502, no_block=True)
server.data_bank = CustomDataBank()

print("Starting server...")

print("Server is online")

try:
    server.start()
    while True:
        holding_regs = server.data_bank.get_holding_registers(0, 4)
        input_regs = server.data_bank.get_input_registers(0, 5)
        coils = server.data_bank.get_coils(0, 3)
        discrete_inputs = server.data_bank.get_discrete_inputs(0, 10)

        print(f"Holding Registers: {holding_regs}")
        print(f"Input Registers: {input_regs}")
        print(f"Coils: {coils}")
        print(f"Discrete Inputs: {discrete_inputs}")
except KeyboardInterrupt:
    print("Stopping server...")
    server.stop()
    print("Server stopped")
