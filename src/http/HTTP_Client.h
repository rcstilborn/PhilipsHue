/*
 * HTTP_Client.h
 *
 *  Created on: Nov 4, 2017
 *      Author: richard
 */

#ifndef HTTP_HTTP_CLIENT_H_
#define HTTP_HTTP_CLIENT_H_

#include <string>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>

namespace http {

class HTTP_Client {
 public:
  HTTP_Client(const std::string& address);
  HTTP_Client(const HTTP_Client&) = delete;
  HTTP_Client& operator= (const HTTP_Client&) = delete;
  virtual ~HTTP_Client();

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
  boost::asio::ip::tcp::socket socket_;

  static boost::asio::io_service io_service_;

  unsigned int process_response(boost::asio::streambuf& response);
};

} /* namespace http */

#endif /* HTTP_HTTP_CLIENT_H_ */
