
#include "stdafx.h"
#include "texlib.h"

#include <iostream>

using namespace std;

TeX::TeX()
{
	_istexcreated = false;
	_istexmodified = true;
}

TeX::TeX(string filename)
{
	open(filename);
}

bool TeX::open()
{
	close();

	if (exists())
		_texfile.open(_texname);
	else
		_texfile.open(_texname, ios::out);

	if (_texfile.bad())
		return false;
	else
	{
		_istexcreated = true;
		_istexmodified = true;

		return true;
	}
}

bool TeX::open(string filename)
{
	close();

	name(filename);

	if (open())
	{
		_istexcreated = true;

		return true;
	}
	else
	{
		throw "Error opening file " + filename;

		return false;
	}
}

inline void TeX::close()
{
	if (_texfile.is_open())
		_texfile.close();

		_istexmodified = false;
}

TeX::~TeX()
{
	close();
}

ostream & TeX::operator<<(string & s)
{
	_istexmodified = true;

	return _texfile << s;
}

ostream & TeX::operator<<(const char* s)
{
	_istexmodified = true;

	return _texfile << s;
}

inline bool fexists(string filename)
{
	fstream test(filename);

	if (test.good())
		return true;
	else
		return false;
}

inline bool TeX::exists()
{
	if (_istexcreated && !_texname.empty())
	{
		return fexists(_texname);
	}
	else
		return false;
}

inline void TeX::name(string texname)
{
	regex texext("\.tex$");

	_texname = texname;
	_dviname = regex_replace(texname, texext, ".dvi");
	_svgname = regex_replace(texname, texext, ".svg");
	_pdfname = regex_replace(texname, texext, ".pdf");
}

void TeX::to_pdf()
{
	close();

	if (exists())
		system(("pdflatex " + _texname).c_str());
	else
		throw "File " + _texname + " not found";
}

void TeX::to_dvi()
{
	close();

	if (exists())
		system(("latex " + _texname).c_str());
	else
		throw "File " + _texname + " not found";
}

void TeX::to_svg()
{
	if (fexists(_svgname) && !_istexmodified) // to be compiled directly from dvi
	{
		system(("dvisvgm " + _dviname).c_str());
	}
	else if (exists()) // to be compiled from tex
	{
		to_dvi();

		system(("dvisvgm " + _dviname).c_str());
	}
	else
	{
		throw "Unable to produce dvi or svg";
	}
}