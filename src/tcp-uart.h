#ifndef TCP_UART_H
#define TCP_UART_H

#include "globals.h"

//TCP Socket for direct connection to TunerStudio
//extern AsyncClient TS_TCP_Client;
#define ECU_SERIAL_TIMEOUT 3000

//void handleData(void *arg, AsyncClient *client, void *data, size_t len);
//void handleNewClient(void *arg, AsyncClient *client);
extern void initTCP();

extern uint32_t TCPrequestsReceived;
extern uint8_t numTCPClients;

#endif