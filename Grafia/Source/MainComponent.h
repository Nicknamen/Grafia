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

	std::string newCommandName;

	TextEditor tex_text;
	TextButton compile_button;
	Label tex_search;

	Label xLabel;
	TextEditor xTextBox;
	Label yLabel;
	TextEditor yTextBox;

	TextButton add_button;
	TextButton remove_button;

	ToggleButton compileAtEachCommand;

	class TableComponent;

	std::unique_ptr<TableComponent> table_ptr;
	std::unique_ptr<LaTexSymbol> selected_symbol = nullptr;

	DrawableButton arrowUp;
	DrawableButton arrowDown;
	DrawableButton arrowLeft;
	DrawableButton arrowRight;

	Slider sizeSlider;

	Label sizeLabel;

	/*class RotationSlider final : public Slider
	{
	public:
		void stoppedDragging() override;
	}*/
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
	void remove();

	void raisex(const double x);
	void raisey(const double y);
	void scale(double factor);
	void rotate(double angle);

	DrawablePath * create_triangle(Point<float> a, Point<float> b, Point<float> c);
	
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

class LaTexSymbol
{
public:
	LaTexSymbol( std::string name, std::string LaTex, double x = 0, double y = 0, double rotAngle = 0, double sizeRatio = 1, bool selected = false);

	LaTexSymbol& operator=(const LaTexSymbol& other);
	LaTexSymbol(const LaTexSymbol& other);
	
	bool operator==(const LaTexSymbol & other);
	bool operator!=(const LaTexSymbol & other);

	std::string getAttributeTextbyID(int id) const;
	void setAttributebyID(int id, std::string text);
	void setAttributebyID(int id, double value);

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
	const int _symbolID; //danger: the price for declaring it const is that iterators brake the continuity. See the operator= implementation.
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
