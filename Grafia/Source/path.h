/*
  ==============================================================================

    path.h
    Created: 14 Apr 2018 6:56:19pm
    Author:  nicoc_000

  ==============================================================================
*/

#pragma once

#include <string>
#include <cstdlib>

class path : public std::string // when c++17 available sobstitute this with the standard path class
{
	using std::string::string;

public:
	path back();
	path forward(std::string folder);
	void remove_last_backslash();

	path(std::string _string) : path(_string.c_str()) {}
};