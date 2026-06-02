#pragma once

#include <Arduino.h>

/**
 * @brief Initialize ESP32 as Access Point.
 *
 * @return true if AP started successfully.
 */
bool wifiManagerInit();

/**
 * @brief Get AP IP address.
 */
String wifiManagerGetIP();