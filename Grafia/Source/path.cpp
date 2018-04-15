/*
  ==============================================================================

    path.cpp
    Created: 14 Apr 2018 6:56:19pm
    Author:  nicoc_000

  ==============================================================================
*/

#include "path.h"

path path::back()
{
	remove_last_backslash();

	path backpath = *this;

	for (auto it = backpath.end() - 1; it != backpath.begin(); --it)
		if (*it == '\\')
		{
			backpath.pop_back();
			break;
		}
		else
			backpath.pop_back();

	return backpath;
}

path path::forward(std::string folder)
{
	remove_last_backslash();

	path forwardpath = *this;

	forwardpath.append("\\" + folder);

	return forwardpath;
}

void path::remove_last_backslash()
{
	if (this->at(this->size() - 1) == '\\')
		this->pop_back();
}
