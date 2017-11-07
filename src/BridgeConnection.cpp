/*
 * BridgeConnection.cpp
 *
 *  Created on: Nov 4, 2017
 *      Author: richard
 */

#include "BridgeConnection.h"

#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "http/HTTP_Client.h"

BridgeConnection::BridgeConnection(const ::http::HTTP_Interface& interface,
                                   const std::string& ip_address,
                                   const std::string& username)
    : interface_(interface),
      ip_address_(ip_address),
      username_(username) {
}

BridgeConnection::~BridgeConnection() {
  // TODO Auto-generated destructor stub
}

bool BridgeConnection::testConnection() {
  boost::property_tree::ptree pt;
  try {
    unsigned int x = interface_.getJSON(ip_address_, "/api", pt);
    if (x != 200)
      return false;
  } catch (...) {
    return false;
  }
  return true;
}

bool BridgeConnection::registerNewUser() {
  // If we don't have a username then we need to register a new one
  // We assume the calling function has requested the user to press the link button
  boost::property_tree::ptree pt;
  try {
    unsigned int x = interface_.postJSON(
        ip_address_, "/api/", "{\"devicetype\":\"PhilipsHueMonitor\"}", pt);
    if (x != 200)
      return false;
    if (!pt.get("success.username","").empty()) {
      username_ = pt.get("success.username","");
      return true;
    } else if (!pt.get(".success.username","").empty()) {
      username_ = pt.get(".success.username","");
      return true;
    }
  } catch (...) {
    return false;
  }
  return false;
}

bool BridgeConnection::verifyUser() {
  try {
    boost::property_tree::ptree pt;
    unsigned int x = interface_.getJSON(ip_address_,
                                        "/api/" + username_ + "/config", pt);
    if (x != 200)
      return false;
//    boost::property_tree::write_json(std::cout, pt);
    if (pt.count("whitelist") == 0)
      return false;
  } catch (...) {
    return false;
  }

  return true;
}

bool BridgeConnection::getLights(boost::property_tree::ptree& pt) {
  try {
    unsigned int x = interface_.getJSON(ip_address_,
                                        "/api/" + username_ + "/lights", pt);
    if (x != 200)
      return false;
//    boost::property_tree::write_json(std::cout, pt);
//    if (pt.count("whitelist") == 0)
//      return false;
  } catch (...) {
    return false;
  }

  return true;
}
