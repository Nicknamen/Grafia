#pragma once

#ifndef TEXLIB_H_INCLUDED
#define TEXLIB_H_INCLUDED

#include <fstream>
#include <string>
#include <cstdlib>
#include <regex>
#include <exception>

#ifdef _WIN32
#include <Windows.h>
#endif


class TeX
{
public:
	TeX();
	TeX(std::string filename);

	~TeX();

	std::ostream & operator<<(std::string & s);
	std::ostream & operator<<(const char* s);

	//convert methods
	void to_pdf();
	void to_dvi();
	void to_svg();
	std::string to_png(std::string ext = "pdf"); //returns a possible message error

	bool open();
	bool open(std::string filename);
	void close();

	bool exists();

	//access methods
	std::string get_name() const;
	std::string get_path() const;
	std::string get_fullpath_ext(std::string extension) const; //returns filename with the extension specified

private:
	std::string _texname;
	std::string _emptyname;
	std::string _texpath;

	bool _istexcreated;
	bool _istexmodified;

	std::fstream _texfile;

	void name(std::string texname);
};

inline bool fexists(std::string filename);

#ifdef _WIN32
std::string ExePath();
#endif

#endif