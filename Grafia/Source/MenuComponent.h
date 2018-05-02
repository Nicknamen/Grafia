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
#include <memory>

//extern std::unique_ptr<ApplicationCommandManager> applicationCommandManager;

class MenuComponent final : public Component,
							public MenuBarModel
{
public:
	MenuComponent();
	~MenuComponent() = default;
	
	void resized() override;

	//==============================================================================
	StringArray getMenuBarNames() override;
	PopupMenu getMenuForIndex(int menuIndex, const String &) override;
	void menuItemSelected(int /*menuItemID*/, int /*topLevelMenuIndex*/) override;

private:
	std::unique_ptr<MenuBarComponent> menuBar;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuComponent)
};

#endif  // MENUCOMPONENT_H_INCLUDED
