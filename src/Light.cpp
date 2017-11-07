/*
 * Light.cpp
 *
 *  Created on: Nov 6, 2017
 *      Author: richard
 */

#include "Light.h"
#include <string>
#include <ostream>
#include <boost/property_tree/ptree.hpp>

Light::Light(const std::string& id, const boost::property_tree::ptree& pt)
: id_(id), data_(pt), updates_() {
}

Light::~Light() {
}

void Light::updateData(const boost::property_tree::ptree& pt) {
  // check if the on/off or brightness have changed.
  if (data_.get("state.on","") != pt.get("state.on",""))
    updates_.emplace_back("{\n  \"id\":\""+id_+"\",\n  \"on\":"+pt.get("state.on","")+"\n}");
  if (data_.get("state.bri","") != pt.get("state.bri",""))
    updates_.emplace_back("{\n  \"id\":\""+id_+"\",\n  \"brightness\":"+pt.get("state.bri","")+"\n}");
  data_ = pt;
}

std::ostream& Light::getUpdates(std::ostream& os) {
  while(!updates_.empty()) {
    os << updates_.front() << "\n";
    updates_.pop_front();
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const Light& l)
{
  os << "  {\n";
  os << "    \"name\":\"" << l.getData().get("name","") << "\",\n";
  os << "    \"id\":\"" << l.getId() << "\",\n";
  os << "    \"on\":" << l.getData().get("state.on","") << ",\n";
  os << "    \"brightness\":" << l.getData().get("state.bri","") << "\n";
  os << "  }";
  return os;
}
