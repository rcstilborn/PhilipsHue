/*
 * HTTPInterfaceImpl.h
 *
 *  Created on: Nov 6, 2017
 *      Author: richard
 */

#ifndef HTTP_HTTP_INTERFACEIMPL_H_
#define HTTP_HTTP_INTERFACEIMPL_H_

#include "HTTP_Interface.h"
#include <string>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>

namespace http {

class HTTP_InterfaceImpl : public HTTP_Interface {
 public:
  HTTP_InterfaceImpl();
  virtual ~HTTP_InterfaceImpl();

  unsigned int get(const std::string& hostname,
                   const std::string& path,
                   boost::asio::streambuf& response) const override;
  unsigned int getS(const std::string& hostname,
                    const std::string& path,
                    boost::asio::streambuf& response) const override;
  unsigned int post(const std::string& hostname,
                    const std::string& path,
                    const std::string& body,
                    boost::asio::streambuf& response) const override;
  unsigned int postS(const std::string& hostname,
                     const std::string& path,
                     const std::string& body,
                     boost::asio::streambuf& response) const override;
  unsigned int getJSON(const std::string& hostname,
                       const std::string& path,
                       boost::property_tree::ptree& pt) const override;
  unsigned int getSJSON(const std::string& hostname,
                        const std::string& path,
                        boost::property_tree::ptree& pt) const override;
  unsigned int postJSON(const std::string& hostname,
                        const std::string& path,
                        const std::string& body,
                        boost::property_tree::ptree& pt) const override;
  unsigned int postSJSON(const std::string& hostname,
                         const std::string& path,
                         const std::string& body,
                         boost::property_tree::ptree& pt) const override;
};

} /* namespace http */

#endif /* HTTP_HTTP_INTERFACEIMPL_H_ */
