#include <stdint.h>

void print_string(char *mes);
void print_uint32(uint32_t *mes);
void print_uint8(uint8_t *mes);
int read_uint32(uint32_t *mes);
int read_uint8(uint8_t *mes);
int read_packet(uint8_t *mes, uint8_t *packet_size);
void init_communication();