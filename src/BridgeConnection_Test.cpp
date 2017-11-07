/*
 * BridgeConnection_Test.cpp
 *
 *  Created on: Nov 4, 2017
 *      Author: richard
 */

#include "BridgeConnection.h"

#include <iostream>
#include <gmock/gmock.h>
#include <boost/test/unit_test.hpp>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "http/HTTP_Interface.h"
#include "http/HTTP_InterfaceImpl.h"

#pragma GCC diagnostic ignored "-Weffc++"
class MockInterface : public ::http::HTTP_Interface {
public:
  MockInterface() { }
  virtual ~MockInterface() { }

  MOCK_CONST_METHOD3(get, unsigned int(const std::string& hostname, const std::string& path, boost::asio::streambuf& response));
  MOCK_CONST_METHOD3(getS, unsigned int(const std::string& hostname, const std::string& path, boost::asio::streambuf& response));
  MOCK_CONST_METHOD4(post, unsigned int(const std::string& hostname, const std::string& path, const std::string& body, boost::asio::streambuf& response));
  MOCK_CONST_METHOD4(postS, unsigned int(const std::string& hostname, const std::string& path, const std::string& body, boost::asio::streambuf& response));
  MOCK_CONST_METHOD3(getJSON, unsigned int(const std::string& hostname, const std::string& path, boost::property_tree::ptree& pt));
  MOCK_CONST_METHOD3(getSJSON, unsigned int(const std::string& hostname, const std::string& path, boost::property_tree::ptree& pt));
  MOCK_CONST_METHOD4(postJSON, unsigned int(const std::string& hostname, const std::string& path, const std::string& body, boost::property_tree::ptree& pt));
  MOCK_CONST_METHOD4(postSJSON, unsigned int(const std::string& hostname, const std::string& path, const std::string& body, boost::property_tree::ptree& pt));
};
#pragma GCC diagnostic pop

BOOST_AUTO_TEST_SUITE (BridgeConnection_Test)

BOOST_AUTO_TEST_CASE (CTor) {
  MockInterface mockInterface;
  BridgeConnection conn(mockInterface, "123", "me");
  BOOST_CHECK_MESSAGE(
      conn.getIP_Address() == "123",
      "Got incorrect IP address.  Expected \"123\" but got " << conn.getIP_Address());
  BOOST_CHECK_MESSAGE(
      conn.getUsername() == "me",
      "Got incorrect username.  Expected \"me\" but got " << conn.getUsername());
}

BOOST_AUTO_TEST_CASE (TestConnectionNoHost) {
  MockInterface mockInterface;
  EXPECT_CALL(mockInterface, getJSON("",::testing::_,::testing::_))
    .Times(1)
    .WillOnce(::testing::Throw(std::invalid_argument("No address provided!")));

  BridgeConnection conn(mockInterface, "", "");
  BOOST_CHECK(!conn.testConnection());
}

BOOST_AUTO_TEST_CASE (TestConnectionBadHost) {
  MockInterface mockInterface;
  EXPECT_CALL(mockInterface, getJSON("bad_hostname",::testing::_,::testing::_))
    .Times(1)
    .WillOnce(::testing::Throw(std::exception()));

  BridgeConnection conn(mockInterface, "bad_hostname", "");
  BOOST_CHECK(!conn.testConnection());
}

BOOST_AUTO_TEST_CASE (TestConnectionSuccess) {
  MockInterface mockInterface;
  EXPECT_CALL(mockInterface, getJSON("hostname",::testing::_,::testing::_))
    .Times(1)
    .WillOnce(::testing::Return(200));

  BridgeConnection conn(mockInterface, "hostname", "");
  BOOST_CHECK(conn.testConnection());
}

BOOST_AUTO_TEST_CASE (VerifyUserSuccess) {
  MockInterface mockInterface;
  boost::property_tree::ptree pt, child;
  child.put("bar","some data");
  pt.push_back(std::make_pair("whitelist", child));
  EXPECT_CALL(mockInterface, getJSON("hostname","/api/gooduser/config",::testing::_))
    .Times(1)
    .WillOnce(DoAll(::testing::SetArgReferee<2>(pt), ::testing::Return(200)));

  BridgeConnection conn(mockInterface, "hostname", "gooduser");
  BOOST_CHECK(conn.verifyUser());
}

BOOST_AUTO_TEST_CASE (VerifyUserFailure) {
  MockInterface mockInterface;
  boost::property_tree::ptree pt;
  pt.put("name", "Philips hue");
  EXPECT_CALL(mockInterface, getJSON("hostname","/api/baduser/config",::testing::_))
    .Times(1)
    .WillOnce(DoAll(::testing::SetArgReferee<2>(pt), ::testing::Return(200)));

  BridgeConnection conn(mockInterface, "hostname", "baduser");
  BOOST_CHECK(!conn.verifyUser());
}

BOOST_AUTO_TEST_CASE (RegisterNewUserSuccess) {
  MockInterface mockInterface;
  boost::property_tree::ptree pt, toplevel, success;
  success.put("username","new username");
  toplevel.push_back(std::make_pair("success", success));
  pt.push_back(std::make_pair("", toplevel));
  EXPECT_CALL(mockInterface, postJSON("hostname","/api/","{\"devicetype\":\"PhilipsHueMonitor\"}",::testing::_))
    .Times(1)
    .WillOnce(DoAll(::testing::SetArgReferee<3>(pt), ::testing::Return(200)));

  BridgeConnection conn(mockInterface, "hostname", "old username");
  BOOST_CHECK(conn.registerNewUser());
  BOOST_CHECK(conn.getUsername() == "new username");
}

BOOST_AUTO_TEST_CASE (RegisterNewUserFailure) {
  MockInterface mockInterface;
  boost::property_tree::ptree pt, toplevel, error;
  error.put("type","101");
  toplevel.push_back(std::make_pair("error", error));
  pt.push_back(std::make_pair("", toplevel));
  EXPECT_CALL(mockInterface, postJSON("hostname","/api/","{\"devicetype\":\"PhilipsHueMonitor\"}",::testing::_))
    .Times(1)
    .WillOnce(DoAll(::testing::SetArgReferee<3>(pt), ::testing::Return(200)));

  BridgeConnection conn(mockInterface, "hostname", "old username");
  BOOST_CHECK(!conn.registerNewUser());
  BOOST_CHECK(conn.getUsername() == "old username");
}

BOOST_AUTO_TEST_CASE (GetLights) {
  MockInterface mockInterface;
  boost::property_tree::ptree pt, light1, light2, state1, state2;
  state1.put("on","true");
  state1.put("bri",254);
  light1.push_back(std::make_pair("state", state1));
  light1.put("uniqueid","00:17:88:01:02:94:3f:85-0b");
  state2.put("on","false");
  state2.put("bri",254);
  light2.push_back(std::make_pair("state", state2));
  light2.put("uniqueid","00:17:88:01:02:dd:65:2f-0b");
  pt.push_back(std::make_pair("1", light1));
  pt.push_back(std::make_pair("2", light2));
  EXPECT_CALL(mockInterface, getJSON("hostname","/api/username/lights",::testing::_))
    .Times(1)
    .WillOnce(DoAll(::testing::SetArgReferee<2>(pt), ::testing::Return(200)));

  boost::property_tree::ptree result;
  BridgeConnection conn(mockInterface, "hostname", "username");
  BOOST_CHECK(conn.getLights(result));
  BOOST_CHECK(pt == result);
}

BOOST_AUTO_TEST_SUITE_END( )
