/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

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
											   arrowRight("arrowRight", DrawableButton::ImageOnButtonBackground)

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

ApplicationCommandManager & MainContentComponent::getApplicationCommandManager()
{
	if (applicationCommandManager == nullptr)
		applicationCommandManager = new ApplicationCommandManager();

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

void MainContentComponent::handleAsyncUpdate()
{
	// This registers all of our commands with the command manager but has to be done after the window has
	// been created so we can find the number of rendering engines available
	auto& commandManager = getApplicationCommandManager();

//	commandManager.registerAllCommandsForTarget(MainContentComponent);
//	commandManager.registerAllCommandsForTarget(JUCEApplication::getInstance());
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
	applicationCommandManager = nullptr;
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

	arrowUp.setBounds(70, getHeight() - 140, 50, 50);
	arrowDown.setBounds(70, getHeight() - 80, 50, 50);
	arrowLeft.setBounds(10, getHeight() - 80, 50, 50);
	arrowRight.setBounds(130, getHeight() - 80, 50, 50);
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