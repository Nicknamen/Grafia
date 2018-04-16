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

std::string eatRightZeros(std::string & input)
{
	for (int i = input.size() - 1; i != 0; --i) // apparently the string iterator is not deferentiable...
		if (input[i] == '0')
			input.erase(i);
		else if (input[i] == '.')
		{
			input.erase(i);

			break;
		}
		else
			break;

/*
	for (auto it = input.begin(); it != input.end();)
		if (*it == 0)
		{
			it = input.erase(it);
		}
		else
			--it;
*/
	return input;
}

//==============================================================================
MainContentComponent::MainContentComponent() : arrowUp("arrowUp", DrawableButton::ImageOnButtonBackground),
											   arrowDown("arrowDown", DrawableButton::ImageOnButtonBackground),
											   arrowLeft("arrowLeft", DrawableButton::ImageOnButtonBackground),
											   arrowRight("arrowRight", DrawableButton::ImageOnButtonBackground),
											   moveUp("moveSymbolUp", DrawableButton::ImageOnButtonBackground),
											   moveDown("armoveSymbolDown", DrawableButton::ImageOnButtonBackground),
											   table_ptr(new TableComponent(this))
{
	getLookAndFeel().setColour(Label::textColourId, Colours::black);
	getLookAndFeel().setColour(ToggleButton::ColourIds::textColourId, Colours::black);
	getLookAndFeel().setColour(ToggleButton::ColourIds::tickColourId, Colours::black);
	getLookAndFeel().setColour(ToggleButton::ColourIds::tickDisabledColourId, Colours::black);

	getLookAndFeel().setUsingNativeAlertWindows(true);

	ImageFileName = "tex_file";
	texstream.open(ImageFileName + ".tex");

	applicationCommandManager->registerAllCommandsForTarget(this); //registers the commands created by the user
	applicationCommandManager->registerAllCommandsForTarget(JUCEApplication::getInstance()); //registers the standard commands, such as quit.
	addAndMakeVisible(&menubar);

	addKeyListener(applicationCommandManager->getKeyMappings());

	addAndMakeVisible(compile_button);
	compile_button.setButtonText("Compile");
	compile_button.addListener(this);

	addAndMakeVisible(add_button);
	add_button.setButtonText("Add");
	add_button.addListener(this);

	addAndMakeVisible(remove_button);
	remove_button.setButtonText("Remove");
	remove_button.addListener(this);

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

	addAndMakeVisible(moveUp);
	addAndMakeVisible(moveDown);

	moveUp.setImages(create_triangle(Point<float>(5, 0), Point<float>(10, 10), Point<float>(0, 10)), 0, 0);
	moveDown.setImages(create_triangle(Point<float>(), Point<float>(10, 0), Point<float>(5, 10)), 0, 0);

	moveUp.addListener(this);
	moveDown.addListener(this);

	addAndMakeVisible(sizeSlider);
	sizeSlider.setRange(0.0, 100.0);
	sizeSlider.setSkewFactor(0.3);
	sizeSlider.addListener(this);
	sizeSlider.setValue(1.0);
	sizeSlider.setChangeNotificationOnlyOnRelease(true); // this might change when implementing with KaTex
	sizeSlider.setNumDecimalPlacesToDisplay(3);

	addAndMakeVisible(sizeLabel);
	sizeLabel.setText("Size:", dontSendNotification);
//	sizeLabel.setColour(100, findColour(0x1000200));
	sizeLabel.attachToComponent(&sizeSlider, true);

	addAndMakeVisible(rotationSlider);
	rotationSlider.setRange(0.0, 360.0);
	rotationSlider.setTextValueSuffix(CharPointer_UTF8("\xc2\xb0"));
	rotationSlider.addListener(this);
	rotationSlider.setChangeNotificationOnlyOnRelease(true); // this might change when implementing with KaTex
	rotationSlider.setNumDecimalPlacesToDisplay(3);

	addAndMakeVisible(rotationLabel);
	rotationLabel.setText("Rotation:", dontSendNotification);
	rotationLabel.attachToComponent(&rotationSlider, true);

	symbolsList.push_back(LaTexSymbol("Freccia a destra", "\\longrightarrow"));
	symbolsList.push_back(LaTexSymbol("Integrale", "\\int", 1.5, -1.5, 60, 2));

	addAndMakeVisible(*table_ptr);
	table_ptr->update();

	addAndMakeVisible(tex_text);
	tex_text.addListener(this);

	addAndMakeVisible(symbolNameEditor);
	symbolNameEditor.addListener(this);
	juce::Font symbolNameFont = symbolNameEditor.getFont();
	symbolNameFont.setItalic(true);
	symbolNameEditor.setFont(symbolNameFont);
	symbolNameEditor.setText(newSymbolName, dontSendNotification);

	addAndMakeVisible(symbolNameLabel);
	symbolNameLabel.setText("Symbol name:", dontSendNotification);
	symbolNameLabel.attachToComponent(&symbolNameEditor, true);

	addAndMakeVisible(xTextBox);
	xTextBox.addListener(this);
	xTextBox.setJustification(Justification::centredRight);
	xTextBox.setInputRestrictions(10, "0123456789+-,.");

	xLabel.setText("x:", dontSendNotification);
	xLabel.attachToComponent(&xTextBox, true);

	addAndMakeVisible(yTextBox);
	yTextBox.addListener(this);
	yTextBox.setJustification(Justification::centredRight);
	yTextBox.setInputRestrictions(10, "0123456789+-,.");

	yLabel.setText("y:", dontSendNotification);
	yLabel.attachToComponent(&yTextBox, true);

	tex_search.setText("LaTex code:", dontSendNotification);
	tex_search.attachToComponent(&tex_text, false);

	addAndMakeVisible(compileAtEachCommand);
	compileAtEachCommand.setButtonText("Compile at each command");

	addAndMakeVisible(tex_image);

	setSize(800, 450);

	setMessage(""); //not working
}

void MainContentComponent::buttonClicked(Button* button)
{
	const double raiseValue = 1;

	if (button == &compile_button)
	{
		compile();

		tex_image.repaint();

		repaint();
	}
	else if (button == &add_button)
	{
		add(LaTexSymbol(tex_text.getText().toStdString(), tex_text.getText().toStdString()));
	}
	else if (button == &remove_button)
	{
		remove();
	}
	else if (button == &arrowUp)
	{
		raisey(raiseValue);

		update_displayed();
		if (compileAtEachCommand.getToggleState())
			compile();
	}
	else if (button == &arrowDown)
	{
		raisey(-raiseValue);

		update_displayed();
		if (compileAtEachCommand.getToggleState())
			compile();
	}
	else if (button == &arrowLeft)
	{
		raisex(-raiseValue);

		update_displayed();
		if (compileAtEachCommand.getToggleState())
			compile();
	}
	else if (button == &arrowRight)
	{
		raisex(raiseValue);

		update_displayed();
		if (compileAtEachCommand.getToggleState())
			compile();
	}
	else if (button == &moveUp)
	{
		moveSymbolUp();
	}
	else if (button == &moveDown)
	{
		moveSymbolDown();
	}
}

void MainContentComponent::sliderValueChanged(Slider * slider)
{
	if (slider == &sizeSlider)
	{
		if (selected_symbol == nullptr)
			setMessage("No symbol is being visualized: no size to be changed");
		else
		{
			double previous_value = selected_symbol->getSizeRatio();

			for (auto & symbol : symbolsList)
				if (symbol.is_selected())
					symbol.setSizeRatio((symbol.getSizeRatio() / previous_value) * sizeSlider.getValue());

			if (compileAtEachCommand.getToggleState())
				compile();
		}
	}
	else if (slider == &rotationSlider)
	{
		if (selected_symbol == nullptr)
			setMessage("No symbol is being visualized: no rotation to be changed");
		else
		{
			double previous_value = selected_symbol->getRotAngle();

			for (auto & symbol : symbolsList)
				if (symbol.is_selected())
					symbol.setRotAngle((symbol.getRotAngle() - previous_value) + rotationSlider.getValue());

			if (compileAtEachCommand.getToggleState())
				compile();
		}
	}
	
}

void MainContentComponent::textEditorTextChanged(TextEditor & textEditor)
{
	try
	{
		if (&textEditor == &xTextBox)
		{
			if (selected_symbol == nullptr)
			{
				setMessage("No symbol is being displayed. No x value to be changed");

				xTextBox.setText("");
			}
			else
			{
				double previous_value = selected_symbol->getx();

				for (auto & symbol : symbolsList)
					if (symbol.is_selected())
						symbol.setx((symbol.getx() - previous_value)
							+ stod(xTextBox.getText().toStdString() == "" ? "0" : xTextBox.getText().toStdString()));

				if (compileAtEachCommand.getToggleState())
					compile();

				message = "";
			}
		}
		else if (&textEditor == &yTextBox)
		{
			if (selected_symbol == nullptr)
			{
				message = "No symbol is being displayed. No y value to be changed";

				yTextBox.setText("");
			}
			else
			{
				double previous_value = selected_symbol->gety();

				for (auto & symbol : symbolsList)
					if (symbol.is_selected())
						symbol.sety((symbol.gety() - previous_value)
							+ stod(yTextBox.getText().toStdString() == "" ? "0" : yTextBox.getText().toStdString()));

				if (compileAtEachCommand.getToggleState())
					compile();

				message = "";
			}
		}
		else if (&textEditor == &symbolNameEditor)
		{
			newSymbolName = symbolNameEditor.getText().toStdString();
		}

		repaint();
	}
	catch (exception & exc)
	{
		errorAlert(exc);
	}
}

void MainContentComponent::textEditorReturnKeyPressed(TextEditor & textEditor)
{
	try
	{
		if (&textEditor == &tex_text)
		{
			add(LaTexSymbol(tex_text.getText().toStdString(), tex_text.getText().toStdString()));

			tex_text.setText("");
		}
	}
	catch (exception & error_)
	{
		message = error_.what();

		repaint();
	}

}

void MainContentComponent::update_displayed()
{
	if (selected_symbol != nullptr)
	{
		rotationSlider.setValue(selected_symbol->getRotAngle());
		sizeSlider.setValue(selected_symbol->getSizeRatio());
		xTextBox.setText(eatRightZeros(to_string(selected_symbol->getx())));
		yTextBox.setText(eatRightZeros(to_string(selected_symbol->gety())));
	}

	table_ptr->update();

	repaint();
}

void MainContentComponent::zero_displayed()
{
	selected_symbol = nullptr;

	rotationSlider.setValue(0);
	sizeSlider.setValue(1);
	xTextBox.setText("0");
	yTextBox.setText("0");
}

ApplicationCommandManager & MainContentComponent::getApplicationCommandManager()
{
	if (!applicationCommandManager)
		applicationCommandManager = make_unique<ApplicationCommandManager>();

	return *applicationCommandManager;
}

void MainContentComponent::exportSymbol()
{
	{
		fc.reset(new FileChooser("Export as .tex file",
			File::getCurrentWorkingDirectory().getChildFile(StringRef(newSymbolName)),
			"*.tex"));

		fc->launchAsync(FileBrowserComponent::saveMode | FileBrowserComponent::canSelectFiles,
			[&](const FileChooser& chooser)
		{
			auto result = chooser.getURLResult();
			string name = result.isEmpty() ? string()
										   : (result.isLocalFile() ? result.getLocalFile().getFullPathName().toStdString()
																   : result.toString(true).toStdString());

			TeX saveSymbol(name);
			saveSymbol << "\\documentclass{minimal}\n\n"
				"\\usepackage{graphicx, amsmath, amssymb}\n";

			writeSymbolTeXCode(saveSymbol);

			saveSymbol << "\\begin{document}\n"
				"$$\\" + newSymbolName + "$$\n"
				"\\end{document}";

			saveSymbol.do_not_cancel("tex", "log");
		});
	}
}

void MainContentComponent::runSettings()
{
#if JUCE_MODAL_LOOPS_PERMITTED
	AlertWindow w("Settings",
		"The following parameters control the functionalities of this application.",
		AlertWindow::NoIcon);

	w.addTextEditor("image_density", to_string(texstream.get_image_density()), "Image density:");
	w.addComboBox("slidersDigits", { "1", "2", "3", "4", "5", "6", "7", "8", "9" }, "Sliders decimal digits number");
	w.getComboBoxComponent("slidersDigits")->setSelectedItemIndex(mySlider::slidersDigitsNum - 1);
	w.addButton("OK", 1, KeyPress(KeyPress::returnKey, 0, 0));
	w.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey, 0, 0));

	w.getTextEditor("image_density")->setInputRestrictions(5, "0123456789+-,.");

	if (w.runModalLoop() != 0) // is they picked 'ok'
	{
		// this is the item they chose in the drop-down list..
		int num = w.getComboBoxComponent("slidersDigits")->getSelectedItemIndex();
		if (num != -1)
		{
			mySlider::slidersDigitsNum = num + 1;

			sizeSlider.setNumDecimalPlacesToDisplay(mySlider::slidersDigitsNum);
			rotationSlider.setNumDecimalPlacesToDisplay(mySlider::slidersDigitsNum);
		}

		// this is the text they entered..
		texstream.set_image_density(stoi(w.getTextEditorContents("image_density").toStdString()));
	}
#endif
}

void MainContentComponent::writeSymbolTeXCode(TeX & texStream)
{
	texStream << "\\newcommand{\\" + newSymbolName + "}{\\mathbin{\\ooalign{\n"
		"	\\rotatebox[origin=c]{" + eatRightZeros(to_string(symbolsList[0].getRotAngle()))
		+ "}{\\scalebox{" + eatRightZeros(to_string(symbolsList[0].getSizeRatio()))
		+ "}{$" + symbolsList[0].getLaTex() + "$}}\\cr\n"; //The first symbol is dominant

	for (auto it = symbolsList.begin() + 1; it != symbolsList.end(); ++it)
	{
		double x = it->getx();
		double y = it->gety();

		texStream << "	\\hidewidth\\kern" + eatRightZeros(to_string(x)) + "pt\\raise" + eatRightZeros(to_string(y))
			+ "pt\\hbox{\\rotatebox[origin=c]{" + eatRightZeros((to_string(it->getRotAngle()))) + "}{\\scalebox{"
			+ eatRightZeros(to_string(it->getSizeRatio())) + "}{$" + it->getLaTex() + "$}}}\\hidewidth\\cr\n";
	}

	texStream << "}}}\n\n";
}

void MainContentComponent::compile()
{
	try
	{
		setMessage("Writing file"); // non funziona

		texstream.open_rewritemode();

		texstream << "\\documentclass{minimal}\n\n"
			"\\usepackage[paperwidth=6.4cm, paperheight=3.6cm]{geometry}\n"
			"\\usepackage{graphicx, amsmath, amssymb}\n";

		writeSymbolTeXCode(texstream);
		
		texstream << "\\begin{document}\n"
			"$$\\" + newSymbolName + "$$\n"
			"\\end{document}";

		setMessage("Compiling...");
		repaint();

		texstream.to_png();

		File teximage(File::getCurrentWorkingDirectory().getChildFile(String(ImageFileName + ".png")));

		setMessage("Processing picture");

		Image tex_preimage = PNGImageFormat::loadFrom(teximage);

		if (!tex_preimage.isValid())
		{
			throw GrafiaException("Image is not valid");
		}

		tex_image.setImage(tex_preimage);

		update_displayed();

		setMessage("Compiled succesfully");
	}
	catch (exception& exc)
	{
		errorAlert(exc);
	}
}

void MainContentComponent::add(LaTexSymbol newObject)
{
	symbolsList.push_back(newObject);

	tex_text.setText("");

	table_ptr->update();
}

void MainContentComponent::remove()
{
	for (auto it = symbolsList.begin(); it != symbolsList.end();)
		if (it->is_selected())
		{
			if (selected_symbol != nullptr)
				if (*it == *selected_symbol)
					zero_displayed();

			it = symbolsList.erase(it);
		}
		else
			++it;

	table_ptr->update();
}

void MainContentComponent::reset()
{
	zero_displayed();

	symbolsList.clear();

	table_ptr->update();

	setNewSymbolName("newSymbol");

	tex_text.setText("", dontSendNotification);

	setMessage("");

	message;

	Image a;

	tex_image.setImage(a);
}

void MainContentComponent::moveSymbolUp()
{
	if (selected_symbol == nullptr)
		setMessage("No symbol selected");
	else
	{
		int i = 0;

		for (auto symbol : symbolsList)
		{
			if (symbol == *selected_symbol)
				break;

			++i;
		}

		if (i != 0)
		{
			LaTexSymbol temp = symbolsList[i - 1];
			symbolsList[i - 1] = symbolsList[i];
			symbolsList[i] = temp;

			selected_symbol = &symbolsList[i - 1];

			table_ptr->forceRowSelected(i - 1);
		}

		update_displayed();

		setMessage("");
	}
}

void MainContentComponent::moveSymbolDown()
{
	if (selected_symbol == nullptr)
		setMessage("No symbol selected");
	else
	{
		int i = 0;

		for (auto symbol : symbolsList)
		{
			if (symbol == *selected_symbol)
				break;

			++i;
		}

		if (i != symbolsList.size() - 1)
		{
			LaTexSymbol temp = symbolsList[i + 1];
			symbolsList[i + 1] = symbolsList[i];
			symbolsList[i] = temp;

			selected_symbol = &symbolsList[i + 1];

			table_ptr->forceRowSelected(i + 1);
		}

		setMessage("");
	}
}

void MainContentComponent::raisex(const double x)
{
	for (auto & symbol : symbolsList)
		if (symbol.is_selected())
			symbol.setx(symbol.getx() + x);
}

void MainContentComponent::raisey(const double y)
{
	for (auto & symbol : symbolsList)
		if (symbol.is_selected())
			symbol.sety(symbol.gety() + y);
}

void MainContentComponent::scale(double factor)
{
	for (auto & symbol : symbolsList)
		if (symbol.is_selected())
			symbol.setSizeRatio(symbol.getSizeRatio()*factor);
}

void MainContentComponent::rotate(double angle)
{
	for (auto & symbol : symbolsList)
		if (symbol.is_selected())
			symbol.setRotAngle(symbol.getRotAngle() + angle);
}

void MainContentComponent::errorAlert(const std::exception & exc)
{
	setMessage("Error caught during compilation");

	AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Error", String(exc.what()));
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
								MainContentComponent::New,
								MainContentComponent::Open,
								MainContentComponent::Export,
								MainContentComponent::Settings,
								MainContentComponent::About
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

		case MainContentComponent::Open:
			result.setInfo("Open", "Open an existing project", filecommands, 0);
			result.addDefaultKeypress('o', ModifierKeys::commandModifier);
			break;

		case MainContentComponent::Export:
			result.setInfo("Export", "Export the created symbol", filecommands, 0);
			result.addDefaultKeypress('e', ModifierKeys::commandModifier);
			break;

		case MainContentComponent::Settings:
			result.setInfo("Settings", "Runs a settings window", filecommands, 0);
			result.addDefaultKeypress('j', ModifierKeys::commandModifier);
			break;

		case MainContentComponent::About:
			result.setInfo("About Grafia", "Shows informations about the program", filecommands, 0);
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
			reset();
			break;

		case Open:
			message = "Open";

			repaint();
			break;

		case Export:
			exportSymbol();
			break;

		case Settings:
			runSettings();
			break;

		case About:
			AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "About Grafia",
				String("Grafia " + string(SOFTWARE_VERSION) + "\n") +
				String(CharPointer_UTF8("\xc2\xa9")) + String(" 2018\n"
				"Written and designed by Nicol") + String(CharPointer_UTF8("\xc3\xb2")) + String(" Cavalleri\n"
				"University of Milan\n"
				"https://github.com/Nicknamen/Grafia"));
			break;
	
		default:
			return false;
	}

	return true;
}

void MainContentComponent::setMessage(std::string to_be_written)
{
	message = to_be_written;

	repaint();
}

void MainContentComponent::setNewSymbolName(std::string newName)
{
	newSymbolName = newName;

	symbolNameEditor.setText(newName, dontSendNotification);
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
	tex_text.setBounds(getWidth() / 2 + 5, 50, getWidth()/4 - 10, 25);
	compile_button.setBounds(getWidth()*0.75 + 5, 80, getWidth() / 4 - 10, 25);

	symbolNameEditor.setBounds(getWidth()*0.75 + 5, 110, getWidth() / 4 - 10, 25);

	add_button.setBounds(getWidth()*0.75 + 5, 50, getWidth() / 4 - 10, 25);
	remove_button.setBounds(getWidth() * 5 / 6 + 5, getHeight() / 2 + - 25, getWidth() / 6 - 10, 25);

	xTextBox.setBounds(getWidth() * 3 / 8 - 10, getHeight() - 90, getWidth() / 8 - 10, 25);
	yTextBox.setBounds(getWidth() * 3 / 8 - 10, getHeight() - 55, getWidth() / 8 - 10, 25);

	arrowUp.setBounds(70, getHeight() - 140, 50, 50);
	arrowDown.setBounds(70, getHeight() - 80, 50, 50);
	arrowLeft.setBounds(10, getHeight() - 80, 50, 50);
	arrowRight.setBounds(130, getHeight() - 80, 50, 50);

	moveUp.setBounds(getWidth() / 2 + 5, getHeight() / 2 + -25, getWidth() / 6 - 10, 25);
	moveDown.setBounds(getWidth() * 2 / 3 + 5, getHeight() / 2 + -25, getWidth() / 6 - 10, 25);

	compileAtEachCommand.setBounds(getWidth() / 2 + 5, 80, getWidth() / 4 - 10, 25);

	int sliderLeft = 80;
	sizeSlider.setBounds(sliderLeft, getHeight() / 2 + 20, getWidth()/2 - sliderLeft - 10, 20);
	rotationSlider.setBounds(sliderLeft, getHeight() / 2 + 50, getWidth()/2 - sliderLeft - 10, 20);

	table_ptr->setBounds(juce::Rectangle<int>(getWidth()/2 + 10, getHeight()/2 + 10, getWidth() / 2 - 20, getHeight() / 2 - 40));
}

void LatexDisplay::paint(Graphics & g)
{
	g.fillAll(Colours::white);

	juce::Rectangle<float> bounds(0, 0, getWidth(), getHeight());

	g.drawImage(getImage(), bounds);

	g.setColour(Colours::darkgrey);

	g.drawRect(bounds);
}

int LaTexSymbol::_symbolCount = 0;

LaTexSymbol::LaTexSymbol(std::string name, std::string LaTex, double x, double y, double rotAngle, double sizeRatio, bool selected):
	_symbolID(_symbolCount++)
{
	_name = name;
	_LaTex = LaTex;
	_selected = selected;
	_x = x;
	_y = y;
	_rotAngle = rotAngle;
	_sizeRatio = sizeRatio;
}

LaTexSymbol & LaTexSymbol::operator=(const LaTexSymbol & other)
{
	_name = other._name;
	_LaTex = other._LaTex;
	_x = other._x;
	_y = other._y;
	_rotAngle = other._rotAngle;
	_sizeRatio = other._sizeRatio;
	_selected = other._selected;
	return *this;
}

LaTexSymbol::LaTexSymbol(const LaTexSymbol & other):
	_symbolID(other._symbolID),
	_name(other._name),
	_LaTex(other._LaTex),
	_x(other._x),
	_y(other._y),
	_rotAngle(other._rotAngle),
	_sizeRatio(other._sizeRatio),
	_selected(other._selected)
{
}

bool LaTexSymbol::operator==(const LaTexSymbol & other) const
{
	if (_symbolID == other._symbolID &&
		_name == other._name &&
		_LaTex == other._LaTex &&
		_x == other._x &&
		_y == other._y &&
		_rotAngle == other._rotAngle &&
		_sizeRatio == other._sizeRatio)
		return true;
	else
		return false;
}

bool LaTexSymbol::operator!=(const LaTexSymbol & other) const
{
	return !operator==(other);
}

std::string LaTexSymbol::getAttributeTextbyID(int id) const
{
	if (id == symbolID_id)
		return to_string(_symbolID);
	else if (id == name_id)
		return _name;
	else if (id == LaTex_id)
		return _LaTex;
	else if (id == selected_id)
		return _selected ? "Y" : "N";
	else
		return{};
}

void LaTexSymbol::setAttributebyID(int id, std::string text)
{
	if (id == name_id)
		_name = text;
	else if (id == LaTex_id)
		_LaTex = text;
	else if (id == selected_id)
		_selected = ((text == "Y") ? 1 : 0);
}

void LaTexSymbol::setAttributebyID(int id, double value)
{
	if (id == selected_id)
		_selected = value;
	else if (id == x_id)
		_x = value;
	else if (id == y_id)
		_y = value;
	else if (id == rotAngle_id)
		_rotAngle = value;
	else if (id == sizeRatio_id)
		_sizeRatio = value;
}

bool LaTexSymbol::is_selected() const
{
	return _selected;
}

void LaTexSymbol::set_selected(const bool b)
{
	_selected = b;
}

std::string LaTexSymbol::getLaTex() const
{
	return _LaTex;
}

double LaTexSymbol::getx() const
{
	return _x;
}

void LaTexSymbol::setx(const double x)
{
	_x = x;
}

double LaTexSymbol::gety() const
{
	return _y;
}

void LaTexSymbol::sety(const double y)
{
	_y = y;
}

double LaTexSymbol::getSizeRatio() const
{
	return _sizeRatio;
}

void LaTexSymbol::setSizeRatio(const double size_ratio)
{
	_sizeRatio = size_ratio;
}

double LaTexSymbol::getRotAngle() const
{
	return _rotAngle;
}

void LaTexSymbol::setRotAngle(const double rotation_angle)
{
	_rotAngle = rotation_angle;
}

int MainContentComponent::mySlider::slidersDigitsNum = 3;

String MainContentComponent::mySlider::getTextFromValue(double value)
{
	if (textFromValueFunction != nullptr)
		return textFromValueFunction(value);

	if (getNumDecimalPlacesToDisplay() > 0)
		return eatRightZeros((String(value, getNumDecimalPlacesToDisplay())).toStdString()) + getTextValueSuffix();

	return String(roundToInt(value)) + getTextValueSuffix();
}
