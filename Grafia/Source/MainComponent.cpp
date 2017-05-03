/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainContentComponent::MainContentComponent()
{
	try
	{
		texstream.open("sum.tex");

		to_tex("\\sum_{i=0}^n i = \\frac{n(n+1)}{2}", texstream);

		message += texstream.to_png();

		File teximage(File::getCurrentWorkingDirectory().getChildFile("sum.png"));

		File imagefile("C:\\Diploma_riforma.png");
	/*	File svgfile("C:\\prova.svg");

		ScopedPointer<XmlElement> svg(XmlDocument::parse(svgfile));

		if (svg != nullptr)
		{
			svg_image = DrawableImage::createFromSVG(*svg);

			message += "svg != nullptr";
		}
		else
		{
			message += "svg = nullptr";
		}
	*/
	//	addAndMakeVisible(svg_image);

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

		addAndMakeVisible(tex_image);
	}
	catch (string exc)
	{
		message += " " + exc;
	}

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
	g.drawText(message, getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{
	tex_image.setBounds(getWidth() / 6, getHeight() / 6, getWidth() / 1.5, getHeight() / 1.5);
}

void to_tex(string formula, TeX & tw)
{
	tw << "\\documentclass[tikz]{standalone}\n%\\usetikzlibrary{calc}\n\\begin{document}\n\\begin{tikzpicture}\n\\draw(0, 0) rectangle(8, 4) node[midway]{ \\large $"
		<< formula
		<< "$};\n\\end{tikzpicture}\n\\end{document}";

	return;
}
