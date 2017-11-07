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


BOOST_AUTO_TEST_SUITE (LightManager_Test)

BOOST_AUTO_TEST_CASE (CTor)
{
  LightManager lm;
}

BOOST_AUTO_TEST_CASE (LoadLights)
{
  boost::property_tree::ptree pt;
  boost::property_tree::json_parser::read_json("test_data/lights_initial.json", pt);
  LightManager lm;
//  lm.initialLoad(pt);
//  boost::property_tree::write_json(std::cout, pt);
}


BOOST_AUTO_TEST_SUITE_END( )
