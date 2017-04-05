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

	tex_preimage = PNGImageFormat::loadFrom(imagefile);

	if (tex_preimage.isValid())
	{
		message += "Image is valid";
	}
	else
	{
		message += "Image is not valid";
	}

	addAndMakeVisible(tex_image);
	setSize(900, 400);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xff001F36));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);

	tex_image.setImage(tex_preimage);
}

void MainContentComponent::resized()
{
	tex_image.setBounds(getWidth() / 4, getHeight() / 4, getWidth() / 2, getHeight() / 2);
}
