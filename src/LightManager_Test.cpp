/*
 * LightManager_Test.cpp
 *
 *  Created on: Nov 6, 2017
 *      Author: richard
 */

#include "LightManager.h"

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/test/output_test_stream.hpp>

BOOST_AUTO_TEST_SUITE (LightManager_Test)

BOOST_AUTO_TEST_CASE (CTor) {
  LightManager lm;
}

BOOST_AUTO_TEST_CASE (LoadLights) {
  LightManager lm;
  {
    boost::property_tree::ptree pt;
    boost::property_tree::json_parser::read_json(
        "test_data/lights_initial.json", pt);
    lm.loadLights(pt);
  }
  BOOST_CHECK(lm.numberOfLights() == 2);
}

BOOST_AUTO_TEST_CASE (PrintLights) {
  LightManager lm;
  {
    boost::property_tree::ptree pt;
    boost::property_tree::json_parser::read_json("test_data/lights_initial.json", pt);
    lm.loadLights(pt);
  }
  boost::test_tools::output_test_stream output("test_data/lights_initial.expected", true );
  lm.getFormattedOutput(output);
//  std::cout << lm;
  BOOST_TEST(output.match_pattern());
}

BOOST_AUTO_TEST_CASE (UpdateDataOuput)
{
  LightManager lm;
  boost::property_tree::ptree pt;
  boost::property_tree::json_parser::read_json("test_data/lights_initial.json", pt);
  lm.loadLights(pt);

  boost::property_tree::json_parser::read_json("test_data/lights_white_off.json", pt);
  lm.updateLights(pt);
  boost::test_tools::output_test_stream output("test_data/lights_white_off.expected", true );
  lm.getUpdates(output);
  BOOST_TEST(output.match_pattern());
}

BOOST_AUTO_TEST_CASE (MultipleUpdateDataOuput)
{
  LightManager lm;
  boost::property_tree::ptree pt;
  boost::property_tree::json_parser::read_json("test_data/lights_initial.json", pt);
  lm.loadLights(pt);

  boost::property_tree::json_parser::read_json("test_data/lights_multiple_changes.json", pt);
  lm.updateLights(pt);
  boost::test_tools::output_test_stream output("test_data/lights_multiple_changes.expected", true );
  lm.getUpdates(output);
  BOOST_TEST(output.match_pattern());
}

BOOST_AUTO_TEST_CASE (NewDeletedChangedLight)
{
  LightManager lm;
  boost::property_tree::ptree pt;
  boost::property_tree::json_parser::read_json("test_data/lights_initial.json", pt);
  lm.loadLights(pt);

  boost::property_tree::json_parser::read_json("test_data/lights_new_deleted_changed.json", pt);
  lm.updateLights(pt);
  boost::test_tools::output_test_stream output("test_data/lights_new_deleted_changed.expected", true );
  lm.getUpdates(output);
  BOOST_CHECK(lm.numberOfLights() == 2);
  BOOST_TEST(output.match_pattern());
}


BOOST_AUTO_TEST_SUITE_END( )
