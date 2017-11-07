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
#include <boost/property_tree/json_parser.hpp>

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

void Light::getUpdates(std::ostream& os) {
  while(!updates_.empty()) {
    os << updates_.front() << "\n";
    updates_.pop_front();
  }
}

void Light::getFormattedOutput(std::ostream& os, const int indent) const
{
  std::string prefix(indent, ' ');

  os << prefix << "{\n";
  os << prefix << "  \"name\":\"" << data_.get("name","") << "\",\n";
  os << prefix << "  \"id\":\"" << id_ << "\",\n";
  os << prefix << "  \"on\":" << data_.get("state.on","") << ",\n";
  os << prefix << "  \"brightness\":" << data_.get("state.bri","") << "\n";
  os << prefix << "}";
}

std::ostream& operator<<(std::ostream& os, const Light& l)
{
  boost::property_tree::write_json(os, l.data_);
  return os;
}
