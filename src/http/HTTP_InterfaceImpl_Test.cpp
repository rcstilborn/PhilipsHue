/*
 * HTTPInterfaceImpl_Test.cpp
 *
 *  Created on: Nov 6, 2017
 *      Author: richard
 */

#include "HTTP_InterfaceImpl.h"
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/test/unit_test.hpp>

namespace http {

static const std::string kStartOfHTMLDoc("<!doctype html>");
static const std::string kEndOfHTMLDoc("</html>");

BOOST_AUTO_TEST_SUITE (HTTP_InterfaceImpl_test)

BOOST_AUTO_TEST_CASE (GetNoAddress)
{
  HTTP_InterfaceImpl interface;
  boost::asio::streambuf response;
  BOOST_CHECK_THROW(interface.get("", "", response),std::invalid_argument);
}

BOOST_AUTO_TEST_CASE (GetBadHostName)
{
  HTTP_InterfaceImpl interface;
  boost::asio::streambuf response;
  BOOST_CHECK_THROW(interface.get("foo", "", response),std::exception);
}

BOOST_AUTO_TEST_CASE (GetNoAddressS)
{
  HTTP_InterfaceImpl interface;
  boost::asio::streambuf response;
  BOOST_CHECK_THROW(interface.getS("", "", response),std::invalid_argument);
}

BOOST_AUTO_TEST_CASE (GetBadHostNameS)
{
  HTTP_InterfaceImpl interface;
  boost::asio::streambuf response;
  BOOST_CHECK_THROW(interface.getS("foo", "", response),std::exception);
}

BOOST_AUTO_TEST_CASE (PostNoAddress)
{
  HTTP_InterfaceImpl interface;
  boost::asio::streambuf response;
  BOOST_CHECK_THROW(interface.post("", "", "", response),std::invalid_argument);
}

BOOST_AUTO_TEST_CASE (PostBadHostName)
{
  HTTP_InterfaceImpl interface;
  boost::asio::streambuf response;
  BOOST_CHECK_THROW(interface.post("foo", "", "", response),std::exception);
}

BOOST_AUTO_TEST_CASE (PostNoAddressS)
{
  HTTP_InterfaceImpl interface;
  boost::asio::streambuf response;
  BOOST_CHECK_THROW(interface.postS("", "", "", response),std::invalid_argument);
}

BOOST_AUTO_TEST_CASE (PostBadHostNameS)
{
  HTTP_InterfaceImpl interface;
  boost::asio::streambuf response;
  BOOST_CHECK_THROW(interface.postS("foo", "", "", response),std::exception);
}

BOOST_AUTO_TEST_CASE (GetPageSuccess)
{
  HTTP_InterfaceImpl interface;
  boost::asio::streambuf response;
  int x = interface.get("www.google.com", "/", response);
//  std::cout << "www.google.com/ RESPONSE:" << std::string(std::istreambuf_iterator<char>(&response), std::istreambuf_iterator<char>()) << std::endl;
  BOOST_CHECK_MESSAGE(x == 200,"Was expecting 200 but got " << x);
  boost::asio::streambuf::const_buffers_type bufs = response.data();
  std::string resp(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + response.size());
  BOOST_CHECK_MESSAGE(resp.compare(0,kStartOfHTMLDoc.size()+1,kStartOfHTMLDoc),"Response start is incorrect " << resp.substr(0,20));
  auto len = resp.size();
  BOOST_CHECK_MESSAGE(resp.compare(len-kEndOfHTMLDoc.size()-1,len,kEndOfHTMLDoc),"Response end is incorrect " << resp.substr(len-20,len));
}

BOOST_AUTO_TEST_CASE (GetPageSuccessS)
{
  HTTP_InterfaceImpl interface;
  boost::asio::streambuf response;
  int x = interface.getS("www.google.com", "/", response);
//  std::cout << "www.google.com/ RESPONSE:" << std::string(std::istreambuf_iterator<char>(&response), std::istreambuf_iterator<char>()) << std::endl;
  BOOST_CHECK_MESSAGE(x == 200,"Was expecting 200 but got " << x);
  boost::asio::streambuf::const_buffers_type bufs = response.data();
  std::string resp(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + response.size());
  BOOST_CHECK_MESSAGE(resp.compare(0,kStartOfHTMLDoc.size()+1,kStartOfHTMLDoc),"Response start is incorrect " << resp.substr(0,20));
  auto len = resp.size();
  BOOST_CHECK_MESSAGE(resp.compare(len-kEndOfHTMLDoc.size()-1,len,kEndOfHTMLDoc),"Response end is incorrect " << resp.substr(len-20,len));
}

BOOST_AUTO_TEST_CASE (httpbin_org_getJSON)
{
  HTTP_InterfaceImpl interface;
  boost::property_tree::ptree pt;
  int x = interface.getJSON("httpbin.org", "/get", pt);
  BOOST_CHECK_MESSAGE(x == 200,"Was expecting 200 but got " << x);
  BOOST_CHECK_MESSAGE(pt.get<std::string>("headers.Host") == "httpbin.org", "Expected \"httpbin.org\" but got " << pt.get<std::string>("headers.Host"));
//  boost::property_tree::write_json(std::cout, pt);
}

BOOST_AUTO_TEST_CASE (httpbin_org_postJSON)
{
  HTTP_InterfaceImpl interface;
  boost::property_tree::ptree pt;
  int x = interface.postJSON("httpbin.org", "/post", "{\"data\":\"foo bar\"}", pt);
  BOOST_CHECK_MESSAGE(x == 200,"Was expecting 200 but got " << x);
  BOOST_CHECK_MESSAGE(pt.get<std::string>("headers.Host") == "httpbin.org", "Expected \"httpbin.org\" but got " << pt.get<std::string>("headers.Host"));
}

BOOST_AUTO_TEST_CASE (GetJsonPagePhilipsHueNuPnP)
{
  HTTP_InterfaceImpl interface;
  boost::property_tree::ptree pt;
  int x = interface.getSJSON("www.meethue.com", "/api/nupnp", pt);
  BOOST_CHECK_MESSAGE(x == 200,"Was expecting 200 but got " << x);
//  boost::property_tree::write_json(std::cout, pt);
  for (auto& item : pt.get_child("")) {
//    boost::property_tree::write_json(std::cout, item.second);
    BOOST_CHECK_MESSAGE(!item.second.get<std::string>("internalipaddress").empty(), "Expected some data in internalipaddress");
  }
}

BOOST_AUTO_TEST_SUITE_END( )

} /* namespace http */
