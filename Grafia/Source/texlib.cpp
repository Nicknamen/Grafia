
#include "texlib.h"

#include <iostream>
#include <string>

#include "Magick++.h"

using namespace Magick;
using namespace std;

template<class T, class Compare = std::less<Key>, class Allocator = std::allocator<Key>>
std::set<T, Compare, Allocator> set_difference(const std::set<T, Compare, Allocator> & A, 
											   const std::set<T, Compare, Allocator> & B) //returns A - B
{
	std::set<T, Compare, Allocator> C;

	for (T a : A)									//I can for sure optimize this function
		if (B.find(a) == B.end())
			C.insert(a);

	return C;
}

template<class T, class Compare = std::less<Key>, class Allocator = std::allocator<Key>>
std::set<T, Compare, Allocator> operator-(const std::set<T, Compare, Allocator> & A,
										  const std::set<T, Compare, Allocator> & B) //returns A - B
{
	return set_difference(A, B);
}

TeX::TeX(bool show_shell)
{
	_istexcreated = false;
	_istexmodified = true;

	_is_shell_hidden = !show_shell;
}

TeX::TeX(string filename, bool show_shell)
{
	open(filename);

	_is_shell_hidden = !show_shell;
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

bool TeX::open_rewritemode()
{
	close();

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

	string command;

	for (auto ext : extensions - extensions_not_to_cancel)
		command += "rm " + _emptyname + "." + ext + " ";

	execute(command.c_str());
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

	test.close();
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
		execute(("pdflatex " + _texname).c_str());
	else
		throw "File " + _texname + " not found";
}

void TeX::to_dvi()
{
	close();

	if (exists())
		execute(("latex " + _texname).c_str());
	else
		throw "File " + _texname + " not found";
}

void TeX::to_svg()
{
	if (fexists(_emptyname + ".dvi") && !_istexmodified) // to be compiled directly from dvi
	{
		execute(("dvisvgm " + _emptyname + ".dvi").c_str());
	}
	else if (exists()) // to be compiled from tex
	{
		to_dvi();

		execute(("dvisvgm " + _emptyname + ".dvi").c_str());
	}
	else
	{
		throw "Unable to produce dvi or svg";
	}
}

void TeX::to_png(string ext)
{
	InitializeMagick(ExePath().forward("bin").c_str()); // not able to properly initialize magick

	Image image;

	string fname = _emptyname + "." + ext;

	image.density(to_string(get_image_density()));

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
			throw TeXException("Magick exception: " + string(error_.what()) + " ");
		}
	}
	else if (exists()) // to be compiled from tex
	{
		if (ext == "pdf")
			to_pdf();
		else if (ext == "dvi")
			to_dvi();
		else
			throw TeXException("Extension " + ext + " not recognized");

		try
		{
			// Read a file into image object 
			image.read(fname);

			// Write the image to a file 
			image.write(_emptyname + ".png");
		}
		catch (Exception &error_)
		{
			throw TeXException("Magick exception: " + string(error_.what()) + " ");
		}
	}
	else
	{
		throw TeXException("Unable to produce pdf or png");
	}
}

void TeX::set_image_density(const int density)
{
	_density = density;
}

int TeX::get_image_density() const
{
	return _density;
}

set<string> TeX::extensions = {"pdf", "tex", "log", "aux", "png"};

void TeX::execute(const char* comand)
{
#ifdef _WIN32

	if (_is_shell_hidden)
	{
		LPSTR lpcomand = const_cast<char *>(comand);

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		CreateProcess(NULL,   // No module name (use command line)
			lpcomand,        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			CREATE_NO_WINDOW,  // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi);         // Pointer to PROCESS_INFORMATION structure

		WaitForSingleObject(pi.hProcess, 10000);

		// Close process and thread handles. 
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else
		system(comand);

#elif defined __linux__

	if (_is_shell_hidden)
		fork(comand);
	else
		system(comand);

#endif
}

#ifdef _WIN32
path ExePath() {
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	string::size_type pos = string(buffer).rfind("VisualStudio2015");
	return path(string(buffer).substr(0, pos + 16));
}
#endif
