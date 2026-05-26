#include <stdio.h>
#include <stdint.h>

int hunnu_vm_run(const uint8_t* bytecode, size_t bytecode_len,
                 const uint8_t* constants, size_t constants_size) {
    (void)bytecode;
    (void)bytecode_len;
    (void)constants;
    (void)constants_size;
    fprintf(stderr, "Error: Rust VM not available (compile with Rust/cargo to enable)\n");
    return 1;
}

