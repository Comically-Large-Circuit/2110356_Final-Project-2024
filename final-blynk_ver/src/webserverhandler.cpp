#include <WebServer.h>
#include <webserverhandler.h>
#include <WiFi.h>

WebServer server(443);

void handleRoot(){
    String response = "ESP32 Web Server";
    server.send(200, "text/plain", response);
}



void setupWebServer(){
    server.on("/", handleRoot);
    
    server.begin(); 
    
    Serial.println("HTTP server started on ");
    Serial.print(WiFi.localIP());
}

void handleWebServer(){
    server.handleClient();
}