/*
 * PhilipsHueMonitor_Test.cpp
 *
 *  Created on: Nov 4, 2017
 *      Author: richard
 */

#define BOOST_TEST_MODULE PhilipsHueMonitor_Test
#include <iostream>
#include <gmock/gmock.h>
#include <boost/test/included/unit_test.hpp>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "http/HTTP_InterfaceImpl.h"
#include "http/HTTPS_Client.h"
#include "http/HTTP_Client.h"
#include "BridgeConnection.h"


struct InitGMock {
    InitGMock() {
        ::testing::GTEST_FLAG(throw_on_failure) = true;
        ::testing::InitGoogleMock(&boost::unit_test::framework::master_test_suite().argc,
                                  boost::unit_test::framework::master_test_suite().argv);
    }
    ~InitGMock() { }
};
BOOST_GLOBAL_FIXTURE(InitGMock);

// This test suite is disabled because most testcases need to connect to to a live bridge!
BOOST_AUTO_TEST_SUITE (PhilipsHueMonitor_Test, * boost::unit_test::disabled())

const std::string kTestFileName = "testfilename.ini";

BOOST_AUTO_TEST_CASE (GetInfo)
{
  ::http::HTTP_Client client("192.168.1.116");
  boost::property_tree::ptree pt;
  int x = client.getJSON("/api", pt);
  BOOST_CHECK_MESSAGE(x == 200,"Was expecting 200 but got " << x);
//  boost::property_tree::write_json(std::cout, pt);
}

// Disabled because it needs the link button pressed to succeed
BOOST_AUTO_TEST_CASE (RequestNewUser)
{
  ::http::HTTP_Client client("192.168.1.116");
  boost::property_tree::ptree pt;
  int x = client.postJSON("/api", "{\"devicetype\":\"PhilipsHueMonitor\"}", pt);
  BOOST_CHECK_MESSAGE(x == 200,"Was expecting 200 but got " << x);
//  boost::property_tree::write_json(std::cout, pt);
}

BOOST_AUTO_TEST_CASE (GetConfigGoodUSer)
{
  ::http::HTTP_Client client("192.168.1.116");
  boost::property_tree::ptree pt;
  int x = client.getJSON("/api/Brw-6Jt9yIxcwrzdYH-S5hITbfxk8Iq93XxRYMa4/config", pt);
  BOOST_CHECK_MESSAGE(x == 200,"Was expecting 200 but got " << x);
//  boost::property_tree::write_json(std::cout, pt);
}

BOOST_AUTO_TEST_CASE (GetConfigBadUSer)
{
  ::http::HTTP_Client client("192.168.1.116");
  boost::property_tree::ptree pt;
  int x = client.getJSON("/api/Brw-6JtxcwrzdYH-S5hITbfxk8Iq93XxRYMa4/config", pt);
  BOOST_CHECK_MESSAGE(x == 200,"Was expecting 200 but got " << x);
//  boost::property_tree::write_json(std::cout, pt);
}

BOOST_AUTO_TEST_CASE (GetLights)
{
  ::http::HTTP_Client client("192.168.1.116");
  boost::property_tree::ptree pt;
  int x = client.getJSON("/api/Brw-6Jt9yIxcwrzdYH-S5hITbfxk8Iq93XxRYMa4/lights", pt);
  BOOST_CHECK_MESSAGE(x == 200,"Was expecting 200 but got " << x);
//  boost::property_tree::write_json(std::cout, pt);
}

//
//BOOST_AUTO_TEST_CASE (PostInfo)
//{
//  HTTP_Client client("192.168.1.116");
//  boost::asio::streambuf response;
//  int x = client.post("/api", "{\"devicetype\":\"my_hue_app#iphone peter\"}", response);
//  std::cout << "192.168.1.116/api RESPONSE:" << std::string(std::istreambuf_iterator<char>(&response), std::istreambuf_iterator<char>()) << std::endl;
//  BOOST_CHECK_MESSAGE(x == 200,"Was expecting 200 but got " << x);
//}

BOOST_AUTO_TEST_CASE (GetPagePhilipsHueNuPnP302)
{
  ::http::HTTP_Client client("www.meethue.com");
  boost::asio::streambuf response;
  int x = client.get("/api/nupnp", response);
  BOOST_CHECK_MESSAGE(x == 302,"Was expecting 302 but got " << x);
}

BOOST_AUTO_TEST_CASE (GetJsonPagePhilipsHueNuPnP)
{
  ::http::HTTPS_Client client("www.meethue.com");
  boost::property_tree::ptree pt;
  int x = client.getJSON("/api/nupnp", pt);
  BOOST_CHECK_MESSAGE(x == 200,"Was expecting 200 but got " << x);
//  boost::property_tree::write_json(std::cout, pt);
  for (auto& item : pt.get_child("")) {
//    boost::property_tree::write_json(std::cout, item.second);
    BOOST_CHECK_MESSAGE(!item.second.get<std::string>("internalipaddress").empty(), "Expected some data in internalipaddress");
  }
}

BOOST_AUTO_TEST_CASE (TestConnectionSuccess)
{
  ::http::HTTP_InterfaceImpl interface;
  BridgeConnection conn(interface, "192.168.1.116", "");
  BOOST_CHECK(conn.testConnection());
}

// Disabled due to long timeout
//BOOST_AUTO_TEST_CASE (TestConnectionFailure)
//{
//  ::http::HTTP_InterfaceImpl interface;
//  BridgeConnection conn(interface, "192.168.1.115", "");
//  BOOST_CHECK(!conn.testConnection());
//}

BOOST_AUTO_TEST_CASE (VerifyUserSuccess)
{
  ::http::HTTP_InterfaceImpl interface;
  BridgeConnection conn(interface, "192.168.1.116", "Brw-6Jt9yIxcwrzdYH-S5hITbfxk8Iq93XxRYMa4");
  BOOST_CHECK(conn.verifyUser());
}

BOOST_AUTO_TEST_CASE (VerifyUserFailure)
{
  ::http::HTTP_InterfaceImpl interface;
  BridgeConnection conn(interface, "192.168.1.116", "lhjlhjlhj");
  BOOST_CHECK(!conn.verifyUser());
}

// Requires user to press link button!
//BOOST_AUTO_TEST_CASE (RegisterNewUserSuccess)
//{
//  ::http::HTTP_InterfaceImpl interface;
//  BridgeConnection conn(interface, "192.168.1.116", "foo");
//  BOOST_CHECK(conn.registerNewUser());
//  BOOST_CHECK(conn.getUsername() != "foo");
//}

BOOST_AUTO_TEST_CASE (RegisterNewUserFailure)
{
  ::http::HTTP_InterfaceImpl interface;
  BridgeConnection conn(interface, "192.168.1.116", "foo");
  BOOST_CHECK(!conn.verifyUser());
  BOOST_CHECK(conn.getUsername() == "foo");
}

BOOST_AUTO_TEST_SUITE_END( )

