/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "texlib.h"
#include "MenuComponent.h"

#include <vector>
#include <memory>

using namespace std;

void to_tex(string formula, TeX & tw);
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
class MainContentComponent   : public Component,
							   public Button::Listener,
							   public TextEditor::Listener,
						  	   public Slider::Listener,
							   public ApplicationCommandTarget
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&) override;
    void resized() override;

	ApplicationCommandTarget* getNextCommandTarget() override;
	void getAllCommands(Array<CommandID>& commands) override;
	void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
	bool perform(const InvocationInfo& info) override;

	void buttonClicked(Button* button) override;
	void sliderValueChanged(Slider* slider) override;

	static ApplicationCommandManager& getApplicationCommandManager();

	enum CommandIDs
	{
		Save		= 1000,
		Quit		= 1001,
		New			= 1002,
		Open		= 1003,
		Export		= 1004
	};

private:
	vector<LaTexSymbol> symbolsList;

	TextEditor tex_text;
	TextButton compile_button;
	Label tex_search;

	Label xLabel;
	TextEditor xTextBox;
	Label yLabel;
	TextEditor yTextBox;

	TextButton add_button;

	ToggleButton compileAtEachCommand;

	class TableComponent;

	std::unique_ptr<TableComponent> table_ptr;

	DrawableButton arrowUp;
	DrawableButton arrowDown;
	DrawableButton arrowLeft;
	DrawableButton arrowRight;

	Slider sizeSlider;
	Label sizeLabel;
	Slider rotationSlider;
	Label rotationLabel;

	MenuComponent menubar;

	LatexDisplay tex_image;
	PNGImageFormat PNGreader;
	std::string message;

	Image tex_preimage;
	DrawableImage svg_image;
	TeX texstream;
	std::string ImageFileName;

	void compile();
	void add(LaTexSymbol newObject);

	DrawablePath * create_triangle(Point<float> a, Point<float> b, Point<float> c);
	
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

class LaTexSymbol
{
public:
	LaTexSymbol( std::string name, std::string LaTex, int x = 0, int y = 0, double rotAngle = 0, double sizeRatio = 1, bool selected = false);

	std::string getAttributeTextbyID(int id) const
	{
		if (id == symbolID_id)
			return to_string(_symbolID);
		else if (id == name_id)
			return _name;
		else if (id == LaTex_id)
			return _LaTex;
		else if (id == selected_id)
			return _selected ? "Y" : "N";
		else
			return{};
	}

	void setAttributebyID(int id, std::string text)
	{
		if (id == name_id)
			_name = text;
		else if (id == LaTex_id)
			_LaTex = text;
		else if (id == selected_id)
			_selected = ((text == "Y") ? 1 : 0);
	}

	void setAttributebyID(int id, double value)
	{
		if (id == selected_id)
			_selected = value;
		else if (id == x_id)
			_x = value;
		else if (id == y_id)
			_y = value;
		else if (id == rotAngle_id)
			_rotAngle = value;
		else if (id == sizeRatio_id)
			_sizeRatio = value;
	}

	bool get_selected() const;
	void set_selected(const bool b);

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
	const int _symbolID;
	std::string _name;
	std::string _LaTex;
	int _x;
	int _y;
	double _rotAngle;
	double _sizeRatio;
	bool _selected;

	static int _symbolCount;
};

#endif  // MAINCOMPONENT_H_INCLUDED
