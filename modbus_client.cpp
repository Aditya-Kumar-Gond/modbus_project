#include <modbus/modbus.h>
#include <iostream>
#include <unistd.h>

#define SERVER_IP "192.168.1.18"  // Modbus server IP address
#define SERVER_PORT 5020            // Modbus TCP port
#define MODBUS_ID 1                // Slave ID

int main() {
    // Create a Modbus TCP context
    modbus_t *mb;
    mb = modbus_new_tcp(SERVER_IP, SERVER_PORT);
    if (mb == NULL) {
        std::cerr << "Unable to create the Modbus connection" << std::endl;
        return -1;
    }

    modbus_set_slave(mb, MODBUS_ID);

    if (modbus_connect(mb) == -1) {
        std::cerr << "Connection failed: " << modbus_strerror(errno) << std::endl;
        modbus_free(mb);
        return -1;
    }

    // Read 1 holding register starting from address 0
    uint16_t tab_reg[6];  // Array to hold the register value
    int rc = modbus_read_registers(mb, 2, 1, tab_reg);  // Read 6 register starting from address 0
    if (rc == -1) {
        std::cerr << "Read failed: " << modbus_strerror(errno) << std::endl;
    } else {
        std::cout << "Register value: " <<rc<< std::endl;
    }

    // Close the connection
    modbus_close(mb);
    modbus_free(mb);

    return 0;
}
