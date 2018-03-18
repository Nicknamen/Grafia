/*
  ==============================================================================

    MenuComponent.cpp
    Created: 17 Aug 2017 5:10:56pm
    Author:  nicoc_000

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
		Quit = 1001,
		New = 1002,
		Open = 1003,
		Export = 1004
	};
};

MenuComponent::MenuComponent()
{
	addAndMakeVisible(menuBar = new MenuBarComponent(this));

	setApplicationCommandManagerToWatch(&MainContentComponent::getApplicationCommandManager());
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
	const char* const names[] = { "File", "Edit", "LaTex", "Help", nullptr };

	return StringArray(names);
}

PopupMenu MenuComponent::getMenuForIndex(int menuIndex, const String& /*menuName*/)
{
	ApplicationCommandManager* commandManager = &MainContentComponent::getApplicationCommandManager();

	PopupMenu menu;

	if (menuIndex == 0)
	{
		menu.addCommandItem(commandManager, MainContentComponent::New);
		menu.addCommandItem(commandManager, MainContentComponent::Save);
		menu.addCommandItem(commandManager, MainContentComponent::Open);
		menu.addSeparator();
		menu.addCommandItem(commandManager, StandardApplicationCommandIDs::quit);
	}
	else if (menuIndex == 1)
	{
		PopupMenu v4SubMenu;
		v4SubMenu.addCommandItem(commandManager, MainContentComponent::Open);

		menu.addSubMenu("submenu", v4SubMenu);

		menu.addSeparator();
	}
	else if (menuIndex == 2)
	{
		menu.addCommandItem(commandManager, MainContentComponent::Export);
		/*if (TabbedComponent* tabs = findParentComponentOfClass<TabbedComponent>())
		{
			menu.addItem(3000, "Tabs at Top", true, tabs->getOrientation() == TabbedButtonBar::TabsAtTop);
			menu.addItem(3001, "Tabs at Bottom", true, tabs->getOrientation() == TabbedButtonBar::TabsAtBottom);
			menu.addItem(3002, "Tabs on Left", true, tabs->getOrientation() == TabbedButtonBar::TabsAtLeft);
			menu.addItem(3003, "Tabs on Right", true, tabs->getOrientation() == TabbedButtonBar::TabsAtRight);
		}*/
	}
	else if (menuIndex == 3)
	{
//		return getDummyPopupMenu();
	}

	return menu;
}

void MenuComponent::menuItemSelected(int menuItemID, int)
{
	// most of our menu items are invoked automatically as commands, but we can handle the
	// other special cases here..

	if (menuItemID == 6000)
	{
#if JUCE_MAC
		if (MenuBarModel::getMacMainMenu() != nullptr)
		{
			MenuBarModel::setMacMainMenu(nullptr);
			menuBar->setModel(this);
		}
		else
		{
			menuBar->setModel(nullptr);
			MenuBarModel::setMacMainMenu(this);
		}
#endif
	}
	else if (menuItemID >= 3000 && menuItemID <= 3003)
	{
		if (TabbedComponent* tabs = findParentComponentOfClass<TabbedComponent>())
		{
			TabbedButtonBar::Orientation o = TabbedButtonBar::TabsAtTop;

			if (menuItemID == 3001) o = TabbedButtonBar::TabsAtBottom;
			if (menuItemID == 3002) o = TabbedButtonBar::TabsAtLeft;
			if (menuItemID == 3003) o = TabbedButtonBar::TabsAtRight;

			tabs->setOrientation(o);
		}
	}
	else if (menuItemID >= 12298 && menuItemID <= 12305)
	{
//		sendChangeMessage();
	}
}
