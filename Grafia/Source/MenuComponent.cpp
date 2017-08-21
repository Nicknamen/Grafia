/*
  ==============================================================================

    MenuComponent.cpp
    Created: 17 Aug 2017 5:10:56pm
    Author:  nicoc_000

  ==============================================================================
*/

#include "MenuComponent.h"

MenuComponent::MenuComponent()
{
	addAndMakeVisible(menuBar = new MenuBarComponent(this));
}

void MenuComponent::resized()
{
		Rectangle<int> area(getLocalBounds());
		menuBar->setBounds(area.removeFromTop(LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight()));

		area.removeFromTop(20);
		area = area.removeFromTop(33);
	}

StringArray MenuComponent::getMenuBarNames()
{
	const char* const names[] = { "File", "Edit", "Tools", "Help", nullptr };

	return StringArray(names);
}

PopupMenu MenuComponent::getMenuForIndex(int menuIndex, const String& /*menuName*/)
{
//	ApplicationCommandManager* commandManager = &MainAppWindow::getApplicationCommandManager();

	PopupMenu menu;

	if (menuIndex == 0)
	{
//		menu.addCommandItem(commandManager, MainAppWindow::showPreviousDemo);
//		menu.addCommandItem(commandManager, MainAppWindow::showNextDemo);
		menu.addSeparator();
//		menu.addCommandItem(commandManager, StandardApplicationCommandIDs::quit);
	}
	else if (menuIndex == 1)
	{
//		menu.addCommandItem(commandManager, MainAppWindow::useLookAndFeelV1);
//		menu.addCommandItem(commandManager, MainAppWindow::useLookAndFeelV2);
//		menu.addCommandItem(commandManager, MainAppWindow::useLookAndFeelV3);

		PopupMenu v4SubMenu;
//		v4SubMenu.addCommandItem(commandManager, MainAppWindow::useLookAndFeelV4Dark);
//		v4SubMenu.addCommandItem(commandManager, MainAppWindow::useLookAndFeelV4Midnight);
//		v4SubMenu.addCommandItem(commandManager, MainAppWindow::useLookAndFeelV4Grey);
//		v4SubMenu.addCommandItem(commandManager, MainAppWindow::useLookAndFeelV4Light);

		menu.addSubMenu("Use LookAndFeel_V4", v4SubMenu);

		menu.addSeparator();
//		menu.addCommandItem(commandManager, MainAppWindow::useNativeTitleBar);

#if ! JUCE_LINUX
//		menu.addCommandItem(commandManager, MainAppWindow::goToKioskMode);
#endif

/*		if (MainAppWindow* mainWindow = MainAppWindow::getMainAppWindow())
		{
			StringArray engines(mainWindow->getRenderingEngines());

			if (engines.size() > 1)
			{
				menu.addSeparator();

				for (int i = 0; i < engines.size(); ++i)
					menu.addCommandItem(commandManager, MainAppWindow::renderingEngineOne + i);
			}
		}
*/	}
	else if (menuIndex == 2)
	{
		if (TabbedComponent* tabs = findParentComponentOfClass<TabbedComponent>())
		{
			menu.addItem(3000, "Tabs at Top", true, tabs->getOrientation() == TabbedButtonBar::TabsAtTop);
			menu.addItem(3001, "Tabs at Bottom", true, tabs->getOrientation() == TabbedButtonBar::TabsAtBottom);
			menu.addItem(3002, "Tabs on Left", true, tabs->getOrientation() == TabbedButtonBar::TabsAtLeft);
			menu.addItem(3003, "Tabs on Right", true, tabs->getOrientation() == TabbedButtonBar::TabsAtRight);
		}
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
