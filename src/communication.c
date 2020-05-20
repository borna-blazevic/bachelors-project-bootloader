
#include <stdint.h>
#include <usart.h>
#include <gpio.h>

void print_uint32(uint32_t *mes)
{

    USART1_SendChar((*mes & 0xff000000) >> 24);
    USART1_SendChar((*mes & 0xff0000) >> 16);
    USART1_SendChar((*mes & 0xff00) >> 8);
    USART1_SendChar((*mes & 0xff));
}

void print_string(char *mes)
{

    while (*mes != '\0')
    {
        USART1_SendChar(*mes);
        mes++;
    }
}
void print_uint8(uint8_t *mes)
{

    USART1_SendChar(*mes);
}

int read_uint32(uint32_t *mes)
{

    char c;

    if (USART1_Dequeue(&c) == 0)
        return -1;
    *mes = c;

    if (USART1_Dequeue(&c) == 0)
        return -1;
    *mes |= c << 8;

    if (USART1_Dequeue(&c) == 0)
        return -1;
    *mes |= c << 16;

    if (USART1_Dequeue(&c) == 0)
        return -1;
    *mes |= c << 24;
    return 0;
}

int read_uint8(uint8_t *mes)
{

    if (USART1_Dequeue((char *)mes) == 0)
        return -1;
    return 0;
}
int read_packet(uint8_t *mes, uint8_t *packet_size)
{

    uint8_t i = 0; 
    if(read_uint8(packet_size))
        return -1;
    

    while (i < *packet_size)
    {
        if(read_uint8(mes))
            continue;
        i++;
        mes++;
    }
    return 0;
}

void init_communication()
{
    USART1_Init();
}