/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainContentComponent::MainContentComponent()
{
	ImageFileName = "tex_file";
	texstream.open(ImageFileName + ".tex");

	addAndMakeVisible(compile_button);
	compile_button.setButtonText("Compile");
	compile_button.addListener(this);

	addAndMakeVisible(tex_text);
	tex_text.addListener(this);

	tex_search.setText("Search:", dontSendNotification);
	tex_search.attachToComponent(&tex_text, FALSE);

	addAndMakeVisible(tex_image);

	setSize(800, 450);
}

void MainContentComponent::buttonClicked(Button* button)
{
	if (button == &compile_button)
	{
		compile();

		tex_image.repaint();

		repaint();
	}
}

void MainContentComponent::compile()
{
	try
	{
		message = "";

		texstream.open_rewritemode();

		to_tex(tex_text.getText().toStdString(), texstream);

		message += texstream.to_png();

		File teximage(File::getCurrentWorkingDirectory().getChildFile(String(ImageFileName + ".png")));

		tex_preimage = PNGImageFormat::loadFrom(teximage);

		if (tex_preimage.isValid())
		{
			message += " Image is valid";
		}
		else
		{
			message += " Image is not valid";
		}

		tex_image.setImage(tex_preimage);
	}
	catch (string exc)
	{
		message += " " + exc;
	}
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colours::lightgrey);

    g.setFont (Font (16.0f));
    g.setColour (Colours::black);

	juce::Rectangle<int> base(0, getHeight() - 20, getWidth(), getHeight());

	g.setColour(Colours::dimgrey);

	g.fillRect(base);

	g.setFont(Font(16.0f));
	g.setColour(Colours::white);

	g.drawText(message, juce::Rectangle<int>(0, 0, getWidth() - 5, getHeight() - 2), Justification::bottomRight, true);
}

void MainContentComponent::resized()
{
	tex_image.setBounds(10, 10, getWidth() / 2 - 15, getHeight() / 2 - 15);
	tex_text.setBounds(getWidth() / 2 + 5, getHeight()*0.1, getWidth()/4 - 10, 25);
	compile_button.setBounds(getWidth()*0.75 + 5, getHeight()*0.1, getWidth()/4 - 10, 25);
}

void to_tex(string formula, TeX & tw)
{
	tw << "\\documentclass{minimal}\n"
		"\\usepackage[paperwidth=6.4cm, paperheight=3.6cm]{geometry}\n"
		"\\begin{document}\n$$"
		<< formula
		<< "$$\n\\end{document}";

	return;
}

void LatexDisplay::paint(Graphics & g)
{
	g.fillAll(Colours::white);

	juce::Rectangle<float> bounds(0, 0, getWidth(), getHeight());

	g.drawImage(getImage(), bounds);

	g.setColour(Colours::darkgrey);

	g.drawRect(bounds);
}