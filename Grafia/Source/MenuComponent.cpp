/*
  ==============================================================================

    MenuComponent.cpp
	Project:	Grafia
    Created:	17 Aug 2017 5:10pm
    Author:		Nicolo' Cavalleri
	Github:		Nicknamen
	Link:		https://github.com/Nicknamen/Grafia

  ==============================================================================
*/

#include "MenuComponent.h"

class MainContentComponent
{
public:
	static ApplicationCommandManager& getApplicationCommandManager();

	enum CommandIDs
	{
		Save = 1000,
		SaveAs,
		New,
		Open,
		Export,
		Settings,
		About
	};
};

MenuComponent::MenuComponent()
{
//	menuBar = std::make_unique<MenuBarComponent>(this);

	menuBar.reset(new MenuBarComponent(this));	//what does change with respect to before? The new demo does like this...
	addAndMakeVisible(menuBar.get());

	setApplicationCommandManagerToWatch(&MainContentComponent::getApplicationCommandManager());	//to know the commands
}

void MenuComponent::resized()
{
		juce::Rectangle<int> area(getLocalBounds());
		menuBar->setBounds(area.removeFromTop(LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight()));

		area.removeFromTop(20);
		area = area.removeFromTop(33);
}

StringArray MenuComponent::getMenuBarNames()
{
	return  {"File", "Edit", "LaTeX", "Help" };
}

PopupMenu MenuComponent::getMenuForIndex(int menuIndex, const String& /*menuName*/)
{
	ApplicationCommandManager* commandManager = &MainContentComponent::getApplicationCommandManager();

	PopupMenu menu;

	if (menuIndex == 0)
	{
		menu.addCommandItem(commandManager, MainContentComponent::New);
		menu.addCommandItem(commandManager, MainContentComponent::Save);
		menu.addCommandItem(commandManager, MainContentComponent::SaveAs);
		menu.addCommandItem(commandManager, MainContentComponent::Open);
		menu.addSeparator();
		menu.addCommandItem(commandManager, StandardApplicationCommandIDs::quit);
	}
	else if (menuIndex == 1)
	{
		menu.addCommandItem(commandManager, MainContentComponent::Settings);

//		PopupMenu v4SubMenu;
//		v4SubMenu.addCommandItem(commandManager, MainContentComponent::Open);

//		menu.addSubMenu("submenu", v4SubMenu);
	}
	else if (menuIndex == 2)
	{
		menu.addCommandItem(commandManager, MainContentComponent::Export);
	}
	else if (menuIndex == 3)
	{
		menu.addCommandItem(commandManager, MainContentComponent::About);
	}

	return menu;
}

void MenuComponent::menuItemSelected(int /*menuItemID*/, int)	//no need of this function but
{																//it's abstract so I have to overwrite it
	// most of our menu items are invoked automatically as commands, but we can handle the
	// other special cases here..
}
