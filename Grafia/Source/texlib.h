/*
==============================================================================

	texlib.cpp
	Author:		Nicolo' Cavalleri
	Github:		Nicknamen
	Created:	16 Aug 2017

==============================================================================
*/

#pragma once

#include <fstream>
#include <string>
#include <cstdlib>
#include <regex>
#include <exception>
#include <set>
#include <experimental/filesystem> //why is this not present in Linux???

#ifdef _WIN32
#include <Windows.h>
#endif

#include "path.h"

#ifdef _WIN32 // to execute shell commands
	static STARTUPINFO si;
	static PROCESS_INFORMATION pi;
#endif

/**
	This class is a TeX quick-compiler: it basically converts small portions of tex code to pdf or png.
	It is not designed to handle a proper tex file
	(reason for which it the methods to write and compile are implemented in the same class),
	even though, with little modification, it can be used as such.

	@tags{TEX}
*/
class TeX
{
public:
	/** Default creator
		@param	show_shell		if true shows shell information when executing shell commands (LaTeX compilation)
	*/
	TeX(bool temp = true, bool show_shell = false);

	/**
		@param	filename		TeX file to be opened
	*/
	TeX(std::string filename, bool temp = false, bool show_shell = false);

	/** Destructor
		Removes the last file created and the compilation temporary file.
		If certains extentions are not to be removed they should be passed to the function do_not_cancel(ext).
	*/
	~TeX();

	/** The default << operator to write on file.
		@param	string			what will be written on file. Accepts anything that can be converted to a string.
	*/
	template<typename string_convertable>
	std::ostream & operator<<(const string_convertable to_be_written) // template functions must be defined in header file
	{
		_istexmodified = true;

		return _texfile << std::string(to_be_written);
	}

	//	CONVERT METHODS //
	/** 
		Most basic conversion. Converts the tex input to a pdf document
	*/
	void to_pdf();
	void to_dvi();
	/** The central function of the library

		@param	ext				the extension to which one wants to convert the TeX.
								The allowed extensions can be found https://www.imagemagick.org/script/formats.php
		@param	middle_ext		the extension to which the TeX code shoud be compiled to: either PDF or DVI.
								Experimentally I can see that PDF is quicker, contrarily to expectations.

		Returns an eventual warning message. It makes no sense to throw exceptions for simple warnings.
	*/
	std::string to(std::string ext, std::string middle_ext = "pdf");

	/**
		Access methods for image density. The image density is intended to be the pixel density. By default it is 600.
	*/
	void set_image_density(const int density);
	int get_image_density() const;

	/**
		Opens the file stored in _texname. It returns
		- false				If no file constructor or opene(filenamed) has been called ever before
							or if any error occurs when opening the file
		- true				If it actually opens a file and everything goes well.

		No exception is thrown in this function, for it returns false easily.
		Exceptions must be handled with an if statement when calling the function.
	*/
	bool open();

	/**
		Opens the file explicitely with ios::trunc. Returns meaning same as in open().
	*/
	bool open_rewritemode();

	/**
		Opens a new file with a specified file name

		@param	filename		the name or path of the file that's going to be opened

		If there's problem at opening the file exception is thrown here.
	*/
	void open(std::string filename);

	/**
		Closes the current opened file if any.
	*/
	void close();

	/**
		Returns true if the file sored in _texname exists in the exe path.
	*/
	bool exists();

	/**
		The two following functions work thanks to variadic templates. They specify which extension of the compiled tex file
		are not to be removed in the exe path
	*/
	template<typename T = std::string>
	void do_not_cancel(T extenstion) //if these function are not defined here there is a linking error
	{
		extensions_not_to_cancel.insert(std::string(extenstion));
	}

	/**
		See above for explenation. The function is invoked like this: do_not_cancel(ext1, ext2, ext3, ..., extn);
	*/
	template <typename T = std::string, typename... Types>
	void do_not_cancel(T extenstion, Types... others)
	{
		extensions_not_to_cancel.insert(std::string(extenstion));

		do_not_cancel(others...);
	}

	/** 
		Removes the temporaty files. Let A be the set of all possible extensions and B the set of extension
		defined by the user; then all files of the kind emptyname.ext, with ext in A/B will be removed
	*/
	void rmfiles();

	/**	Access method for the name of the .tex file without extension. */
	std::string get_name() const;

	/** returns the path where the tex_file is stored */
	path get_path() const;
	/** returns filename with the full path and the extension
		@param extension		the extension to be added at the end of the file.
	*/
	std::string get_fullpath_ext(std::string extension) const;

	/** standard exception for errors happening in this libray */
	class TeXException : public std::exception
	{
	public:
		/** sets the message error for the exception considered */
		template<typename T>
		TeXException(T what)
		{
			error_what = "TeX level error: " + (std::string)what;
		}

		/** returns the message errror for the exception considered */
		virtual const char* what() const throw()
		{
			return error_what.c_str(); //Warning: do not create the string to be returned inside this scope
		}

	private:
		std::string error_what;
	};

private:
	int _density = 600; // I do not want this to go in the constructors:
						// the user might want to only compile tex files and forget about image features.

	std::string _texname; // name of file with .tex extension
	std::string _emptyname; // name of file without extension
#ifdef _WINDOWS
	path _temp_path = "C:\\temp\\"; // path of file.
#elif defined __linux__
	path _temp_path = "/tmp/"; // path of file.
#endif

	bool _istexcreated; // flag: true if the tex file has been created
	bool _istexmodified; // flag: true if texfile has been written on

	bool _is_shell_hidden;	// flag: true if the user wants to see the shell when executing commands
	bool _temp;				// flag: true of the path where the TeX should be created should be the OS temporary directory

	std::string _texpath;

	std::fstream _texfile; // opened file

	void name(std::string texname); // sets the name and the path with the input file "texname"

	std::set<std::string> extensions_not_to_cancel; // set of the extension not to cancel

	static std::set<std::string> extensions; // set of all the possible extension produced during the object life
};

/** Tests if a given file exists */
inline bool fexists(const std::string & filename) //inline functions must be defined in the header file!
{
	return std::experimental::filesystem::exists(filename);
}

/**
	Returns the path where all the operation involving the exe file are done.
*/
path ExePath();

/**	Execute a specific command

	@param	command				the command to be executed
	@param	is_shell_hidden		if true the console is not shown during execution
*/
std::string execute(const char* command, bool is_shell_hidden);
