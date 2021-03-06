/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

	This program has the purpose of creating new LaTeX mathematical symbols. A more detailed introduction can be
	found on GitHub:

	https://github.com/Nicknamen/Grafia

	Project:	Grafia
	Created:	17 Apr 2017
	Author:		Nicolo' Cavalleri
	Github:		Nicknamen

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"
#include "texlib.h"

using namespace std;

//==============================================================================
class GrafiaApplication  : public JUCEApplication
{
public:
    //==============================================================================
    GrafiaApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        // This method is where you should put your application's initialisation code..

        mainWindow = new MainWindow (getApplicationName(), commandLine);
    }

    void shutdown() override
    {
        // Add your application's shutdown code here..

        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const String& /*commandLine*/) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainContentComponent class.
    */
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow (String name)  : DocumentWindow (name,
                                                    Colours::lightgrey,
                                                    DocumentWindow::allButtons) //maximise button not dispayed on linux
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainContentComponent(), true);

            centreWithSize (getWidth(), getHeight());
            setVisible (true);
        }

		MainWindow(String name, const String & commandline) : DocumentWindow(name,
																			 Colours::lightgrey,
																			 DocumentWindow::allButtons)
		{
			setUsingNativeTitleBar(true);

			if (commandline.isEmpty())
				setContentOwned(move(mainComponent_ptr = new MainContentComponent()), true);
			else	//this is useful for calling grafia from terminal (linux) or double clicking on a file (windows)
				setContentOwned(move(mainComponent_ptr = new MainContentComponent(commandline)), true);

			centreWithSize(getWidth(), getHeight());
			setVisible(true);
		}

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
			
			if (mainComponent_ptr->projectIsSaved()) // if the project is not saved the user is asked wether he's sure to quit
				JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    ScopedPointer<MainWindow> mainWindow;

	static MainContentComponent * mainComponent_ptr;
};

MainContentComponent * GrafiaApplication::mainComponent_ptr = nullptr;

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (GrafiaApplication)
