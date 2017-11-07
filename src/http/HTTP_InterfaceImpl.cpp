/*
 * HTTPInterfaceImpl.cpp
 *
 *  Created on: Nov 6, 2017
 *      Author: richard
 */

#include "HTTP_InterfaceImpl.h"
#include <string>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include "HTTP_Interface.h"
#include "HTTP_Client.h"
#include "HTTPS_Client.h"

namespace http {

HTTP_InterfaceImpl::HTTP_InterfaceImpl() {}

HTTP_InterfaceImpl::~HTTP_InterfaceImpl() {}

unsigned int HTTP_InterfaceImpl::get(const std::string& hostname,
                                     const std::string& path,
                                     boost::asio::streambuf& response) const {
    HTTP_Client client(hostname);
    try {
      return client.get(path, response);
    } catch (...) {
      throw "Could not retrieve data from  " + hostname;
    }
}

unsigned int HTTP_InterfaceImpl::getS(const std::string& hostname,
                                      const std::string& path,
                                      boost::asio::streambuf& response) const {
    HTTPS_Client client(hostname);
    try {
      return client.get(path, response);
    } catch (...) {
      throw "Could not retrieve data from  " + hostname;
    }
}

unsigned int HTTP_InterfaceImpl::post(const std::string& hostname,
                                      const std::string& path,
                                      const std::string& body,
                                      boost::asio::streambuf& response) const {
  HTTP_Client client(hostname);
  try {
    return client.post(path, body, response);
  } catch (...) {
    throw "Could not retrieve data from  " + hostname;
  }
}

unsigned int HTTP_InterfaceImpl::postS(const std::string& hostname,
                                      const std::string& path,
                                      const std::string& body,
                                      boost::asio::streambuf& response) const {
  HTTPS_Client client(hostname);
  try {
    return client.post(path, body, response);
  } catch (...) {
    throw "Could not retrieve data from  " + hostname;
  }
}

unsigned int HTTP_InterfaceImpl::getJSON(const std::string& hostname,
                                         const std::string& path,
                                         boost::property_tree::ptree& pt) const {
  HTTP_Client client(hostname);
  try {
    return client.getJSON(path, pt);
  } catch (...) {
    throw "Could not retrieve data from  " + hostname;
  }
}

unsigned int HTTP_InterfaceImpl::getSJSON(const std::string& hostname,
                                         const std::string& path,
                                         boost::property_tree::ptree& pt) const {
  HTTPS_Client client(hostname);
  try {
    return client.getJSON(path, pt);
  } catch (...) {
    throw "Could not retrieve data from  " + hostname;
  }
}

unsigned int HTTP_InterfaceImpl::postJSON(const std::string& hostname,
                                          const std::string& path,
                                          const std::string& body,
                                          boost::property_tree::ptree& pt) const {
  HTTP_Client client(hostname);
  try {
    return client.postJSON(path, body, pt);
  } catch (...) {
    throw "Could not retrieve data from  " + hostname;
  }
}

unsigned int HTTP_InterfaceImpl::postSJSON(const std::string& hostname,
                                          const std::string& path,
                                          const std::string& body,
                                          boost::property_tree::ptree& pt) const {
  HTTPS_Client client(hostname);
  try {
    return client.postJSON(path, body, pt);
  } catch (...) {
    throw "Could not retrieve data from  " + hostname;
  }
}

} /* namespace http */
