/*
 * BridgeConnection.h
 *
 *  Created on: Nov 4, 2017
 *      Author: richard
 */

#ifndef BRIDGECONNECTION_H_
#define BRIDGECONNECTION_H_

#include <string>
#include <boost/property_tree/ptree.hpp>
#include "http/HTTP_Interface.h"

class BridgeConnection {
 public:
  BridgeConnection(const ::http::HTTP_Interface& interface, const std::string& ip_address, const std::string& username);
  virtual ~BridgeConnection();

  bool testConnection();
  bool registerNewUser();
  bool verifyUser();
  bool getLights(boost::property_tree::ptree& pt);

  const std::string& getIP_Address() const { return ip_address_; }
  const std::string& getUsername() const { return username_; }

 private:
  const ::http::HTTP_Interface& interface_;
  std::string ip_address_;
  std::string username_;
};

#endif /* BRIDGECONNECTION_H_ */
