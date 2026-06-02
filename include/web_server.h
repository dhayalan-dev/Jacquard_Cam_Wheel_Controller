#pragma once

/**
 * @brief Initialize web server and start listening for clients.
 * @return true if initialization was successful, false otherwise.
 */
bool webServerInit();

/**
 * @brief Handle incoming client requests and update the web server state accordingly.
 * This function should be called regularly in the main loop to ensure the web server remains responsive.
 */
void webServerUpdate();