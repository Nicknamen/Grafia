
#include "texlib.h"

#include <iostream>
#include <Magick++.h>

using namespace Magick;
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

std::string TeX::get_name() const
{
	return _emptyname;
}

std::string TeX::get_path() const
{
	return _texpath;
}

std::string TeX::get_fullpath_ext(string extension) const
{
	return _texpath + _emptyname + "." + extension;
}

inline void TeX::name(string texname)
{
	regex texext("\.tex$");
	regex rgx_version("(Debug|Release)$");

	_texname = texname;
	_emptyname = regex_replace(texname, texext, "");
	_texpath = regex_replace(ExePath(), rgx_version, "");
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
	if (fexists(_emptyname + ".dvi") && !_istexmodified) // to be compiled directly from dvi
	{
		system(("dvisvgm " + _emptyname + ".dvi").c_str());
	}
	else if (exists()) // to be compiled from tex
	{
		to_dvi();

		system(("dvisvgm " + _emptyname + ".dvi").c_str());
	}
	else
	{
		throw "Unable to produce dvi or svg";
	}
}

string TeX::to_png(string ext)
{
	InitializeMagick(ExePath().c_str());

	Image image;

	string fname = _emptyname + "." + ext;

	image.density("300");

	if (fexists(fname) && !_istexmodified) // to be compiled directly from dvi or pdf
	{
		try
		{
			// Read a file into image object 
			image.read(fname);

			// Write the image to a file 
			image.write(_emptyname + ".png");
		}
		catch (Exception &error_)
		{
			cout << "Caught exception: " << error_.what() << endl;
		}
	}
	else if (exists()) // to be compiled from tex
	{
		if (ext == "pdf")
			to_pdf();
		else if (ext == "dvi")
			to_dvi();
		else
			throw "Extension " + ext + " not recognized";

		try
		{
			// Read a file into image object 
			image.read(fname);

			// Write the image to a file 
			image.write(_emptyname + ".png");
		}
		catch (Exception &error_)
		{
			return "Magick has caught exception: " + string(error_.what()) + " ";
		}
	}
	else
	{
		throw "Unable to produce pdf or png";
	}

	return "";
}

#ifdef _WIN32
string ExePath() {
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	string::size_type pos = string(buffer).find_last_of("\\/");
	return string(buffer).substr(0, pos);
}
#endif