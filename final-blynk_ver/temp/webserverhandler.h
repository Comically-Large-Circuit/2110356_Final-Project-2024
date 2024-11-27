#ifndef WEBSERVERHANDLER_H
#define WEBSERVERHANDLER_H

#include <WebServer.h>

// Declare a server instance
extern WebServer server;

// Function to initialize the server
void setupWebServer();

// Function to handle clients
void handleWebServer();

#endif // WEBSERVERHANDLER_H
