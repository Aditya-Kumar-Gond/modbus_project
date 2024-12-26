#include <modbus/modbus.h>
#include <iostream>
#include <vector>
#include <cstring>

int main() {
    // Initialize Modbus RTU context
    modbus_t *ctx = modbus_new_rtu("/dev/ttyUSB0", 9600, 'N', 8, 1);
    if (ctx == nullptr) {
        std::cerr << "Unable to create Modbus context" << std::endl;
        return -1;
    }

    // Set Middleman device slave ID
    int slave_id = 1;  // Update to match your Middleman slave ID
    if (modbus_set_slave(ctx, slave_id) == -1) {
        std::cerr << "Failed to set slave ID: " << modbus_strerror(errno) << std::endl;
        modbus_free(ctx);
        return -1;
    }

    // Connect to the Middleman device
    if (modbus_connect(ctx) == -1) {
        std::cerr << "Connection failed: " << modbus_strerror(errno) << std::endl;
        modbus_free(ctx);
        return -1;
    }

    // Define number of registers to read
    const int NUM_REGISTERS = 10;  // Number of registers to read
    std::vector<uint16_t> registers(NUM_REGISTERS);

    // Read holding registers (function code 0x03)
    int start_address = 0;  // Starting register address
    if (modbus_read_registers(ctx, start_address, NUM_REGISTERS, registers.data()) == -1) {
        std::cerr << "Failed to read registers: " << modbus_strerror(errno) << std::endl;
    } else {
        std::cout << "Read Registers:" << std::endl;
        for (size_t i = 0; i < registers.size(); ++i) {
            std::cout << "Address " << start_address + i << ": " << registers[i] << std::endl;
        }
    }

    // Close the Modbus connection
    modbus_close(ctx);

    // Free the Modbus context
    modbus_free(ctx);

    return 0;
}
