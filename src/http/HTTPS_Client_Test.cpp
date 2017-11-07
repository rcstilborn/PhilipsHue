/*
 * HTTPS_Client_Test.cpp
 *
 *  Created on: Nov 4, 2017
 *      Author: richard
 */

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include "HTTPS_Client.h"

namespace http {

static const std::string kStartOfHTMLDoc("<!doctype html>");
static const std::string kEndOfHTMLDoc("</html>");

BOOST_AUTO_TEST_SUITE (HTTPS_Client_test)

BOOST_AUTO_TEST_CASE (NoAddress)
{
  BOOST_CHECK_THROW(HTTPS_Client client(""),std::invalid_argument);
}

BOOST_AUTO_TEST_CASE (BadHostName)
{
  BOOST_CHECK_THROW(HTTPS_Client client("foo"),std::exception);
}

// TODO (richard) Implement timeout for connect operation
//BOOST_AUTO_TEST_CASE (BadIPAddress)
//{
//  BOOST_CHECK_THROW(HTTP_Client client("192.168.5.5"),std::exception);
//}

BOOST_AUTO_TEST_CASE (GetPageSuccess)
{
  HTTPS_Client client("www.google.com");
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

// TODO (richard) There is a SSL issue with this server.
// SSL works for google.com AND meethue.com but not httpbin.org
//BOOST_AUTO_TEST_CASE (GetPageFailure404)
//{
//  HTTPS_Client client("httpbin.org");
//  boost::asio::streambuf response;
//  int x = client.get("/status/404", response);
//  BOOST_CHECK_MESSAGE(x == 404,"Was expecting 404 but got " << x);
//}
//
//BOOST_AUTO_TEST_CASE (GetRedirect302)
//{
//  HTTPS_Client client("httpbin.org");
//  boost::asio::streambuf response;
//  int x = client.get("/status/302", response);
//  BOOST_CHECK_MESSAGE(x == 302,"Was expecting 302 but got " << x);
//}
//
//BOOST_AUTO_TEST_CASE (PostPageFailure404)
//{
//  HTTPS_Client client("httpbin.org");
//  boost::asio::streambuf response;
//  int x = client.post("/status/404", "", response);
//  BOOST_CHECK_MESSAGE(x == 404,"Was expecting 404 but got " << x);
//}
//
//BOOST_AUTO_TEST_CASE (PostRedirect302)
//{
//  HTTPS_Client client("httpbin.org");
//  boost::asio::streambuf response;
//  int x = client.post("/status/302", "", response);
//  BOOST_CHECK_MESSAGE(x == 302,"Was expecting 302 but got " << x);
//}

// Other potential test sites
//http://posttestserver.com/post.php?dump&html&dir=henry&status_code=202
//https://requestb.in/1hnuta21

} /* namespace http */


BOOST_AUTO_TEST_SUITE_END( )
