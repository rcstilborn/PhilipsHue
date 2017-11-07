/*
 * Utils.h
 *
 *  Created on: Nov 7, 2017
 *      Author: richard
 */

#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#include <string>
#include "http/HTTP_Interface.h"

void load_config(std::string& ip_address, std::string& username,
                 const std::string& filename = "config.ini");

void save_config(const std::string& ip_address, const std::string& username,
                 const std::string& filename = "config.ini");

bool isValidIpAddress(const std::string& ip);

std::string discover_bridge(http::HTTP_Interface& interface);

#endif /* SRC_UTILS_H_ */
