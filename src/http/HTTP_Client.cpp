/*
 * HTTP_Client.cpp
 *
 *  Created on: Nov 4, 2017
 *      Author: richard
 */

#include "HTTP_Client.h"

#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace http {

boost::asio::io_service HTTP_Client::io_service_;

HTTP_Client::HTTP_Client(const std::string& address)
    : address_(address),
      socket_(HTTP_Client::io_service_) {
  if (address_.empty())
    throw std::invalid_argument("No address provided!");

  // Get a list of endpoints corresponding to the server name.
  boost::asio::ip::tcp::resolver resolver(HTTP_Client::io_service_);
  boost::asio::ip::tcp::resolver::query query(address_, "http");
  boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(
      query);
  // Try each endpoint until we successfully establish a connection.
  boost::asio::connect(socket_, endpoint_iterator);
}


//// For timed version of connect
//deadline_timer deadline_;
//deadline_.expires_from_now(timeout);
//boost::system::error_code ec = boost::asio::error::would_block;
//boost::asio::async_connect(socket_, endpoint_iterator, var(ec) = _1);
//do io_service_.run_one(); while (ec == boost::asio::error::would_block);
//if (ec || !socket_.is_open())
//  throw boost::system::system_error(
//      ec ? ec : boost::asio::error::operation_aborted);


HTTP_Client::~HTTP_Client() {
}


unsigned int HTTP_Client::get(const std::string& path,
                              boost::asio::streambuf& response) {
    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "GET " << path << " HTTP/1.0\r\n";
    request_stream << "Host: " << address_ << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    // Send the request.
    boost::asio::write(socket_, request);

    // Process the response
    return process_response(response);
}

unsigned int HTTP_Client::post(const std::string& path,
                               const std::string& body,
                               boost::asio::streambuf& response) {
    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "POST " << path << " HTTP/1.0\r\n";
    request_stream << "Host: " << address_ << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Content-Length: " <<  body.length() << "\r\n";
    request_stream << "Content-Type: application/x-www-form-urlencoded";
    request_stream << "Connection: close\r\n\r\n";
    request_stream << body;

    // Send the request.
    boost::asio::write(socket_, request);

    // Process the response
    return process_response(response);
}

unsigned int HTTP_Client::getJSON(const std::string& path,
                                  boost::property_tree::ptree& pt) {
  boost::asio::streambuf response;
  unsigned int result_code = get(path, response);
  if (result_code != 200) return result_code;
  std::istream response_stream(&response);

  boost::property_tree::read_json(response_stream, pt);
  return result_code;
}

unsigned int HTTP_Client::postJSON(const std::string& path,
                                   const std::string& body,
                                   boost::property_tree::ptree& pt) {
  boost::asio::streambuf response;
  unsigned int result_code = post(path, body, response);
  if (result_code != 200) return result_code;
  std::istream response_stream(&response);
  boost::property_tree::read_json(response_stream, pt);
  return result_code;
}

unsigned int HTTP_Client::process_response(boost::asio::streambuf& response) {
  // Read the response status line. The response streambuf will automatically
  // grow to accommodate the entire line. The growth may be limited by passing
  // a maximum size to the streambuf constructor.
  boost::asio::read_until(socket_, response, "\r\n");

  // Check that response is OK.
  std::istream response_stream(&response);
  std::string http_version;
  response_stream >> http_version;
  unsigned int status_code;
  response_stream >> status_code;
  std::string status_message;
  std::getline(response_stream, status_message);
  if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    return 1;

  if (!(status_code == 200 || status_code == 302))
    return status_code;

  // Read the response headers, which are terminated by a blank line.
  boost::asio::read_until(socket_, response, "\r\n\r\n");

  // Process the response headers.
  std::string header;
  while (std::getline(response_stream, header) && header != "\r") ;

  // Read until EOF, writing data to output as we go.
  boost::system::error_code error;
  boost::asio::read(socket_, response, error);

  if (error && error != boost::asio::error::eof)
    throw boost::system::system_error(error);

  return status_code;
}

} /* namespace http */
