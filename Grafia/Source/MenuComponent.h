/*
  ==============================================================================

    MenuComponent.h
    Created: 17 Aug 2017 5:10:56pm
    Author:  nicoc_000

  ==============================================================================
*/

#ifndef MENUCOMPONENT_H_INCLUDED
#define MENUCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class MenuComponent : public Component,
				  public MenuBarModel,
				  private Button::Listener
{
public:
	MenuComponent()
	{
		addAndMakeVisible(menuBar = new MenuBarComponent(this));
	}

	~MenuComponent()
	{
	}

	void resized() override
	{
		Rectangle<int> area(getLocalBounds());
		menuBar->setBounds(area.removeFromTop(LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight()));

		area.removeFromTop(20);
		area = area.removeFromTop(33);
	}

	//==============================================================================
	StringArray getMenuBarNames() override
	{
		const char* const names[] = { "Demo", "Look-and-feel", "Tabs", "Misc", nullptr };

		return StringArray(names);
	}

private:
	ScopedPointer<MenuBarComponent> menuBar;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuComponent)
};

#endif  // MENUCOMPONENT_H_INCLUDED
