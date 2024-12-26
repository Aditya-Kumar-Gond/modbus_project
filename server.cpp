#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <modbus/modbus.h>
#include <cstdint>  // For uint16_t and uint8_t

void print_data_periodically(modbus_t *ctx) {
    // Create containers for data
    std::vector<uint16_t> hr(10); // Holding registers (16-bit unsigned integers)
    std::vector<uint8_t> cr(10);  // Coils (8-bit unsigned integers)
    std::vector<uint16_t> ir_list = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // Input registers (16-bit unsigned integers)
    std::vector<bool> dr_list = {true, false, true, false, true, false, true, false, true, false}; // Discrete inputs (booleans)

    while (true) {
        // Read the holding registers (16-bit unsigned integers)
        if (modbus_read_registers(ctx, 0, 10, hr.data()) == -1) {
            std::cerr << "Error reading holding registers: " << modbus_strerror(errno) << std::endl;
        }

        // Read the coils (8-bit unsigned integers)
        if (modbus_read_bits(ctx, 0, 10, cr.data()) == -1) {
            std::cerr << "Error reading coils: " << modbus_strerror(errno) << std::endl;
        }
/*
        // Set input registers (16-bit unsigned integers)
        if (modbus_write_registers(ctx, 0, ir_list.size(), ir_list.data()) == -1) {
            std::cerr << "Error writing input registers: " << modbus_strerror(errno) << std::endl;
        }
        * 
        * /
/*
        // Set discrete inputs (booleans)
        for (size_t i = 0; i < dr_list.size(); i++) {
            if (modbus_write_bit(ctx, i, dr_list[i] ? 1 : 0) == -1) {
                std::cerr << "Error writing discrete input at index " << i << ": " << modbus_strerror(errno) << std::endl;
            }
        }
*/
        // Print the current values
        std::cout << "Coils: ";
        for (auto val : cr) {
            std::cout << (int)val << " "; // Convert to int for printing
        }
        std::cout << std::endl;

        std::cout << "Holding Registers: ";
        for (auto val : hr) {
            std::cout << val << " ";
        }
        std::cout << std::endl;

        std::cout << "Input Registers: ";
        for (auto val : ir_list) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
/*
        std::cout << "Discrete Inputs: ";
        for (auto val : dr_list) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
*/
        // Sleep for 1 second before the next print
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void run_modbus_server() {
    // Initialize the Modbus server
    modbus_t *ctx = modbus_new_tcp("0.0.0.0", 5028); // Listen on all interfaces and port 5021
    if (ctx == nullptr) {
        std::cerr << "Unable to allocate libmodbus context." << std::endl;
        return;
    }

    // Set the server in non-blocking mode
    modbus_set_debug(ctx, TRUE);
    
    // Start the Modbus server in a separate thread
    std::thread print_thread(print_data_periodically, ctx);

    // Open the connection
    if (modbus_connect(ctx) == -1) {
    std::cerr << "Unable to connect to Modbus server: " << modbus_strerror(errno) << std::endl;
    modbus_free(ctx);
    return;
}


    std::cout << "Starting Modbus server..." << std::endl;
    
    // Main server loop (this could be extended to handle requests)
    try {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // Cleanup
    modbus_close(ctx);
    modbus_free(ctx);
    print_thread.join();
}

int main() {
    run_modbus_server();
    return 0;
}
