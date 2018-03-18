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
					  public MenuBarModel
{
public:
	MenuComponent();

	void resized() override;

	//==============================================================================
	StringArray getMenuBarNames() override;
	PopupMenu getMenuForIndex(int menuIndex, const String &) override;
	void menuItemSelected(int menuItemID, int /*topLevelMenuIndex*/) override;

private:
	ScopedPointer<MenuBarComponent> menuBar;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuComponent)
};

extern ScopedPointer<ApplicationCommandManager> applicationCommandManager;

#endif  // MENUCOMPONENT_H_INCLUDED
