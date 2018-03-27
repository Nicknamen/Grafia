/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "TableComponent.h"
#include "MainComponent.h"

inline Colour getRandomColour(float brightness)
{
	return Colour::fromHSV(Random::getSystemRandom().nextFloat(), 0.5f, brightness, 1.0f);
}

inline Colour getRandomBrightColour() { return getRandomColour(0.8f); }
inline Colour getRandomDarkColour() { return getRandomColour(0.3f); }

//==============================================================================
MainContentComponent::MainContentComponent() : arrowUp("arrowUp", DrawableButton::ImageOnButtonBackground),
											   arrowDown("arrowDown", DrawableButton::ImageOnButtonBackground),
											   arrowLeft("arrowLeft", DrawableButton::ImageOnButtonBackground),
											   arrowRight("arrowRight", DrawableButton::ImageOnButtonBackground),
											   table_ptr(new TableComponent(this))

{
	ImageFileName = "tex_file";
	texstream.open(ImageFileName + ".tex");

	applicationCommandManager->registerAllCommandsForTarget(this); //registers the commands created by the user
	applicationCommandManager->registerAllCommandsForTarget(JUCEApplication::getInstance()); //registers the standard commands, such as quit.
	addAndMakeVisible(&menubar);

	addKeyListener(applicationCommandManager->getKeyMappings());

	addAndMakeVisible(compile_button);
	compile_button.setButtonText("Compile");
	compile_button.addListener(this);

	addAndMakeVisible(arrowUp);
	addAndMakeVisible(arrowDown);
	addAndMakeVisible(arrowLeft);
	addAndMakeVisible(arrowRight);

	arrowUp.setImages(create_triangle(Point<float>(10, 0), Point<float>(20, 20), Point<float>(0, 20)), 0, 0);
	arrowDown.setImages(create_triangle(Point<float>(), Point<float>(20, 0), Point<float>(10, 20)), 0, 0);
	arrowLeft.setImages(create_triangle(Point<float>(0, 10), Point<float>(20, 0), Point<float>(20, 20)), 0, 0);
	arrowRight.setImages(create_triangle(Point<float>(), Point<float>(0, 20), Point<float>(20, 10)), 0, 0);

	arrowUp.addListener(this);
	arrowDown.addListener(this);
	arrowLeft.addListener(this);
	arrowRight.addListener(this);

	addAndMakeVisible(sizeSlider);
	sizeSlider.setRange(0.0, 100.0);
	sizeSlider.setSkewFactor(0.3);
	sizeSlider.addListener(this);
	sizeSlider.setValue(1.0);
//	sizeSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 100, sizeSlider.getTextBoxHeight());

	addAndMakeVisible(sizeLabel);
	sizeLabel.setText("Size", dontSendNotification);
//	sizeLabel.setColour(100, findColour(0x1000200));
	sizeLabel.attachToComponent(&sizeSlider, true);

	addAndMakeVisible(rotationSlider);
	rotationSlider.setRange(0.0, 360.0);
	rotationSlider.setTextValueSuffix("d");
	rotationSlider.addListener(this);

	addAndMakeVisible(rotationLabel);
	rotationLabel.setText("Rotation", dontSendNotification);
	rotationLabel.attachToComponent(&rotationSlider, true);

	symbolsList.push_back(LaTexSymbol(0,"example1","\\frac{2}{3}"));
	symbolsList.push_back(LaTexSymbol(0, "example2", "\\int"));

	addAndMakeVisible(*table_ptr);
	table_ptr->update();

	addAndMakeVisible(tex_text);
	tex_text.addListener(this);

	addAndMakeVisible(xTextBox);
	xTextBox.addListener(this);
	xTextBox.setJustification(Justification::centredRight);

	xLabel.setText("x", dontSendNotification);
	xLabel.attachToComponent(&xTextBox, true);

	addAndMakeVisible(yTextBox);
	yTextBox.addListener(this);
	yTextBox.setJustification(Justification::centredRight);

	yLabel.setText("y", dontSendNotification);
	yLabel.attachToComponent(&yTextBox, true);

	tex_search.setText("Search:", dontSendNotification);
	tex_search.attachToComponent(&tex_text, false);

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
	else if (button == &arrowUp)
	{
		compile();
	}
	else if (button == &arrowDown)
	{
		compile();
	}
	else if (button == &arrowLeft)
	{
		compile();
	}
	else if (button == &arrowRight)
	{
		compile();
	}
}

void MainContentComponent::sliderValueChanged(Slider * slider)
{
	if (slider == &sizeSlider)
	{
		message = to_string(sizeSlider.getValue());

		repaint();
	}
	else if (slider == &rotationSlider)
	{
		message = to_string(rotationSlider.getValue());

		repaint();
	}
}

ApplicationCommandManager & MainContentComponent::getApplicationCommandManager()
{
	if (!applicationCommandManager)
		applicationCommandManager = make_unique<ApplicationCommandManager>();

	return *applicationCommandManager;
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

void MainContentComponent::add(LaTexSymbol newObject)
{
	symbolsList.push_back(newObject);

	table_ptr->update();
}

DrawablePath * MainContentComponent::create_triangle(Point<float> a, Point<float> b, Point<float> c)
{
	static DrawablePath triangle;

	Path p;
	p.addTriangle(a, b, c);
	triangle.setPath(p);
	triangle.setFill(findColour(0x1000202)); //Using standard IDs of Juce
	triangle.setStrokeFill(findColour(0x1000205));

	return &triangle;
}

ApplicationCommandTarget * MainContentComponent::getNextCommandTarget()
{
	// this will return the next parent component that is an ApplicationCommandTarget (in this
	// case, there probably isn't one, but it's best to use this method in your own apps).
	return findFirstTargetParentComponent();
}

void MainContentComponent::getAllCommands(Array<CommandID>& commands)
{
	const CommandID ids[] = {
								MainContentComponent::Save,
								MainContentComponent::Quit,
								MainContentComponent::New,
								MainContentComponent::Open,
								MainContentComponent::Export
							};

	commands.addArray(ids, numElementsInArray(ids));
}

void MainContentComponent::getCommandInfo(CommandID commandID, ApplicationCommandInfo & result)
{
	const String filecommands("File commands");

	switch (commandID)
	{
		case MainContentComponent::Save:
			result.setInfo("Save", "Saves the current project", filecommands, 0);
			result.addDefaultKeypress('s', ModifierKeys::commandModifier);
			break;

		case MainContentComponent::New:
			result.setInfo("New", "Opens new project", filecommands, 0);
			result.addDefaultKeypress('n', ModifierKeys::commandModifier);
			break;

		case MainContentComponent::Quit:
			result.setInfo("Quit", "Quits", filecommands, 0);
			result.addDefaultKeypress('n', ModifierKeys::commandModifier);
			break;

		case MainContentComponent::Open:
			result.setInfo("Open", "Open an existing project", filecommands, 0);
			result.addDefaultKeypress('o', ModifierKeys::commandModifier);
			break;

		case MainContentComponent::Export:
			result.setInfo("Export", "Export the created symbol", filecommands, 0);
			result.addDefaultKeypress('e', ModifierKeys::commandModifier);
			break;

		default:
			break;
	}
}

bool MainContentComponent::perform(const InvocationInfo & info)
{
	switch (info.commandID)
	{
		case Save:
			message = "Save";

			repaint();
			break;

		case New:
			message = "New";

			repaint();
			break;

		case Open:
			message = "Open";

			repaint();
			break;

		case Quit:
			message = "Quit";

			repaint();
			break;

		case Export:
			message = "Export";

			repaint();
			break;
	
		default:
			return false;
	}

	return true;
}

MainContentComponent::~MainContentComponent()
{
	applicationCommandManager.reset();
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
	menubar.setBounds(0, 0, getWidth(), 20);

	tex_image.setBounds(10, 30, getWidth() / 2 - 15, getHeight() / 2 - 15);
	tex_text.setBounds(getWidth() / 2 + 5, getHeight()*0.1, getWidth()/4 - 10, 25);
	compile_button.setBounds(getWidth()*0.75 + 5, getHeight()*0.1, getWidth()/4 - 10, 25);

	xTextBox.setBounds(getWidth() * 3 / 8 - 10, getHeight() - 90, getWidth() / 8 - 10, 25);
	yTextBox.setBounds(getWidth() * 3 / 8 - 10, getHeight() - 55, getWidth() / 8 - 10, 25);

	arrowUp.setBounds(70, getHeight() - 140, 50, 50);
	arrowDown.setBounds(70, getHeight() - 80, 50, 50);
	arrowLeft.setBounds(10, getHeight() - 80, 50, 50);
	arrowRight.setBounds(130, getHeight() - 80, 50, 50);

	auto sliderLeft = 80;
	sizeSlider.setBounds(sliderLeft, getHeight() / 2 + 20, getWidth()/2 - sliderLeft - 10, 20);
	rotationSlider.setBounds(sliderLeft, getHeight() / 2 + 50, getWidth()/2 - sliderLeft - 10, 20);

	table_ptr->setBounds(juce::Rectangle<int>(getWidth()/2 + 10, getHeight()/2 + 10, getWidth() / 2 - 20, getHeight() / 2 - 40));
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

LaTexSymbol::LaTexSymbol(int symbolID, std::string name, std::string LaTex, int x, int y, double rotAngle, double sizeRatio, bool selected)
{
	_symbolID = symbolID;
	_name = name;
	_LaTex = LaTex;
	_selected = selected;
	_x = x;
	_y = y;
	_rotAngle = rotAngle;
	_sizeRatio = sizeRatio;
}

bool LaTexSymbol::get_selected() const
{
	return _selected;
}

void LaTexSymbol::set_selected(const bool b)
{
	_selected = b;
}
