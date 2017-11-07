/*
 * HTTPS_Client.h
 *
 *  Created on: Nov 4, 2017
 *      Author: richard
 */

#ifndef HTTP_HTTPS_CLIENT_H_
#define HTTP_HTTPS_CLIENT_H_

#include <string>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/property_tree/ptree.hpp>
#include "HTTP_Client.h"

namespace http {

class HTTPS_Client {
 public:
  HTTPS_Client(const std::string& address);
  HTTPS_Client(const HTTPS_Client&) = delete;
  HTTPS_Client& operator= (const HTTPS_Client&) = delete;
  virtual ~HTTPS_Client();

  unsigned int get(const std::string& path,
                   boost::asio::streambuf& response);
  unsigned int post(const std::string& path,
                    const std::string& body,
                    boost::asio::streambuf& response);
  unsigned int getJSON(const std::string& path,
                       boost::property_tree::ptree& pt);
  unsigned int postJSON(const std::string& path,
                    const std::string& body,
                    boost::property_tree::ptree& pt);
 private:
  std::string address_;
  boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;

  static boost::asio::io_service io_service_;
  static boost::asio::ssl::context ctx_;

  unsigned int process_response(boost::asio::streambuf& response);
};

} /* namespace http */

#endif /* HTTP_HTTPS_CLIENT_H_ */
