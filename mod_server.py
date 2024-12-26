from pymodbus.server.sync import StartTcpServer
from pymodbus.datastore import ModbusSequentialDataBlock, ModbusSlaveContext, ModbusServerContext
from pymodbus.device import ModbusDeviceIdentification


class CustomDataBlock(ModbusSequentialDataBlock):
    def _init_(self, address, values):
        super()._init_(address, values)

    def setValues(self, address, values):
        super().setValues(address, values)
        print(f"Value at address {address} changed to {values}")


def run_server():
    # Initialize data store
    coils = CustomDataBlock(0, [False, True, False])
    discrete_inputs = CustomDataBlock(0, [True] * 10)
    holding_registers = CustomDataBlock(0, [0, 1, 0, 0])
    input_registers = CustomDataBlock(0, [0, 1, 0, 1, 0])

    store = ModbusSlaveContext(
        di=discrete_inputs, co=coils, hr=holding_registers, ir=input_registers)
    context = ModbusServerContext(slaves=store, single=True)

    # Server identity
    identity = ModbusDeviceIdentification()
    identity.VendorName = 'Pymodbus'
    identity.ProductCode = 'PM'
    identity.VendorUrl = 'https://github.com/pymodbus-dev/pymodbus/'
    identity.ProductName = 'Pymodbus Server'
    identity.ModelName = 'Pymodbus Server'
    identity.MajorMinorRevision = '3.1.0'

    # Start server
    print("Starting server...")
    StartTcpServer(context, identity=identity, address=("localhost", 502))


if _name_ == "_main_":
    try:
        run_server()
    except KeyboardInterrupt:
        print("Stopping server...")
        print("Server stopped")