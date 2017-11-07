/*
 * LightManager.h
 *
 *  Created on: Nov 6, 2017
 *      Author: richard
 */

#ifndef LIGHTMANAGER_H_
#define LIGHTMANAGER_H_

#include <map>
#include <deque>
#include <string>
#include <ostream>
#include <boost/property_tree/ptree.hpp>
#include "Light.h"

class LightManager {
 public:
  LightManager();
  virtual ~LightManager();

  unsigned int numberOfLights() { return lights_.size(); }
  void loadLights(const boost::property_tree::ptree& pt);
  void getFormattedOutput(std::ostream& os, const int indent = 0) const;
  void updateLights(const boost::property_tree::ptree& pt);
  void getUpdates(std::ostream& os);

 private:
  std::map<std::string,Light> lights_;
  std::deque<std::string> updates_;

  friend std::ostream& operator<<(std::ostream& os, const LightManager& lm);
};



#endif /* SRC_LIGHTMANAGER_H_ */
