/*
  ==============================================================================

    This file was auto-generated!

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
#include <regex>

using namespace std;

class LaTexSymbol;

class LatexDisplay : public ImageComponent
{
public:

	void paint(Graphics& g) override;
};

static std::unique_ptr<ApplicationCommandManager> applicationCommandManager;

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent final  : public Component,
									public Button::Listener,
									public TextEditor::Listener,
									public Slider::Listener,
									public ApplicationCommandTarget
{
public:
    //==============================================================================
    MainContentComponent();
	MainContentComponent(const String & commandline);
    ~MainContentComponent();

    void paint (Graphics&) override;
    void resized() override;

	ApplicationCommandTarget* getNextCommandTarget() override;
	void getAllCommands(Array<CommandID>& commands) override;
	void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
	bool perform(const InvocationInfo& info) override;

	void setMessage(std::string to_be_written);
	void setNewSymbolName(std::string newName);

	void buttonClicked(Button* button) override;
	void sliderValueChanged(Slider* slider) override;
	void textEditorTextChanged(TextEditor& textEditor) override;
	void textEditorReturnKeyPressed(TextEditor& textEditor) override;

	void update();

	void update_displayed();
	void zero_displayed();

	void exportSymbol();
	void save();
	void saveAs();
	void open();
	void open(std::string filePath);

	void runSettings();

	static ApplicationCommandManager& getApplicationCommandManager();

	enum CommandIDs
	{
		Save		= 1000,
		New,
		Open,
		Export,
		Settings,
		About
	};

	//exceptions
	class GrafiaException : public std::exception
	{
	public:
		template<typename T>
		GrafiaException(T what) { error_what = "Error: " + (std::string)what; }

		virtual const char* what() const throw()
		{
			return error_what.c_str(); //string cannot be modified inside this function
		}

	private:
		std::string error_what;
	};

private:
	std::string pickedFile;

	bool saved = true; //the user will not be disturbed for not saving an empty project
	string projectPath;

	std::vector<LaTexSymbol> symbolsList;

	std::string newSymbolName = "newSymbol";

	TextEditor tex_text;
	TextButton compile_button;
	Label tex_search;

	Label symbolNameLabel;
	TextEditor symbolNameEditor;

	Label xLabel;
	TextEditor xTextBox;
	Label yLabel;
	TextEditor yTextBox;

	TextButton add_button;
	TextButton remove_button;

	ToggleButton compileAtEachCommand;

	class TableComponent;

	std::unique_ptr<TableComponent> table_ptr;
//	std::shared_ptr<LaTexSymbol> selected_symbol = nullptr; //unique  or shared pointer does not work... it looks it creates copies. I don't get why
// but why should it ever work? there is no point in using a shared_ptr or unique ptr here.
	LaTexSymbol * selected_symbol = nullptr; //should this be an iterator instead?

	DrawableButton arrowUp;
	DrawableButton arrowDown;
	DrawableButton arrowLeft;
	DrawableButton arrowRight;

	DrawableButton moveUp;
	DrawableButton moveDown;

	class mySlider final : public Slider
	{
	public:
		String 	getTextFromValue(double value) override;

		static int slidersDigitsNum;
	
	} sizeSlider, rotationSlider;

	Label sizeLabel;
	Label rotationLabel;

	MenuComponent menubar;

	LatexDisplay tex_image;
	PNGImageFormat PNGreader;
	std::string message;

	DrawableImage svg_image;
	TeX texstream;
	std::string ImageFileName;

	void writeSymbolTeXCode(TeX & texStream);
	void compile();
	void add(LaTexSymbol newObject);
	void remove();
	void reset();
	
	//returns true if the project is saved or if the user wishes to continue without saving the project
	bool projectIsSaved();
	bool overwriteExistingFile(std::string filename);

	void moveSymbolUp();
	void moveSymbolDown();

	void raisex(const double x);
	void raisey(const double y);
	void scale(double factor);
	void rotate(double angle);

	void errorAlert(const std::exception & exc);

	unique_ptr<FileChooser> fc;

	DrawablePath * create_triangle(Point<float> a, Point<float> b, Point<float> c);
	
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

class LaTexSymbol
{
public:
	LaTexSymbol();
	LaTexSymbol( std::string name, std::string LaTex, double x = 0, double y = 0, double rotAngle = 0, double sizeRatio = 1, bool selected = false);

	LaTexSymbol& operator=(const LaTexSymbol& other);
	LaTexSymbol(const LaTexSymbol& other);
	
	bool operator==(const LaTexSymbol & other) const;
	bool operator!=(const LaTexSymbol & other) const;

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

	const enum attributeIDs
	{
		symbolID_id = 0,
		name_id,
		LaTex_id,
		selected_id,
		x_id,
		y_id,
		rotAngle_id,
		sizeRatio_id
	};

private:
	const int _symbolID; //danger: the price for declaring it const is that iterators break the continuity. See the operator= implementation.
	std::string _name;
	std::string _LaTex;
	double _x;
	double _y;
	double _rotAngle;
	double _sizeRatio;
	bool _selected;

	static int _symbolCount;
};

#endif  // MAINCOMPONENT_H_INCLUDED
