#include "web_server.h"

#include <WebServer.h>

WebServer server(80);

WebServerManager webServerManager;

void WebServerManager::begin()
{
    server.on(
        "/",
        []()
        {
            server.send(
                200,
                "text/plain",
                "Energy Saver"
            );
        }
    );

    server.on(
        "/api/status",
        []()
        {
            server.send(
                200,
                "application/json",
                "{}"
            );
        }
    );

    server.begin();
}