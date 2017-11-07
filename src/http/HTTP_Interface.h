/*
 * HTTPInterface.h
 *
 *  Created on: Nov 6, 2017
 *      Author: richard
 */

#ifndef HTTP_HTTP_INTERFACE_H_
#define HTTP_HTTP_INTERFACE_H_

#include <string>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>

namespace http {

//enum class ConnectionType { Secure, Unsecure };

class HTTP_Interface {
 public:
//  HTTP_Interface();
  virtual ~HTTP_Interface() {}


  // Connect to hostname with http, send GET with path, return response as streambuf
  virtual unsigned int get(const std::string& hostname, const std::string& path,
                           boost::asio::streambuf& response) const = 0;

  // Connect to hostname with https, send GET with path, return response as streambuf
  virtual unsigned int getS(const std::string& hostname,
                            const std::string& path,
                            boost::asio::streambuf& response) const = 0;

  // Connect to hostname with http, send POST with path and body, return response as streambuf
  virtual unsigned int post(const std::string& hostname,
                            const std::string& path, const std::string& body,
                            boost::asio::streambuf& response) const = 0;

  // Connect to hostname with https, send POST with path and body, return response as streambuf
  virtual unsigned int postS(const std::string& hostname,
                             const std::string& path, const std::string& body,
                             boost::asio::streambuf& response) const = 0;

  // Connect to hostname with http, send GET with path, return response as propert_tree
  virtual unsigned int getJSON(const std::string& hostname,
                               const std::string& path,
                               boost::property_tree::ptree& pt) const = 0;

  // Connect to hostname with https, send GET with path, return response as propert_tree
  virtual unsigned int getSJSON(const std::string& hostname,
                                const std::string& path,
                                boost::property_tree::ptree& pt) const = 0;

  // Connect to hostname with http, send POST with path and body, return response as propert_tree
  virtual unsigned int postJSON(const std::string& hostname,
                                const std::string& path,
                                const std::string& body,
                                boost::property_tree::ptree& pt) const = 0;

  // Connect to hostname with https, send POST with path and body, return response as propert_tree
  virtual unsigned int postSJSON(const std::string& hostname,
                                 const std::string& path,
                                 const std::string& body,
                                 boost::property_tree::ptree& pt) const = 0;

};

} /* namespace http */

#endif /* HTTP_HTTP_INTERFACE_H_ */
