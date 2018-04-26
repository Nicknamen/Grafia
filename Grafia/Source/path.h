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

/**	This class is there to implement path strings.
	When c++17 will be available (that is switching to visual studio 2017) I'll sobstitute this with the standard path class
*/
class path : public std::string
{
	using std::string::string; // to inherit string constructors

public:
	path() : std::string() {} // why is this not inherited already?
	path(std::string _string) : path(_string.c_str()) {} // and this as well?

	/** Chops back the last folder
	
		c:\users\nico\desktop -> c:\users\nico
	*/
	path back();

	/**	Adds level to the current stored path

		@param	folder		the name of the folder to add

		forward(destktop); => c:\users\nico -> c:\users\nico\desktop
	*/
	path forward(std::string folder);

	/** no need for description
		c:\users\nico\desktop\ -> c:\users\nico\desktop
	*/
	void remove_last_backslash();
};