/*
 * Light_Test.cpp
 *
 *  Created on: Nov 6, 2017
 *      Author: richard
 */

#include "Light.h"

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/test/output_test_stream.hpp>


BOOST_AUTO_TEST_SUITE (Light_Test)

BOOST_AUTO_TEST_CASE (CTor)
{
  boost::property_tree::ptree pt;
  boost::property_tree::json_parser::read_json("test_data/light_white_on.json", pt);
  Light light("1",pt);
  BOOST_CHECK(light.getId() == "1");
  BOOST_CHECK(light.getData() == pt);
}

BOOST_AUTO_TEST_CASE(OStreamOperator)
{
  boost::property_tree::ptree pt;
  boost::property_tree::json_parser::read_json("test_data/light_white_on.json", pt);
  Light light("1",pt);

  boost::test_tools::output_test_stream output("test_data/light_white_on.expected", true );
  light.getFormattedOutput(output,2);
  BOOST_TEST(output.match_pattern());
}

BOOST_AUTO_TEST_CASE (UpdateData)
{
  boost::property_tree::ptree pt;
  boost::property_tree::json_parser::read_json("test_data/light_white_on.json", pt);
  Light light("1",pt);
  boost::property_tree::json_parser::read_json("test_data/light_white_off.json", pt);
  light.updateData(pt);
  BOOST_CHECK(light.getId() == "1");
  BOOST_CHECK(light.getData() == pt);
}

BOOST_AUTO_TEST_CASE (UpdateDataOuput)
{
  boost::property_tree::ptree pt;
  boost::property_tree::json_parser::read_json("test_data/light_white_on.json", pt);
  Light light("1",pt);
  boost::property_tree::json_parser::read_json("test_data/light_white_off.json", pt);
  light.updateData(pt);
  boost::test_tools::output_test_stream output("test_data/light_white_off.expected", true );
  light.getUpdates(output);
  BOOST_TEST(output.match_pattern());
}

BOOST_AUTO_TEST_CASE (TwoUpdateDataOuput)
{
  boost::property_tree::ptree pt;
  boost::property_tree::json_parser::read_json("test_data/light_white_off.json", pt);
  Light light("1",pt);
  boost::property_tree::json_parser::read_json("test_data/light_white_on_brightness_down.json", pt);
  light.updateData(pt);
  boost::test_tools::output_test_stream output("test_data/light_white_on_brightness_down.expected", true );
  light.getUpdates(output);
  BOOST_TEST(output.match_pattern());
}

BOOST_AUTO_TEST_SUITE_END( )
