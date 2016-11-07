/****************************************************************************
  FileName     [ vlpItoStr.h ]
  Package      [ vlp ]
  Synopsis     [ Header of Hierarchy Tree class ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2006 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_ITOSTR_H
#define VLP_ITOSTR_H

#include <iostream>
#include <string>
#include <sstream>

template<typename T> 
T fromString(const std::string& s) 
{
  std::istringstream iss(s);
  T x;
  iss >> x;
  return x;
}

template<typename T> 
std::string toString(const T& x) 
{
  std::ostringstream oss;
  oss << x;
  return oss.str();
}

#endif

