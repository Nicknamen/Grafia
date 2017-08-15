/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainContentComponent::MainContentComponent()
{
	texstream.open("sum.tex");

	addAndMakeVisible(compile_button);
	compile_button.setButtonText("Compile");
	compile_button.addListener(this);

	addAndMakeVisible(tex_text);
	tex_text.addListener(this);

	setSize(900, 400);
}

void MainContentComponent::buttonClicked(Button* button)
{
	if (button == &compile_button)
	{
		try
		{
			message = "";

			texstream.open_rewritemode();

			to_tex(tex_text.getText().toStdString(), texstream);

			message += texstream.to_png();

			File teximage(File::getCurrentWorkingDirectory().getChildFile("sum.png"));

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

		repaint();
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
	g.drawText(message, getLocalBounds(), Justification::bottomRight, true);

	g.drawImageAt(tex_preimage, 10, 10);
}

void MainContentComponent::resized()
{
	tex_image.setBounds(0, 0, getWidth() / 1.5, getHeight() / 1.5);
	tex_text.setBounds(10, getHeight()*0.8, getWidth()/2 - 10, 40);
	compile_button.setBounds(getWidth()/2+10, getHeight()*0.8, getWidth()/2 - 20, 40);
}

void to_tex(string formula, TeX & tw)
{
	tw << "\\documentclass{minimal}\n"
		"\\usepackage[paperwidth=6.5cm, paperheight=3.5cm]{geometry}\n"
		"\\begin{document}\n$$"
		<< formula
		<< "$$\n\\end{document}";

	return;
}

/*void LatexDisplay::paint(Graphics & g)
{
//	g.fillAll(Colours::white);
}*/
