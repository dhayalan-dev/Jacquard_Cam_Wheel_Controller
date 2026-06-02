#include "wifi_manager.h"
#include <WiFi.h>


#define WIFI_AP_SSID    "JacquardController"
#define WIFI_AP_PASS    "Jack@4455"

bool wifiManagerInit()
{
    WiFi.mode(WIFI_AP);

    bool result = WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASS);

    if(!result)
    {
        Serial.println("Failed to start AP");
        return false;
    }

    Serial.print("AP IP : ");
    Serial.println(WiFi.softAPIP());

    return true;
}

String wifiManagerGetIP()
{
    return WiFi.softAPIP().toString();
}