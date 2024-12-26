#include <iostream>
#include <modbus/modbus.h>

#define MODBUS_SERVER_IP "192.168.137.215"
#define MODBUS_SERVER_PORT 5027

int main() {
    // Create a Modbus context for TCP
    modbus_t *ctx = modbus_new_tcp(MODBUS_SERVER_IP, MODBUS_SERVER_PORT);
    
    // Check if context was created successfully
    if (ctx == NULL) {
        std::cerr << "Unable to create the Modbus context" << std::endl;
        return -1;
    }

    // Connect to the Modbus server
    if (modbus_connect(ctx) == -1) {
        std::cerr << "Connection failed: " << modbus_strerror(errno) << std::endl;
        modbus_free(ctx);
        return -1;
    }
    std::cout << "Connection to Modbus server established." << std::endl;

    // Read 10 holding registers starting from address 0
    uint16_t registers[10];
    int rc = modbus_read_registers(ctx, 0, 10, registers);
    
    if (rc == -1) {
        std::cerr << "Error reading holding registers: " << modbus_strerror(errno) << std::endl;
    } else {
        std::cout << "Register Values: ";
        for (int i = 0; i < 10; ++i) {
            std::cout << registers[i] << " ";
        }
        std::cout << std::endl;
    }

    // Read 10 coils starting from address 0
    uint8_t coils[10];
    rc = modbus_read_bits(ctx, 0, 10, coils);

    if (rc == -1) {
        std::cerr << "Error reading coils: " << modbus_strerror(errno) << std::endl;
    } else {
        std::cout << "Coil Values: ";
        for (int i = 0; i < 10; ++i) {
            std::cout << (int)coils[i] << " ";
        }
        std::cout << std::endl;
    }

    // Write a single register at address 0 with value 100
    int val = 453400;
    rc = modbus_write_register(ctx, 0, val);
    if (rc == -1) {
        std::cerr << "Error writing register: " << modbus_strerror(errno) << std::endl;
    } else {
        std::cout << "Wrote "<<val<<" to register 0." << std::endl;
    }

    // Write a single coil at address 9 with value 1 (true)
    rc = modbus_write_bit(ctx, 8, 1);
    if (rc == -1) {
        std::cerr << "Error writing coil: " << modbus_strerror(errno) << std::endl;
    } else {
        std::cout << "Wrote True (1) to coil 8." << std::endl;
    }

    // Close the connection and free the context
    modbus_close(ctx);
    modbus_free(ctx);
    
    return 0;
}
