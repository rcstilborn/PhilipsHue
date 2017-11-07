/*
 * Utils.cpp
 *
 *  Created on: Nov 7, 2017
 *      Author: richard
 */

#include <string>
#include <iostream>
#include <boost/property_tree/ini_parser.hpp>

#include "http/HTTP_Interface.h"

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

std::string discover_bridge(http::HTTP_Interface& interface) {
  boost::property_tree::ptree pt;

  // Query the website - throw if we can't connect
  try {
    if (interface.getSJSON("www.meethue.com", "/api/nupnp", pt) != 200)
      throw "Could not connect to www.meethue.com.  Please check your Internet connection and try again";
  } catch (...) {
    throw "Could not connect to www.meethue.com.  Please check your Internet connection try again";
  }
  // Loop through the results and return the first IP address we find
  for (auto& item : pt.get_child("")) {
    std::string ip = item.second.get<std::string>("internalipaddress", "");
    if (!ip.empty()) {
      return ip;
    }
  }
  // None found to so throw
  throw "No registered bridges with www.meethue.com.  Make sure your bridge is powered on and connected to the internet";
}

