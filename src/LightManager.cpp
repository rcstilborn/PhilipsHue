/*
 * LightManager.cpp
 *
 *  Created on: Nov 6, 2017
 *      Author: richard
 */

#include "LightManager.h"
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <boost/property_tree/ptree.hpp>
#include "Light.h"

LightManager::LightManager()
    : lights_(), updates_() {
}

LightManager::~LightManager() {
}

void LightManager::loadLights(const boost::property_tree::ptree& pt) {
  for (auto& item : pt.get_child(""))
    lights_.emplace(std::make_pair(item.first, Light(item.first, item.second)));
}

void LightManager::updateLights(const boost::property_tree::ptree& pt) {
  // Get all the current lights as a set if IDs
  std::set<std::string> updateSet;
  for(const auto& item : lights_)
    updateSet.insert(item.first);

  // Then update / create the ones we get.
  for (auto& item : pt.get_child("")) {
    auto search = lights_.find(item.first);
    if (search != lights_.end())
      search->second.updateData(item.second);
    else {
      auto res = lights_.emplace(std::make_pair(item.first, Light(item.first, item.second)));
      std::stringstream ss;
      res.first->second.getFormattedOutput(ss);
      updates_.emplace_back(ss.str());
    }
    updateSet.erase(item.first);
  }

  // Remove the ones we didn't update
  for (const auto& id : updateSet) {
    lights_.erase(id);
    updates_.emplace_back("Deleted light " + id);
  }
}

void LightManager::getFormattedOutput(std::ostream& os, const int indent) const
{
  std::string prefix(indent, ' ');
  std::map<std::string, Light>::const_iterator it = lights_.cbegin();
  os << prefix << "[\n";

  while (it != lights_.cend()) {
    it->second.getFormattedOutput(os, indent+2);
    it++;
    if (it != lights_.cend()) os << ",";
    os << "\n";
  }
  os << prefix << "]\n";
}

void LightManager::getUpdates(std::ostream& os) {
  for(auto& item : lights_)
    item.second.getUpdates(os);
  while(!updates_.empty()) {
    os << updates_.front() << "\n";
    updates_.pop_front();
  }
}

std::ostream& operator<<(std::ostream& os, const LightManager& lm) {
  for(const auto& item : lm.lights_)
    os << item.second;
  return os;
}
