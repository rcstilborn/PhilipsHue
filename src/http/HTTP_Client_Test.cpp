/*
 * HTTP_Client_test.cpp
 *
 *  Created on: Nov 4, 2017
 *      Author: richard
 */

#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/test/unit_test.hpp>
#include "HTTP_Client.h"

namespace http {

static const std::string kStartOfHTMLDoc("<!doctype html>");
static const std::string kEndOfHTMLDoc("</html>");

BOOST_AUTO_TEST_SUITE (HTTP_Client_test)

BOOST_AUTO_TEST_CASE (NoAddress)
{
  BOOST_CHECK_THROW(HTTP_Client client(""),std::invalid_argument);
}

BOOST_AUTO_TEST_CASE (BadHostName)
{
  BOOST_CHECK_THROW(HTTP_Client client("foo"),std::exception);
}

// TODO (richard) Implement timeout for connect operation
//BOOST_AUTO_TEST_CASE (BadIPAddress)
//{
//  BOOST_CHECK_THROW(HTTP_Client client("192.168.5.5"),std::exception);
//}

BOOST_AUTO_TEST_CASE (GetPageSuccess)
{
  HTTP_Client client("www.google.com");
  boost::asio::streambuf response;
  int x = client.get("/", response);
//  std::cout << "www.google.com/ RESPONSE:" << std::string(std::istreambuf_iterator<char>(&response), std::istreambuf_iterator<char>()) << std::endl;
  BOOST_CHECK_MESSAGE(x == 200,"Was expecting 200 but got " << x);
  boost::asio::streambuf::const_buffers_type bufs = response.data();
  std::string resp(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + response.size());
  BOOST_CHECK_MESSAGE(resp.compare(0,kStartOfHTMLDoc.size()+1,kStartOfHTMLDoc),"Response start is incorrect " << resp.substr(0,20));
  auto len = resp.size();
  BOOST_CHECK_MESSAGE(resp.compare(len-kEndOfHTMLDoc.size()-1,len,kEndOfHTMLDoc),"Response end is incorrect " << resp.substr(len-20,len));
}

BOOST_AUTO_TEST_CASE (GetPageFailure404)
{
  HTTP_Client client("httpbin.org");
  boost::asio::streambuf response;
  int x = client.get("/status/404", response);
  BOOST_CHECK_MESSAGE(x == 404,"Was expecting 404 but got " << x);
}

BOOST_AUTO_TEST_CASE (GetRedirect302)
{
  HTTP_Client client("httpbin.org");
  boost::asio::streambuf response;
  int x = client.get("/status/302", response);
  BOOST_CHECK_MESSAGE(x == 302,"Was expecting 302 but got " << x);
}

BOOST_AUTO_TEST_CASE (PostPageFailure404)
{
  HTTP_Client client("httpbin.org");
  boost::asio::streambuf response;
  int x = client.post("/status/404", "", response);
  BOOST_CHECK_MESSAGE(x == 404,"Was expecting 404 but got " << x);
}

BOOST_AUTO_TEST_CASE (PostRedirect302)
{
  HTTP_Client client("httpbin.org");
  boost::asio::streambuf response;
  int x = client.post("/status/302", "", response);
  BOOST_CHECK_MESSAGE(x == 302,"Was expecting 302 but got " << x);
}

BOOST_AUTO_TEST_CASE (GetJSONPageFailure404)
{
  HTTP_Client client("httpbin.org");
  boost::property_tree::ptree pt;
  int x = client.getJSON("/status/404", pt);
  BOOST_CHECK_MESSAGE(x == 404,"Was expecting 404 but got " << x);
}

BOOST_AUTO_TEST_CASE (GetJSONRedirect302)
{
  HTTP_Client client("httpbin.org");
  boost::property_tree::ptree pt;
  int x = client.getJSON("/status/302", pt);
  BOOST_CHECK_MESSAGE(x == 302,"Was expecting 302 but got " << x);
}

BOOST_AUTO_TEST_CASE (PostJSONPageFailure404)
{
  HTTP_Client client("httpbin.org");
  boost::property_tree::ptree pt;
  int x = client.postJSON("/status/404", "", pt);
  BOOST_CHECK_MESSAGE(x == 404,"Was expecting 404 but got " << x);
}

BOOST_AUTO_TEST_CASE (PostJSONRedirect302)
{
  HTTP_Client client("httpbin.org");
  boost::property_tree::ptree pt;
  int x = client.postJSON("/status/302", "", pt);
  BOOST_CHECK_MESSAGE(x == 302,"Was expecting 302 but got " << x);
}

BOOST_AUTO_TEST_CASE (httpbin_org_getJSON)
{
  HTTP_Client client("httpbin.org");
  boost::property_tree::ptree pt;
  int x = client.getJSON("/get", pt);
  BOOST_CHECK_MESSAGE(x == 200,"Was expecting 200 but got " << x);
  BOOST_CHECK_MESSAGE(pt.get<std::string>("headers.Host") == "httpbin.org", "Expected \"httpbin.org\" but got " << pt.get<std::string>("headers.Host"));
//  boost::property_tree::write_json(std::cout, pt);
}

BOOST_AUTO_TEST_CASE (httpbin_org_postJSON)
{
  HTTP_Client client("httpbin.org");
  boost::property_tree::ptree pt;
  int x = client.postJSON("/post", "{\"data\":\"foo bar\"}", pt);
  BOOST_CHECK_MESSAGE(x == 200,"Was expecting 200 but got " << x);
  BOOST_CHECK_MESSAGE(pt.get<std::string>("headers.Host") == "httpbin.org", "Expected \"httpbin.org\" but got " << pt.get<std::string>("headers.Host"));
}

BOOST_AUTO_TEST_SUITE_END( )

} /* namespace http */
