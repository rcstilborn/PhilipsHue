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
#include <istream>
#include <ostream>
#include <string>
#include <chrono>
#include <thread>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "http/HTTP_InterfaceImpl.h"
#include "BridgeConnection.h"
#include "LightManager.h"

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

void load_config(std::string& ip_address, std::string& username,
                 const std::string& filename = "config.ini") {
  boost::property_tree::ptree pt;

  try {
    boost::property_tree::ini_parser::read_ini(filename, pt);
    ip_address = pt.get<std::string>("ip_address");
  } catch (...) {  // Either the file doesn't exist or it doesn't have the expected entries so create a new one.
    pt.put("ip_address", "");
    pt.put("username", "");
    boost::property_tree::ini_parser::write_ini(filename, pt);
    ip_address = "";
    username = "";
  }
  try {
    username = pt.get<std::string>("username");
  } catch (...) {  // Still missing one of the entries.  So add it.
    pt.put("username", "");
    boost::property_tree::ini_parser::write_ini(filename, pt);
    username = "";
  }
}

void save_config(const std::string& ip_address, const std::string& username,
                 const std::string& filename = "config.ini") {
  boost::property_tree::ptree pt;
  pt.put("ip_address", ip_address);
  pt.put("username", username);
  try {
    boost::property_tree::ini_parser::write_ini(filename, pt);
  } catch (...) {
    std::cerr << "Could not write configuration file " << filename << std::endl;
  }
}

bool isValidIpAddress(const std::string& ip) {
  boost::system::error_code ec;
  boost::asio::ip::address::from_string(ip, ec);
  if (ec) return false;
  return true;
}

//std::string discover_bridge(http::HTTP_Interface& interface) {
//  boost::property_tree::ptree pt;
//
//  // Query the website - throw if we can't connect
//  if (interface.getJSON("www.meethue.com", "/api/nupnp", pt) != 200)
//    throw "Could not connect to www.meethue.com.  Please check your Internet connection";
//
//  // Loop through the results and return the first IP address we find
//  for (auto& item : pt.get_child("")) {
//    std::string ip = item.second.get<std::string>("internalipaddress", "");
//    if (!ip.empty()) {
//      return ip;
//    }
//  }
//  // None found to so throw
//  throw "No registered bridges with www.meethue.com.  Make sure your bridge is powered on and connected to the internet";
//}

int main(int argc, char *argv[]) {

  if (argc > 2) {
    std::cerr << "Incorrect arguments." << std::endl;
    print_usage();
    return EXIT_FAILURE;
  }

  if (argc == 2 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")) {
      print_usage();
      return EXIT_SUCCESS;
  }

  std::string ip_address, username;
  if (argc == 2 && isValidIpAddress(argv[1]))
    ip_address = argv[1];
  else
    load_config(ip_address, username);

  if(ip_address.empty()) {
    // Do discovery!

    std::cerr << "\nCould not discover bridge. Please make sure it is switch on and connected to the network\n" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Attempting to connect to " << ip_address << " ... ";
  http::HTTP_InterfaceImpl interface;
  BridgeConnection bridge(interface, ip_address, username);
  if (!bridge.testConnection()) {
    std::cerr << "\nCould not connect to bridge with address " << ip_address
              << "\nPlease make sure the bridge is switch on and connected to the network\n";
    return EXIT_FAILURE;
  }
  std::cout << "Connected."  << std::endl;

  if (!username.empty() && bridge.verifyUser()) {
    std::cout << "Username verified." << std::endl;
  } else {
    std::cout << "Need to register with bridge.  Press the link button now." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    if (!bridge.registerNewUser()) {
        std::cerr << "Could not register new user.  Please try again." << std::endl;
        return EXIT_FAILURE;
    }
  }
  username = bridge.getUsername();
  save_config(ip_address, username);

  LightManager lm;
  boost::property_tree::ptree pt;
  if(!bridge.getLights(pt)) {
    std::cerr << "Could not retrieve data from bridge.  Please try again." << std::endl;
    return EXIT_FAILURE;
  }
  lm.loadLights(pt);
  lm.getFormattedOutput(std::cout);

  while(true){
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    if(!bridge.getLights(pt)) {
      std::cerr << "Could not retrieve data from bridge.  Please try again." << std::endl;
      return EXIT_FAILURE;
    }
    lm.updateLights(pt);
    lm.getUpdates(std::cout);
  }

  return EXIT_SUCCESS;
}
