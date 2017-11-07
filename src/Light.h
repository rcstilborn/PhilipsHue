/*
 * Light.h
 *
 *  Created on: Nov 6, 2017
 *      Author: richard
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include <string>
#include <deque>
#include <boost/property_tree/ptree.hpp>

class Light {
 public:
  Light(const std::string& id, const boost::property_tree::ptree& pt);
  virtual ~Light();

  void updateData(const boost::property_tree::ptree& pt);
  std::ostream& getUpdates(std::ostream& os);
  const std::string& getId() const { return id_; }
  const boost::property_tree::ptree& getData() const { return data_; }

 private:
  std::string id_;
  boost::property_tree::ptree data_;
  std::deque<std::string> updates_;

};

std::ostream& operator<<(std::ostream& os, const Light& l);

#endif /* LIGHT_H_ */
