/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
{
	File imagefile("C:\\Diploma_riforma.png");
	FileInputStream fis(imagefile);	
	if (PNGreader.canUnderstand(fis))
	{
		message = "Image understood";
	}
	else
	{
		message = "Image not understood";
	}

	//	addAndMakeVisible(tex_image);
	setSize(600, 400);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xff001F36));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText (message, getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
