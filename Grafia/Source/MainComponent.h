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
#include <memory>

using namespace std;


void to_tex(string formula, TeX & tw);

class LatexDisplay : public ImageComponent
{
public:

	void paint(Graphics& g) override;
};


static ScopedPointer<ApplicationCommandManager> applicationCommandManager;
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component,
							   public Button::Listener,
							   public TextEditor::Listener,
							   public ApplicationCommandTarget,
							   private AsyncUpdater
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

	static ApplicationCommandManager& getApplicationCommandManager();

	enum CommandIDs
	{
		Save		= 1000,
		Quit		= 1001,
		New			= 1002,
		Open		= 1003
	};

private:
	TextEditor tex_text;
	TextButton compile_button;
	Label tex_search;

	MenuComponent menubar;

	LatexDisplay tex_image;
	PNGImageFormat PNGreader;
	std::string message;

	Image tex_preimage;
	DrawableImage svg_image;
	TeX texstream;
	std::string ImageFileName;

	void compile();

	void handleAsyncUpdate() override;
	
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

#endif  // MAINCOMPONENT_H_INCLUDED
