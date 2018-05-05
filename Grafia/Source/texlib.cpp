/*
==============================================================================

	texlib.h
	Author:		Nicolo' Cavalleri
	Github:		Nicknamen
	Created:	16 Aug 2017

==============================================================================
*/

#include "texlib.h"

#include <iostream>
#include <string>
#include <cstdio>
#include <memory>
#include <array>

#ifdef __linux__
#include <libgen.h>
#include <unistd.h>
#include <linux/limits.h>
#endif

#include "Magick++.h"

using namespace std;

template<class T, class Compare = std::less<T>, class Allocator = std::allocator<T>>
std::set<T, Compare, Allocator> set_difference(const std::set<T, Compare, Allocator> & A, 
											   const std::set<T, Compare, Allocator> & B) //returns A - B, as in mathematics
{
	std::set<T, Compare, Allocator> C;

	for (T a : A)									//I can for sure optimize this function
		if (B.find(a) == B.end())
			C.insert(a);

	return C;
}

template<class T, class Compare = std::less<T>, class Allocator = std::allocator<T>>
std::set<T, Compare, Allocator> operator-(const std::set<T, Compare, Allocator> & A,
										  const std::set<T, Compare, Allocator> & B) //returns A - B, as in mathematics
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

	if (_texname.empty())
		return false;

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

	if (_texname.empty())
		return false;

	_texfile.open(_texname, ios::out | ios::trunc);

	if (_texfile.bad())
		return false;
	else
	{
		_istexcreated = true;
		_istexmodified = true;

		return true;
	}
}

void TeX::open(string filename)
{
	close();

	name(filename);

	if (open())
		_istexcreated = true;
	else
		throw TeXException("Error opening file " + filename);
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

	rmfiles();
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

void TeX::rmfiles()
{
	string command = "rm";

	for (auto ext : extensions - extensions_not_to_cancel)
		command += " " + _emptyname + "." + ext;

	execute(command.c_str(), _is_shell_hidden);
}

std::string TeX::get_name() const
{
	return _emptyname;
}

path TeX::get_path() const
{
	return _texpath;
}

std::string TeX::get_fullpath_ext(string extension) const
{
	return _texpath + _emptyname + "." + extension;
}

inline void TeX::name(string texname)
{
	regex texext(".tex$");

	_texname = texname;
	_emptyname = regex_replace(texname, texext, "");	// removes .tex from the end of the file

#ifdef _WINDOWS
	regex rgx_version("(Debug|Release)$");

	_texpath = regex_replace(ExePath(), rgx_version, ""); // in windows the folder where all files are created is
														  // one level back than Debug or Release. I'm using regular
														  // expressions because I want to be sure not to bo back one level
														  // unnecessarely.
#elif _linux_
	_texpath = ExePath();
#endif
}

void TeX::to_pdf()
{
	close();

	if (exists())
	{
		execute(("pdflatex " + _texname).c_str(), _is_shell_hidden);
	
		extensions.insert("pdf");
	}
	else
		throw TeXException("File " + _texname + " not found");
}

void TeX::to_dvi()
{
	close();

	if (exists())
	{
		execute(("latex " + _texname).c_str(), _is_shell_hidden);
	
		extensions.insert("dvi");
	}
	else
		throw TeXException("File " + _texname + " not found");
}

string TeX::to(std::string ext, std::string middle_ext)
{
	Magick::InitializeMagick(ExePath().forward("bin").c_str()); // not able to properly initialize magick
																// this method shoud tell it where to find xmls

	Magick::Image image;

	string fname = _emptyname + "." + middle_ext;				// the middle file

	image.density(to_string(get_image_density()));

	if (fexists(fname) && !_istexmodified)						// if the middle file has already been created it converts
	{															// the image directly from there
		try
		{
			// Read a file into image object 
			image.read(fname);

			// Write the image to a file 
			image.write(_emptyname + "." + ext);
		}
		catch (exception & exc)
		{
			throw TeXException("Magick exception: " + string(exc.what())); // Magick also throws a lot of exceptions
		}
	}
	else if (exists()) // to be compiled from tex, if pdf or dvi is not found
	{
		if (middle_ext == "pdf")
			to_pdf();
		else if (middle_ext == "dvi")
			to_dvi();
		else
			throw TeXException("Extension " + middle_ext + " not recognized");

		try
		{
			// Read a file into image object 
			image.read(fname);

			// Write the image to a file 
			image.write(_emptyname + "." + ext);
		}
		catch (const exception & exc)
		{
			if (!regex_search(exc.what(), regex("WarningHandler")))
				throw TeXException("Magick exception: " + string(exc.what()));	//important: never write throw exc:
																				//throw throws a copy of the base call,
																				//so in this case of exception!
			else
				return string(exc.what());
		}
	}
	else
	{
		throw TeXException("Unable to produce pdf or " + ext);
	}

	extensions.insert(ext);							// so that it is also automatically remove at the end

	return "Compiled succesfully";
}

void TeX::set_image_density(const int density)
{
	_density = density;
}

int TeX::get_image_density() const
{
	return _density;
}

set<string> TeX::extensions = {"tex", "log", "aux"}; // extension that are always produced during compilation

path ExePath()	// I really did not go into the details of this function: I just found it online
{
#ifdef _WIN32
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	string::size_type pos = string(buffer).rfind("VisualStudio2015");
	return path(string(buffer).substr(0, pos + 16));
#elif __linux__
	char result[PATH_MAX];
	ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
	if (count != -1)
	{
		char * result_path;
		result_path = dirname(result);
		return path(result_path);
	}
	else
		return path("");
#endif
}

std::string execute(const char * command, bool is_shell_hidden) // same as before: no idea how this works.
{																// I just found it online
#ifdef _WIN32 // doesn't return anything yet. I need to work on this

	if (is_shell_hidden)
	{
		LPSTR lpcomand = const_cast<char *>(command);

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
		system(command);

	return "";

#elif defined __linux__

	std::array<char, 128> buffer;
	std::string result;
	std::shared_ptr<FILE> pipe(popen(command, "r"), pclose);

	if (!pipe) throw std::runtime_error("popen() failed!");

	while (!feof(pipe.get()))
	{
		if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
			result += buffer.data();
	}

	//	cout << result;

	return result;

#endif
}

