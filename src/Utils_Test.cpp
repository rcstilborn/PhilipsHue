/*
 * Utils_Test.cpp
 *
 *  Created on: Nov 7, 2017
 *      Author: richard
 */

#include "Utils.h"

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

BOOST_AUTO_TEST_SUITE (Utils_Test)

BOOST_AUTO_TEST_CASE (DiscoverBridgeNetworkInterfaceThows) {
  MockInterface mockInterface;
  std::string ip;
  EXPECT_CALL(mockInterface, getSJSON("www.meethue.com", "/api/nupnp",::testing::_))
    .Times(1)
    .WillOnce(::testing::Throw(std::exception()));
  BOOST_CHECK_THROW(ip = discover_bridge(mockInterface), const char*);
}

BOOST_AUTO_TEST_CASE (DiscoverBridgeNetworkInterfaceReturns404) {
  MockInterface mockInterface;
  std::string ip;
  EXPECT_CALL(mockInterface, getSJSON("www.meethue.com", "/api/nupnp",::testing::_))
    .Times(1)
    .WillOnce(::testing::Return(404));
  BOOST_CHECK_THROW(ip = discover_bridge(mockInterface), const char*);
}

BOOST_AUTO_TEST_CASE (DiscoverBridgeNetworkInterfaceReturnsNoBridges) {
  MockInterface mockInterface;
  std::string ip;
  EXPECT_CALL(mockInterface, getSJSON("www.meethue.com", "/api/nupnp",::testing::_))
    .Times(1)
    .WillOnce(::testing::Return(200));
  BOOST_CHECK_THROW(ip = discover_bridge(mockInterface), const char*);
}

BOOST_AUTO_TEST_CASE (DiscoverBridgeNetworkInterfaceReturnsOneBridge) {
  MockInterface mockInterface;
  boost::property_tree::ptree pt;
  std::stringstream ss;
  // Output from: wget -qO - www.meethue.com/api/nupnp
  ss << "[{\"id\":\"001788fffe728ded\",\"internalipaddress\":\"192.168.1.116\"}]";
  boost::property_tree::read_json(ss,pt);
  EXPECT_CALL(mockInterface, getSJSON("www.meethue.com", "/api/nupnp",::testing::_))
    .Times(1)
    .WillOnce(DoAll(::testing::SetArgReferee<2>(pt), ::testing::Return(200)));
  BOOST_CHECK(discover_bridge(mockInterface) == "192.168.1.116");
}

BOOST_AUTO_TEST_SUITE_END( )
