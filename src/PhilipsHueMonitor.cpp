//============================================================================
// Name        : PhilipsHue.cpp
// Author      : Richard Stilborn
// Version     :
// Copyright   : Your copyright notice
// Description : Command line tool to observe Philips Hue lights
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>

#include "http/HTTP_InterfaceImpl.h"
#include "BridgeConnection.h"
#include "LightManager.h"
#include "Utils.h"

void print_usage() {
  std::cout
      << "Usage: PhilipsHueMonitor -h | --help   : Print usage\n"
      "       PhilipsHueMonitor <IP address>  : Uses provided address to connect to bridge.\n"
      "                                         Saves in config.ini if successful\n"
      "       PhilipsHueMonitor               : Uses address in config.ini if provided and\n"
      "                                         successful otherwise tries to discover bridge via:\n"
      "                                         1. uPnP (note: not yet implemented!!)\n"
      "                                         2. www.meethue.com/api/nupnp\n"
      "\n"
      "       Once connected it prints the current lights and polls the bridge every second to detect changes.\n"
      << std::endl;
}

int main(int argc, char *argv[]) {

  if (argc > 2) {
    std::cerr << "PhilipsHueMonitor: Incorrect arguments.\n"
              "Try 'PhilipsHueMonitor --help' for more information"
              << std::endl;
    return EXIT_FAILURE;
  }

  if (argc == 2
      && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")) {
    print_usage();
    return EXIT_SUCCESS;
  }

  std::string ip_address, username;
  http::HTTP_InterfaceImpl interface;

  if (argc == 2) {
    if (isValidIpAddress(argv[1])) {
      ip_address = argv[1];
    } else {
      std::cerr << "'" << std::string(argv[1])
                << "' is not a valid IP Address.\n"
                "Try 'PhilipsHueMonitor --help' for more information"
                << std::endl;
      return EXIT_FAILURE;
    }
  } else {
    std::cout << "Loading configuration from file" << std::endl;
    load_config(ip_address, username);
  }
  if (ip_address.empty()) {
    // Do discovery!
    try {
      ip_address = discover_bridge(interface);
    } catch (const char* c) {
      std::cerr << c << std::endl;
      return EXIT_FAILURE;
    }
  }

  std::cout << "Attempting to connect to " << ip_address << " ... ";
  BridgeConnection bridge(interface, ip_address, username);
  if (!bridge.testConnection()) {
    std::cerr
        << "\nCould not connect to bridge with address "
        << ip_address
        << "\nPlease make sure the bridge is switch on and connected to the network\n";
    return EXIT_FAILURE;
  }
  std::cout << "Connected." << std::endl;

  if (!username.empty() && bridge.verifyUser()) {
    std::cout << "Username verified." << std::endl;
  } else {
    std::cout << "Need to register with bridge.  Press the link button now."
              << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    if (!bridge.registerNewUser()) {
      std::cerr << "Could not register new user.  Please try again."
                << std::endl;
      return EXIT_FAILURE;
    }
  }
  username = bridge.getUsername();
  save_config(ip_address, username);

  LightManager lm;
  boost::property_tree::ptree pt;
  if (!bridge.getLights(pt)) {
    std::cerr << "Could not retrieve data from bridge.  Please try again."
              << std::endl;
    return EXIT_FAILURE;
  }
  lm.loadLights(pt);
  lm.getFormattedOutput(std::cout);

  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    if (!bridge.getLights(pt)) {
      std::cerr << "Could not retrieve data from bridge.  Please try again."
                << std::endl;
      return EXIT_FAILURE;
    }
    lm.updateLights(pt);
    lm.getUpdates(std::cout);
  }

  return EXIT_SUCCESS;
}
