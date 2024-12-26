#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <modbus/modbus.h>
#include <cstring>

void print_data_periodically(modbus_t *ctx, modbus_mapping_t *mb_mapping) {
    while (true) {
        std::cout << "Coils: ";
        for (int i = 0; i < 10; i++) {
            std::cout << static_cast<int>(mb_mapping->tab_bits[i]) << " ";
        }
        std::cout << std::endl;

        std::cout << "Holding Registers: ";
        for (int i = 0; i < 10; i++) {
            std::cout << mb_mapping->tab_registers[i] << " ";
        }
        std::cout << std::endl;

        std::cout << "Input Registers: ";
        for (int i = 0; i < 10; i++) {
            std::cout << mb_mapping->tab_input_registers[i] << " ";
        }
        std::cout << std::endl;

        std::cout << "Discrete Inputs: ";
        for (int i = 0; i < 10; i++) {
            std::cout << static_cast<int>(mb_mapping->tab_input_bits[i]) << " ";
        }
        std::cout << std::endl;

        // Sleep for 1 second before the next print
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void run_modbus_server() {
    modbus_t *ctx = modbus_new_tcp("0.0.0.0", 502); // Use localhost and a non-privileged port
    if (ctx == nullptr) {
        std::cerr << "Unable to allocate libmodbus context: " << modbus_strerror(errno) << std::endl;
        return;
    }

    modbus_set_debug(ctx, TRUE);

    // Create a mapping for holding registers, input registers, coils, and discrete inputs
    modbus_mapping_t *mb_mapping = modbus_mapping_new(10, 10, 10, 10);
    if (mb_mapping == NULL) {
        std::cerr << "Failed to allocate the mapping: " << modbus_strerror(errno) << std::endl;
        modbus_free(ctx);
        return;
    }

    // Initialize some sample data
    for (int i = 0; i < 10; i++) {
        mb_mapping->tab_bits[i] = i % 2;
        mb_mapping->tab_input_bits[i] = (i + 1) % 2;
        mb_mapping->tab_registers[i] = i * 10;
        mb_mapping->tab_input_registers[i] = i * 100;
    }

    // Start the server
    int socket = modbus_tcp_listen(ctx, 1);
    if (socket == -1) {
        std::cerr << "Unable to listen: " << modbus_strerror(errno) << std::endl;
        modbus_mapping_free(mb_mapping);
        modbus_free(ctx);
        return;
    }

    std::cout << "Starting Modbus server on port 5021..." << std::endl;

    std::thread print_thread(print_data_periodically, ctx, mb_mapping);

    uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
    fd_set refset;
    fd_set rdset;

    // Initialize the file descriptor set
    FD_ZERO(&refset);
    FD_SET(socket, &refset);

    while (true) {
        rdset = refset;
        if (select(socket + 1, &rdset, NULL, NULL, NULL) > 0) {
            int master_socket = modbus_tcp_accept(ctx, &socket);
            if (master_socket == -1) {
                std::cerr << "Accept error: " << modbus_strerror(errno) << std::endl;
                continue;
            }

            while (true) {
                int rc = modbus_receive(ctx, query);
                if (rc > 0) {
                    modbus_reply(ctx, query, rc, mb_mapping);
                } else if (rc == -1) {
                    // Connection closed by the client or error
                    std::cerr << "Connection closed or error occurred" << std::endl;
                    break;
                }
            }

            std::cout << "Client disconnected. Waiting for new connection..." << std::endl;
            modbus_close(ctx);
        }
    }

    // Cleanup
    modbus_mapping_free(mb_mapping);
    modbus_close(ctx);
    modbus_free(ctx);
    print_thread.join();
}

int main() {
    run_modbus_server();
    return 0;
}
