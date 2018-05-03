/*
  ==============================================================================

    This file was auto-generated!

	MainComponent.h
	Project:	Grafia
	Created:	17 Apr 2017
	Author:		Nicolo' Cavalleri
	Github:		Nicknamen

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#define SOFTWARE_VERSION "0.1.0"

#include "../JuceLibraryCode/JuceHeader.h"
#include "texlib.h"
#include "MenuComponent.h"

#include <fstream>
#include <vector>
#include <list>
#include <memory>

std::string eatRightZeros(std::string & input);
std::string eatRightZeros(std::string && input); // to make it work with to_string() output

class LaTexSymbol;

/**	This class implements the portion of the screen where the image is shown.
	It is handy to have it on its own as it can be repainted when the image changes.
 */
class LatexDisplay : public ImageComponent
{
public:

	void paint(Graphics& g) override;
};

static std::unique_ptr<ApplicationCommandManager> applicationCommandManager;

//==============================================================================
/*
    This component lives inside our window. It is the main class, performing all the main functionalities of the program.
*/
class MainContentComponent final  : public Component,
									public Button::Listener,
									public TextEditor::Listener,
									public Slider::Listener,
									public ApplicationCommandTarget // as it will be the target of all menu commands
{
public:
    //==============================================================================
    MainContentComponent();
	MainContentComponent(const String & commandline); // to implement double click on file (not working) or call from terminal
    ~MainContentComponent();

	/** returns true if the project is saved or if the user wishes to continue without saving the project */
	bool projectIsSaved();
	/** returns true if the file "filename" does not exists or if the file exists and the user wishes to overwrite it */
	bool overwriteExistingFile(std::string filename);

	// Abstract juce::Component functions
    void paint (Graphics&) override;
    void resized() override;

	// Abstract juce::ApplicationCommandTarget functions
	ApplicationCommandTarget* getNextCommandTarget() override; //need it to pass commands to the menu
	void getAllCommands(Array<CommandID>& commands) override;
	void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
	bool perform(const InvocationInfo& info) override;

	// Abstract juce::Listener functions
	void buttonClicked(Button* button) override;
	void sliderValueChanged(Slider* slider) override;
	void textEditorTextChanged(TextEditor& textEditor) override;
	void textEditorReturnKeyPressed(TextEditor& textEditor) override;

	void setNewSymbolName(std::string newName);

	/** This function refreshes the window by updating everything displayed */
	void update();

	/** Updates all the components showing data about the symbol displayed */
	void update_displayed();
	/** Resets every component showing data about the symbol displayed */
	void zero_displayed();

	// The following functions save and open the file in some way
	/** Exports the symbol as a .tex file. Prompts the user to input where to save it */
	void exportSymbol();
	/** Exports the symbol as a .grproj file.
		The extension is appositely created for this application to open it.
		Prompts the user to input where to save it only if it is the first time the user saves it.
	*/
	void save();
	/** As in save() but always prompts the user */
	void saveAs();
	/** Opens a file.
		The user is prompted to choose the file to open.
	*/
	void open();
	/** Opens the file given as a parameter */
	void open(std::string filePath); // Not working on double click in windows!!

	/** Runs a settings window where the user can control:
			- the pixel density of the picture displayed
			- the number of digits displayed by sliders
	*/
	void runSettings();

	/** Returns the application command manager */
	static ApplicationCommandManager& getApplicationCommandManager();

	enum CommandIDs
	{
		Save		= 1000,
		SaveAs,
		New,
		Open,
		Export,
		Settings,
		About
	};

	/** To handle exceptions that come out at the Global level */
	class GrafiaException : public std::exception
	{
	public:
		template<typename T>
		GrafiaException(T what) { error_what = "Global level error:\n" + std::string(what); }

		virtual const char* what() const throw()
		{
			return error_what.c_str(); //string cannot be modified inside this function
		}

	private:
		std::string error_what;
	};

private:
	std::string pickedFile;

	bool saved = true; // it starts true so that the user will not be disturbed for not saving an empty project
	std::string projectPath; // it stores the filename of the opened project if any.

	std::vector<LaTexSymbol> symbolsList; // this contains basically all what matters about the symbol
										  // all the TeX components with all the information are stored here

	std::string newSymbolName = "newSymbol"; // is the name of the symbol that will be called in LaTeX as \newSymbol

	TextEditor tex_text; // the editor of the TeX component to add to the symbol
	TextButton compile_button;
	Label tex_search; // Just a label for tex_text

	Label symbolNameLabel;
	TextEditor symbolNameEditor; // where newSymbolName is edited and displayed

	Label xLabel;
	TextEditor xTextBox; // where the x displacement is stored
	Label yLabel;
	TextEditor yTextBox; // where the y displacement is stored

	TextButton add_button; // the button that adds what is in tex_text in symbolsList
	TextButton remove_button; // removes selected symbols from the list

	ToggleButton compileAtEachCommand; // if true everything gets compiled at each modification

	std::unique_ptr<MenuComponent> menubar; // the menu shown on the top. I need to control it's creation and destruction
											// if I do not delete this as the first thing a bug sometimes pops up

	class TableComponent; // the table that displays all the symbols and their information

	std::unique_ptr<TableComponent> table_ptr;	// points to the table component. Pointer because, again, I need to control
												// creation and destruction.
//	std::shared_ptr<LaTexSymbol> selected_symbol = nullptr; //unique  or shared pointer does not work... it looks it creates copies. I don't get why
//  but why should it ever work? there is no point in using a shared_ptr or unique ptr here.
	LaTexSymbol * selected_symbol = nullptr;	// points to the last clicked symbol, the one being displayed.
												// should this be an iterator instead?

	// Arrow buttons
	DrawableButton arrowUp;
	DrawableButton arrowDown;
	DrawableButton arrowLeft;
	DrawableButton arrowRight;

	// move a symbol up and down in the symbolsList
	DrawableButton moveUp;
	DrawableButton moveDown;

	// implements a slider that does not show pointless zeros on the right (the default slider looks ugly).
	class mySlider final : public Slider
	{
	public:
		String 	getTextFromValue(double value) override;

		static int slidersDigitsNum;
	
	} sizeSlider, rotationSlider;

	Label sizeLabel;
	Label rotationLabel;

	LatexDisplay tex_image;
	PNGImageFormat PNGreader; // necessary to display the image

	// the message giving base information on the bottom-right corner of the window
	class messageComponent : public Component
	{
	public:
		void paint(Graphics & g) override;
		void set(std::string to_be_written);

	private:
		std::string message;
	} message;

//	DrawableImage svg_image; // to display svg in the future
	TeX texstream;	// the temporary file to be compiled to display the image
	std::string ImageFileName;	// the filename of the image being shown

	void writeSymbolTeXCode(TeX & texStream);	// it translates the information stored in the symbolsList on the TeX file
	void compile();	//	the central function. Compiles the TeX file and converts it in to an image. 
	void add(LaTexSymbol newObject);	//	adds a symbol to the list
	void remove();	//	removes all the selected symbols
	void reset();	//	cancels everything and resets the window to a virgin status

	// implement the movement of a symbol. The moved symbol is the clicked one
	void moveSymbolUp();
	void moveSymbolDown();

	// functions that modify the value of the symbol parameters and all other selected symbol by a proportional amount
	void raisex(const double x);
	void raisey(const double y);
	void scale(double factor);
	void rotate(double angle);

	// displays an error with the message of the exception
	void errorAlert(const std::exception & exc);

	// necessary to launch file choosers
	std::unique_ptr<FileChooser> fc;
	
	// need this function to drow arrows
	DrawablePath * create_triangle(Point<float> a, Point<float> b, Point<float> c);
	
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

class LaTexSymbol
{
public:
	LaTexSymbol();
	LaTexSymbol( std::string name, std::string LaTex, double x = 0, double y = 0,
		double rotAngle = 0, double sizeRatio = 1, bool selected = false);

	// need to implement these because I put a const private member
	LaTexSymbol& operator=(const LaTexSymbol& other);
	LaTexSymbol(const LaTexSymbol& other);
	
	// need to override these because _seclected should not be confronted
	bool operator==(const LaTexSymbol & other) const;
	bool operator!=(const LaTexSymbol & other) const;

	// ACCESS METHODS
	// these are handy when writing and reading files
	std::string getAttributeTextbyID(int id) const;
	void setAttributebyID(int id, std::string text);
	void setAttributebyID(int id, double value);

	const std::string getName();

	bool is_selected() const;
	void set_selected(const bool b);
	std::string getLaTex() const;

	double getx() const;
	void setx(const double x);
	double gety() const;
	void sety(const double y);

	double getSizeRatio() const;
	void setSizeRatio(const double size_ratio);
	double getRotAngle() const;
	void setRotAngle(const double rotation_angle);

	enum attributeIDs //const compiles on windows but not on linux
	{
		symbolID_id = 0,
		name_id,
		LaTex_id,
		selected_id,
		x_id,
		y_id,
		rotAngle_id,
		sizeRatio_id,
		stop //handy in for loops. Musts always be the last
	};

private:
	const int _symbolID; //danger: the price for declaring it const is that iterators break the continuity. See the operator= implementation.
	std::string _name;
	std::string _LaTex; //LaTeX code
	double _x; //x translation displacement
	double _y; //y translation displacement
	double _rotAngle; //rotation angle
	double _sizeRatio; //size Ratio
	bool _selected;

	static int _symbolCount; //it creates a proper ID for the symbol which is unique for a whole run of the program
};

#endif  // MAINCOMPONENT_H_INCLUDED
