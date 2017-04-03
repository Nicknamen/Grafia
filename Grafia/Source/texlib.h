#pragma once

#ifndef TEXLIB_H_INCLUDED
#define TEXLIB_H_INCLUDED

#include <fstream>
#include <string>
#include <cstdlib>
#include <regex>
#include <exception>

class TeX
{
public:
	TeX();
	TeX(std::string filename);

	~TeX();

	std::ostream & operator<<(std::string & s);
	std::ostream & operator<<(const char* s);

	void to_pdf();
	void to_dvi();
	void to_svg();

	bool open();
	bool open(std::string filename);
	void close();

	bool exists();

private:
	std::string _texname;
	std::string _dviname;
	std::string _pdfname;
	std::string _svgname;

	bool _istexcreated;
	bool _istexmodified;

	std::fstream _texfile;

	void name(std::string texname);
};

inline bool fexists(std::string filename);

#endif