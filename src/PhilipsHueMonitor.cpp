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
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "BridgeConnection.h"
#include "http/HTTP_Client.h"
#include "http/HTTPS_Client.h"

void load_config(const std::string& filename, std::string& ip_address,
                 std::string& username) {
  boost::property_tree::ptree pt;

  try {
    boost::property_tree::ini_parser::read_ini("config.ini", pt);
    ip_address = pt.get<std::string>("ip_address");
  } catch (...) {  // Either the file doesn't exist or it doesn't have the expected entries so create a new one.
    pt.put("ip_address", "");
    pt.put("username", "");
    boost::property_tree::ini_parser::write_ini("config.ini", pt);
    ip_address = "";
    username = "";
  }
  try {
    username = pt.get<std::string>("username");
  } catch (...) {  // Still missing one of the entries.  So add it.
    pt.put("username", "");
    boost::property_tree::ini_parser::write_ini("config.ini", pt);
    username = "";
  }
}

std::string discover_bridge() {
  HTTPS_Client client("www.meethue.com");
  boost::property_tree::ptree pt;

  // Query the website - throw if we can't connect
  if (client.getJSON("/api/nupnp", pt) != 200)
    throw "Could not connect to www.meethue.com.  Please check your Internet connection";

  // Loop through the results and return the first IP address we find
  for (auto& item : pt.get_child("")) {
    std::string ip = item.second.get<std::string>("internalipaddress", "");
    if (!ip.empty()) {
      return ip;
    }
  }
  // None found to so throw
  throw "No registered bridges with www.nmeethue.com.  Make sure your bridge is powered on and connected to the internet";
}

int main() {
  std::cout << "Starting PhilipsHue Monitoring Service" << std::endl;

  try {
    std::string ip_address, username;

    load_config("config.ini", ip_address, username);

    if (ip_address.empty())
      ip_address = discover_bridge();

    BridgeConnection bridge(ip_address, username);

  } catch (const std::string& s) {
    std::cerr << s << std::endl;
    return -1;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
//  BridgeConnection bridge;

  return EXIT_SUCCESS;

//  try {
//    boost::asio::io_service io_service;
//
//
//    boost::asio::ip::tcp::endpoint ip_port(
//        boost::asio::ip::address::from_string("172.217.11.228"), 80);
//    boost::asio::ip::tcp::socket socket(io_service);
//    socket.connect(ip_port);
//
//  return EXIT_SUCCESS;
}
