/*
 * HTTPS_Client.cpp
 *
 *  Created on: Nov 4, 2017
 *      Author: richard
 */

#include "HTTPS_Client.h"

#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace http {

boost::asio::io_service HTTPS_Client::io_service_;
boost::asio::ssl::context HTTPS_Client::ctx_(boost::asio::ssl::context::sslv23);

HTTPS_Client::HTTPS_Client(const std::string& address)
    : address_(address),
      socket_(HTTPS_Client::io_service_, HTTPS_Client::ctx_) {
  if (address_.empty())
    throw std::invalid_argument("No address provided!");


  HTTPS_Client::ctx_.set_default_verify_paths();
//  HTTPS_Client::ctx_.set_verify_mode(boost::asio::ssl::context::verify_none);

  // Get a list of endpoints corresponding to the server name.
  boost::asio::ip::tcp::resolver resolver(HTTPS_Client::io_service_);
  boost::asio::ip::tcp::resolver::query query(address_, "https");
  boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
  // Try each endpoint until we successfully establish a connection.
  boost::asio::connect(socket_.lowest_layer(), endpoint_iterator);

//  socket_.set_verify_mode(boost::asio::ssl::verify_none);
//  socket_.set_verify_mode(boost::asio::ssl::verify_peer);
//  socket_.set_verify_callback(boost::asio::ssl::rfc2818_verification(address));
  socket_.handshake(boost::asio::ssl::stream_base::client);
}

HTTPS_Client::~HTTPS_Client() {
}

unsigned int HTTPS_Client::get(const std::string& path,
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

    return process_response(response);
}

unsigned int HTTPS_Client::post(const std::string& path,
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

    return process_response(response);
}

unsigned int HTTPS_Client::getJSON(const std::string& path,
                                  boost::property_tree::ptree& pt) {
  boost::asio::streambuf response;
  unsigned int result_code = get(path, response);
  if (result_code != 200) return result_code;
  std::istream response_stream(&response);
  boost::property_tree::read_json(response_stream, pt);
  return result_code;
}

unsigned int HTTPS_Client::postJSON(const std::string& path,
                                   const std::string& body,
                                   boost::property_tree::ptree& pt) {
  boost::asio::streambuf response;
  unsigned int result_code = post(path, body, response);
  if (result_code != 200) return result_code;
  std::istream response_stream(&response);
  boost::property_tree::read_json(response_stream, pt);
  return result_code;
}

unsigned int HTTPS_Client::process_response(boost::asio::streambuf& response) {
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

  // Also ignore SSL errors
  if (error && !(error == boost::asio::error::eof || error.category() == boost::asio::error::get_ssl_category()))
    throw boost::system::system_error(error);

  return status_code;
}

} /* namespace http */

