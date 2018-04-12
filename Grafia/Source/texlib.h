#pragma once

#ifndef TEXLIB_H_INCLUDED
#define TEXLIB_H_INCLUDED

#include <fstream>
#include <string>
#include <cstdlib>
#include <regex>
#include <exception>
#include <set>

#ifdef _WIN32
#include <Windows.h>
#endif


class TeX
{
public:
	TeX(bool show_shell = false);
	TeX(std::string filename, bool show_shell = false);

	~TeX();

	std::ostream & operator<<(std::string & s);
	std::ostream & operator<<(const char* s);

	//convert methods
	void to_pdf();
	void to_dvi();
	void to_svg();
	void to_png(std::string ext = "pdf");

	void execute(const char* comand);

	bool open();
	bool open_rewritemode();
	bool open(std::string filename);
	void close();

	bool exists();

	template<typename T = std::string>
	void do_not_cancel(T extenstion) //if these function are not defined here there is a linking error
	{
		extensions_not_to_cancel.insert((string)extenstion);
	}

	template <typename T = std::string, typename... Types>
	void do_not_cancel(T extenstion, Types... others)
	{
		extensions_not_to_cancel.insert((string)extenstion);

		do_not_cancel(others...);
	}

	//access methods
	std::string get_name() const;
	std::string get_path() const;
	std::string get_fullpath_ext(std::string extension) const; //returns filename with the extension specified

	//exception
	class TeXException : public std::exception
	{
	public:
		template<typename T>
		TeXException(T what) { error_what = "TeX level error: " + (std::string)what; }

		virtual const char* what() const throw()
		{
			return error_what.c_str(); //Warning: do not create the string to be returned inside this scope
		}

	private:
		std::string error_what;
	};

private:
	std::string _texname;
	std::string _emptyname;
	std::string _texpath;

	bool _istexcreated;
	bool _istexmodified;

	bool _is_shell_hidden;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	std::fstream _texfile;

	void name(std::string texname);

	std::set<std::string> extensions_not_to_cancel;

	static std::set<std::string> extensions;
};

inline bool fexists(std::string filename);

class path : public std::string // when c++17 available sobstitute this with the standard path class
{
	using std::string::string;

public:
	path back();
	path forward(std::string folder);
	void remove_last_backslash();

	path(std::string _string) : path(_string.c_str()) {}
};

#ifdef _WIN32
path ExePath();
#endif

#endif