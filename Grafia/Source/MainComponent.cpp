/*
  ==============================================================================

    This file was auto-generated!

	MainComponent.cpp
	Project:	Grafia
	Created:	17 Apr 2017
	Author:		Nicolo' Cavalleri
	Github:		Nicknamen
	Link:		https://github.com/Nicknamen/Grafia

  ==============================================================================
*/

#include "TableComponent.h"
#include "MainComponent.h"

//includes necessary only for the implementation
#include <regex>
#include <algorithm>

#ifdef __linux__
#include <locale>
#endif

using namespace std;

inline Colour getRandomColour(float brightness)
{
	return Colour::fromHSV(Random::getSystemRandom().nextFloat(), 0.5f, brightness, 1.0f);
}

inline Colour getRandomBrightColour() { return getRandomColour(0.8f); }
inline Colour getRandomDarkColour() { return getRandomColour(0.3f); }

std::string cleanstring(std::string input)	//cleans a path string. It looks paths passed to the command line when doubleclicking 
{											//the file contain some characters which do not allow opening it.
	regex badchar("[^!#-~]");	//insert here any char that is allowed in a path

	return regex_replace(input, badchar, "");
}

std::string eatRightZeros(std::string & input)	//juce fills everything with useless zeros. I use this function anywhere
{												//to clean number strings
	bool is_it_decimal = false;

	for (auto it = input.begin(); it != input.end(); ++it)
		if (*it == '.' || *it == ',')			//linux makes strange conversions of '.' to ','
		{
			*it = '.';
			is_it_decimal = true;				//don't want to eat zeros in 1000

			break;
		}

	if (is_it_decimal)
		for (int i = input.size() - 1; i != 0; --i)
			if (input[i] == '0')
				input.erase(i);		//erases 0
			else if (input[i] == '.')
			{
				input.erase(i);		//erases the .

				break;
			}
			else
				break;

	return input;
}

//need it to work with temporaries
std::string eatRightZeros(std::string && input)	//to make it work with to_string() output
{
	bool is_it_decimal = false;

	for (auto it = input.begin(); it != input.end(); ++it)
		if (*it == '.' || *it == ',')
		{
			*it = '.';
			is_it_decimal = true;

			break;
		}

	if (is_it_decimal)
		for (int i = input.size() - 1; i != 0; --i)
			if (input[i] == '0')
				input.erase(i);
			else if (input[i] == '.')
			{
				input.erase(i);

				break;
			}
			else
				break;

	return input;
}

string getNextData(string & dataString)	//this function parses lines of .grproj files by knowing the way I invented it
{										//I assume the user does not insert anything like \&/
	auto it = dataString.begin();

	while (it != dataString.end() && !(*it == '\\' && *(it + 1) == '&' && *(it + 2) == '/'))
		++it;

	string data = string(dataString.begin(), it);

	dataString.erase(dataString.begin(), it + 3);

	return data;
}

//==============================================================================
MainContentComponent::MainContentComponent() : arrowUp("arrowUp", DrawableButton::ImageOnButtonBackground),
											   arrowDown("arrowDown", DrawableButton::ImageOnButtonBackground),
											   arrowLeft("arrowLeft", DrawableButton::ImageOnButtonBackground),
											   arrowRight("arrowRight", DrawableButton::ImageOnButtonBackground),
											   moveUp("moveSymbolUp", DrawableButton::ImageOnButtonBackground),
											   moveDown("armoveSymbolDown", DrawableButton::ImageOnButtonBackground),
											   table_ptr(new TableComponent(this)),	//need to initialize it here!!
											   menubar(new MenuComponent)
{
#ifdef __linux__
	setlocale(LC_ALL, "C");
#endif

	getLookAndFeel().setColour(Label::textColourId, Colours::black);
	getLookAndFeel().setColour(ToggleButton::ColourIds::textColourId, Colours::black);
	getLookAndFeel().setColour(ToggleButton::ColourIds::tickColourId, Colours::black);
	getLookAndFeel().setColour(ToggleButton::ColourIds::tickDisabledColourId, Colours::black);

	getLookAndFeel().setUsingNativeAlertWindows(true);

	addAndMakeVisible(message);

	ImageFileName = "tex_file";
	texstream.open(ImageFileName + ".tex");

	applicationCommandManager->registerAllCommandsForTarget(this); //registers the commands created by the user
	applicationCommandManager->registerAllCommandsForTarget(JUCEApplication::getInstance()); //registers the standard commands, such as quit.
	addAndMakeVisible(*menubar);

	addKeyListener(applicationCommandManager->getKeyMappings()); //not working yet, need to look closer the demo on this

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

	//I'm creating the arrows images with create_triangle function
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
	sizeSlider.setRange(0.01, 10.0);
	sizeSlider.setSkewFactor(0.3); //not really handy but it's cool
	sizeSlider.addListener(this);
	sizeSlider.setValue(1.0, dontSendNotification); //as 1 is the identity of (R,*) group
	sizeSlider.setChangeNotificationOnlyOnRelease(true);	//to avoid obvious problems with compileAtEachCommand
															//this might change when implementing with KaTex
	sizeSlider.setNumDecimalPlacesToDisplay(3); //this can be modified with settings

	addAndMakeVisible(sizeLabel);
	sizeLabel.setText("Size:", dontSendNotification);
	sizeLabel.attachToComponent(&sizeSlider, true);

	addAndMakeVisible(rotationSlider);
	rotationSlider.setRange(0.0, 360.0);
	rotationSlider.setTextValueSuffix(CharPointer_UTF8("\xc2\xb0")); //adds ° at the end of number
	rotationSlider.addListener(this);
	rotationSlider.setChangeNotificationOnlyOnRelease(true); //same as sizeSlider. this might change when implementing with KaTex
	rotationSlider.setNumDecimalPlacesToDisplay(3);

	addAndMakeVisible(rotationLabel);
	rotationLabel.setText("Rotation:", dontSendNotification);
	rotationLabel.attachToComponent(&rotationSlider, true);

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
	xTextBox.setInputRestrictions(10, "0123456789+-,.");	//I allow only numeric values in this textboxes

	xLabel.setText("x:", dontSendNotification);
	xLabel.attachToComponent(&xTextBox, true);

	addAndMakeVisible(yTextBox);
	yTextBox.addListener(this);
	yTextBox.setJustification(Justification::centredRight);
	yTextBox.setInputRestrictions(10, "0123456789+-,.");

	yLabel.setText("y:", dontSendNotification);
	yLabel.attachToComponent(&yTextBox, true);

	tex_search.setText("LaTeX code:", dontSendNotification);
	tex_search.attachToComponent(&tex_text, false);

	addAndMakeVisible(compileAtEachCommand);
	compileAtEachCommand.setButtonText("Compile at each command");

	addAndMakeVisible(tex_image);

	setSize(800, 450);

	message.set("Ready");
}

MainContentComponent::MainContentComponent(const String & commandline) : MainContentComponent()
{
	try
	{
		open(cleanstring(commandline.toStdString()));	//I'm assuming to receive only one argument.
	}													//the open() will generate an error otherwise
	catch (exception & exc)
	{
		errorAlert(exc);
	}
}

void MainContentComponent::buttonClicked(Button* button)
{
	const double raiseValue = 1; //how much the arrows displace the object

	try
	{
		if (button == &compile_button)
		{
			compile();

			tex_image.repaint();

			repaint();
		}
		else if (button == &add_button)
		{
			string LaTeXt = tex_text.getText().toStdString();
			if (!LaTeXt.empty()) //these lines eliminate an eventual \ at the beginning for the name
				add(LaTeXSymbol(LaTeXt[0] == '\\' ? string(LaTeXt.begin() + 1, LaTeXt.end())
					: LaTeXt, LaTeXt));
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
	catch (const exception& exc)
	{
		errorAlert(exc);
	}	
}

void MainContentComponent::sliderValueChanged(Slider * slider)
{
	if (slider == &sizeSlider)
	{
		if (selected_symbol == nullptr)	//if nothing has been yet selected
			message.set("No symbol is being visualized: no size to be changed");
		else
		{
			double previous_value = selected_symbol->getSizeRatio();

			for (auto & symbol : symbolsList)	//this is very important. It increases the size of every selected symbol
				if (symbol.is_selected())		//proportionally, respecting the * size group operation
					symbol.setSizeRatio((symbol.getSizeRatio() / previous_value) * sizeSlider.getValue());

			if (compileAtEachCommand.getToggleState())
				compile();
		}
	}
	else if (slider == &rotationSlider)
	{
		if (selected_symbol == nullptr)
			message.set("No symbol is being visualized: no rotation to be changed");
		else
		{
			double previous_value = selected_symbol->getRotAngle();

			for (auto & symbol : symbolsList)	//same as sizeSlider but with sum instead of product
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
			if (selected_symbol == nullptr)	//to avoid that the user believes to change something which does not
			{
				message.set("No symbol is being displayed. No x value to be changed");

				xTextBox.setText("");
			}
			else
			{
				double previous_value = selected_symbol->getx();

				for (auto & symbol : symbolsList) //always the same paradigm: traslates everything by the same amount
					if (symbol.is_selected())
						symbol.setx((symbol.getx() - previous_value) //treats as 0 the empty box
							+ stod(xTextBox.getText().toStdString() == "" ? "0" : xTextBox.getText().toStdString()));

				if (compileAtEachCommand.getToggleState())
					compile();

				message.set("");
			}
		}
		else if (&textEditor == &yTextBox)
		{
			if (selected_symbol == nullptr)
			{
				message.set("No symbol is being displayed. No y value to be changed");

				yTextBox.setText("");
			}
			else
			{
				double previous_value = selected_symbol->gety();

				for (auto & symbol : symbolsList)
					if (symbol.is_selected()) //everything as in xTextBox
						symbol.sety((symbol.gety() - previous_value)
							+ stod(yTextBox.getText().toStdString() == "" ? "0" : yTextBox.getText().toStdString()));

				if (compileAtEachCommand.getToggleState())
					compile();

				message.set("");
			}
		}
		else if (&textEditor == &symbolNameEditor) //sets the name of the command
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
		if (&textEditor == &tex_text) // basically the same as for the add button
		{
			string LaTeXt = tex_text.getText().toStdString();
			if (!LaTeXt.empty())
				add(LaTeXSymbol(LaTeXt[0] == '\\' ? string(LaTeXt.begin() + 1, LaTeXt.end())
													: LaTeXt, LaTeXt));
		}
	}
	catch (exception & exc)
	{
		errorAlert(exc);
	}

}

void MainContentComponent::update()
{
	update_displayed();

	table_ptr->update();

	symbolNameEditor.setText(newSymbolName);

	tex_text.setText("");

	message.set("");

	repaint();
}

void MainContentComponent::update_displayed()
{
	if (selected_symbol != nullptr) //only make sense if something is being displayed
	{
		rotationSlider.setValue(selected_symbol->getRotAngle());
		sizeSlider.setValue(selected_symbol->getSizeRatio());
		xTextBox.setText(eatRightZeros(to_string(selected_symbol->getx())));
		yTextBox.setText(eatRightZeros(to_string(selected_symbol->gety())));
	}
}

void MainContentComponent::zero_displayed()
{
	selected_symbol = nullptr;

	rotationSlider.setValue(0, dontSendNotification);
	sizeSlider.setValue(1, dontSendNotification);
	xTextBox.setText("", dontSendNotification);
	yTextBox.setText("", dontSendNotification);
}

ApplicationCommandManager & MainContentComponent::getApplicationCommandManager()
{
	if (!applicationCommandManager)
		applicationCommandManager = make_unique<ApplicationCommandManager>();

	return *applicationCommandManager;
}

void MainContentComponent::exportSymbol()
{
	fc.reset(new FileChooser("Export as .tex file",
		File::getCurrentWorkingDirectory().getChildFile(StringRef(newSymbolName + ".tex")),
		"*.tex"));

	fc->launchAsync(FileBrowserComponent::saveMode | FileBrowserComponent::canSelectFiles,
		[&](const FileChooser& chooser)
	{
		auto result = chooser.getURLResult();
		string name = result.isEmpty() ? string() //don't exactly know what's happening. Copied from demo
										: (result.isLocalFile() ? result.getLocalFile().getFullPathName().toStdString()
																: result.toString(true).toStdString());

		if (!name.empty() && overwriteExistingFile(name))
		{
			//I'm exporting just the very necessary to compile the new command in tex
			TeX saveSymbol(name);
			saveSymbol << "\\documentclass{minimal}\n\n"
				"\\usepackage{graphicx, amsmath, amssymb}\n\n";

			saveSymbol << "%The following code is the one defining the symbol " + newSymbolName + "\n"
				"%You can copy and paste it in your own TeX articles and use the symbol as shown in this file.\n";

			writeSymbolTeXCode(saveSymbol);

			saveSymbol << "\\begin{document}\n"
				"$$\\" + newSymbolName + "$$\n"
				"\\end{document}";

			saveSymbol.do_not_cancel("tex");	//to avoid it being cancel if the user saves it in the exe path.
												//line to be removed when installing Grafia as a proper program
		}
	});
}

void MainContentComponent::save()
{
	ofstream saveSymbolproj(projectPath, ios::trunc); //gonna overwrite everything

	if (!saveSymbolproj.good())
		throw GrafiaException("Not able to properly create the project file.");

	saveSymbolproj << newSymbolName + "\\&/" + to_string(texstream.get_image_density()) + "\\&/"
		+ to_string(mySlider::slidersDigitsNum) + "\\&/" << endl;	//writing the particular format that the function
																	//getNextData can read.
	for (auto symbol : symbolsList)
	{
		for (int i = 1; i < LaTeXSymbol::stop; i++)
			saveSymbolproj << symbol.getAttributeTextbyID(i) << "\\&/";

		saveSymbolproj << endl;
	}

	if (!saveSymbolproj.good())										//checks for error in the written file
		throw GrafiaException("Not able to properly save the project file.");

	saveSymbolproj.close();

	saved = true;

	message.set("Project succesfully saved");
}

void MainContentComponent::saveAs()
{
	fc.reset(new FileChooser("Save symbol project",
		projectPath.empty() ? File::getCurrentWorkingDirectory().getChildFile(StringRef(newSymbolName + ".grproj"))
							: File(projectPath),
		"*.grproj"));

	fc->launchAsync(FileBrowserComponent::saveMode | FileBrowserComponent::canSelectFiles,
		[&](const FileChooser& chooser)
	{
		URL result = chooser.getURLResult();

		string name = result.isEmpty() ? string()
			: (result.isLocalFile() ? result.getLocalFile().getFullPathName().toStdString()
				: result.toString(true).toStdString());

		if (name != "" && overwriteExistingFile(name))	//if the file already exists the user is prompted to choose wether to
		{												//overwrite it
			projectPath = name;							//Now, until the user saves it on another file, the project is linked
														//to projectPath
			save();
		}
	});
}

void MainContentComponent::open()
{
	if (projectIsSaved())	//if the current project is non-empty and dot saved the user is prompted to choose wether to
	{						//loose it
		fc.reset(new FileChooser("Choose a file to open...", File::getCurrentWorkingDirectory(),
			"*.grproj", true));

		fc->launchAsync(/*FileBrowserComponent::canSelectMultipleItems |*/ FileBrowserComponent::openMode //canSelectMultipleItems to be implemented
			| FileBrowserComponent::canSelectFiles, [&](const FileChooser& chooser)
		{
			String chosen;
			auto results = chooser.getURLResults();

			for (auto result : results)
				chosen << (result.isLocalFile() ? result.getLocalFile().getFullPathName()
					: result.toString(false));

			try
			{
				if (chosen.isNotEmpty())
					open(chosen.toStdString());	//from now on the linked project will be "chosen"
			}
			catch (exception & exc)
			{
				errorAlert(exc);	//exception must be handled inside the lambda!!! Can't find a way to do it otherwise
			}
		});
	}
}

void MainContentComponent::open(std::string filePath)
{
	if (fexists(filePath))
	{
		reset();

		ifstream projReader(filePath, ios::in);

		string result;

		if (projReader.good())
		{
			try
			{
				char buffer[256];

				projReader.getline(buffer, 256);

				string s(buffer);

				newSymbolName = getNextData(s);	//uses the function getNextData do read the file and save all the symbols in
				texstream.set_image_density(stoi(getNextData(s)));	//symbolsList. It also restores the settings and the symbol
				mySlider::slidersDigitsNum = stoi(getNextData(s)); //name

				while (projReader.getline(buffer, 256))
				{
					string d(buffer);

					LaTeXSymbol nextSymbol;

					for (int i = 1; i < LaTeXSymbol::stop; i++)
						nextSymbol.setAttributebyID(i, getNextData(d));

					symbolsList.push_back(nextSymbol);
				}

				update();
			}
			catch (exception & exc)
			{
				throw; //exceptions are not handled in this function.
			}
		}
		else
			throw GrafiaException("Unable to open this file");

		compile();

		saved = true;

		projectPath = filePath;
	}
	else
		throw GrafiaException("The selected file does not exist.\nFile: " + filePath);
}

void MainContentComponent::runSettings()
{
#if JUCE_MODAL_LOOPS_PERMITTED //should always be true in recent operative systems. Otherwise... no settings :(
	AlertWindow w("Settings",
		"The following parameters control the functionalities of this application.",
		AlertWindow::NoIcon);

	w.addTextEditor("image_density", to_string(texstream.get_image_density()), "Image density:");
	w.addComboBox("slidersDigits", { "1", "2", "3", "4", "5", "6", "7", "8", "9" }, "Sliders decimal digits number");
	w.getComboBoxComponent("slidersDigits")->setSelectedItemIndex(mySlider::slidersDigitsNum - 1);
	w.addButton("OK", 1, KeyPress(KeyPress::returnKey, 0, 0));
	w.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey, 0, 0));

	w.getTextEditor("image_density")->setInputRestrictions(5, "0123456789"); //only unsigned int for image_density!

	if (w.runModalLoop() != 0) // if they picked 'ok'
	{
		// this is the item they chose in the drop-down list..
		int num = w.getComboBoxComponent("slidersDigits")->getSelectedItemIndex();
		if (num != -1)
		{
			mySlider::slidersDigitsNum = num + 1;	//how many digits are displayed by sliders

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
		+ "}{$" + symbolsList[0].getLaTeX() + "$}}\\cr\n";	//The first symbol is dominant: the others are positioned with respect
															//to this one. Indeed it is not translated.
	for (auto it = symbolsList.begin() + 1; it != symbolsList.end(); ++it)
		texStream << "	\\hidewidth\\kern" + eatRightZeros(to_string(it->getx())) + "pt\\raise" +
			eatRightZeros(to_string(it->gety())) + "pt\\hbox{\\rotatebox[origin=c]{" +
			eatRightZeros((to_string(it->getRotAngle()))) + "}{\\scalebox{"	+ eatRightZeros(to_string(it->getSizeRatio())) +
			"}{$" + it->getLaTeX() + "$}}}\\hidewidth\\cr\n";	//the other symbols are writtend down with everything,
																//translations included
	texStream << "}}}\n\n";
}

void MainContentComponent::compile()	//most important function!
{
	if (!symbolsList.empty())
	{
		try
		{
			message.set("Writing file..."); //need to implement multithreading for this. It will work in a future release

			texstream.open_rewritemode(); //better to rewrite everything each time

			texstream << "\\documentclass{minimal}\n\n"
				"\\usepackage[paperwidth=3cm, paperheight=1.7cm, margin=-0.269cm]{geometry}\n"
				"\\usepackage{graphicx, amsmath, amssymb}\n";

			writeSymbolTeXCode(texstream);

			texstream << "\\begin{document}\n"
				"$$ a \\" + newSymbolName + " b $$\n"	//the a and b are there to offer something to confront the symbol with
				"\\end{document}";

			message.set("Compiling..."); //need to implement multithreading for this

			string warning = texstream.to("png");	//is this the best way? I'll switch to svg when juce will
													//give a better support for it

			File teximage(String(texstream.get_fullpath_ext("png")));

			message.set("Processing picture"); //need to implement multithreading for this

			Image tex_preimage = PNGImageFormat::loadFrom(teximage);	//I'm leaving juce do all the work

			if (!tex_preimage.isValid())
			{
				throw GrafiaException("Image is not valid");
			}

			tex_image.setImage(tex_preimage);

			update_displayed();

			message.set(warning);
		}
		catch (exception& exc)
		{
			throw; //exceptions are not handled in this function
		}
	}
	else
		message.set("No TeX symbols added. Nothing to compile.");	//no need to throw exceptions for this kind of things
}

void MainContentComponent::add(LaTeXSymbol newObject)
{
	symbolsList.push_back(newObject);

	saved = false;

	tex_text.setText("");

	table_ptr->update();
}

void MainContentComponent::remove()
{
	for (auto it = symbolsList.begin(); it != symbolsList.end();)
		if (it->is_selected())
		{
			if (selected_symbol != nullptr)
				if (*it == *selected_symbol)	//if the current displayed symbol is removed, the displayed
					zero_displayed();			//informations are removed

			it = symbolsList.erase(it);
		}
		else
			++it;

	table_ptr->update();

	if (compileAtEachCommand.getToggleState())
		compile();
}

void MainContentComponent::reset()
{
	zero_displayed();

	symbolsList.clear();

	table_ptr->update();

	setNewSymbolName("newSymbol");

	tex_text.setText("", dontSendNotification);

	message.set("");

	Image a; //it just creates a blank image

	tex_image.setImage(a); //to hide any symbol being currently displayed
}

bool MainContentComponent::projectIsSaved()
{
	if (!saved)
		return AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, "File not saved",
			"The file is not saved. Are you sure you wish to continue?", {}, {}, 0, nullptr);
	else
		return true;
}

bool MainContentComponent::overwriteExistingFile(std::string filename)
{
	if (filename != "" && fexists(filename))
	{
		bool ok;

		AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon, "File already exists",
			"There is already another file with the name chosen. If ok is clicked the file will be overwritten.",
			{}, {}, 0, ModalCallbackFunction::create([&](int result) { ok = result; }));

		return ok;
	}

	return true;
}

void MainContentComponent::moveSymbolUp()
{
	if (selected_symbol == nullptr)
		message.set("No symbol selected");
	else
	{
		int i = 0;

		for (auto symbol : symbolsList)
		{
			if (symbol == *selected_symbol)
				break;

			++i;	//I need the position of the symbol in the vector to force the selected row
		}

		if (i != 0)	//If the element is the first aìof the list it cannot be moved up
		{
			swap(symbolsList[i - 1], symbolsList[i]); //from algorithm.h

			selected_symbol = &symbolsList[i - 1];			

			table_ptr->forceRowSelected(i - 1);
		}

		update_displayed();

		message.set("");

		saved = false;
	}
}

void MainContentComponent::moveSymbolDown()
{
	if (selected_symbol == nullptr)
		message.set("No symbol selected");
	else
	{
		unsigned int i = 0; // just to avoid a windows warning

		for (auto symbol : symbolsList)
		{
			if (symbol == *selected_symbol)
				break;

			++i;
		}

		if (i != symbolsList.size() - 1)
		{
			swap(symbolsList[i + 1], symbolsList[i]);

			selected_symbol = &symbolsList[i + 1];

			table_ptr->forceRowSelected(i + 1);
		}

		message.set("");

		saved = false;
	}
}

void MainContentComponent::raisex(const double x)
{
	for (auto & symbol : symbolsList)
		if (symbol.is_selected())
			symbol.setx(symbol.getx() + x);

	saved = false;
}

void MainContentComponent::raisey(const double y)
{
	for (auto & symbol : symbolsList)
		if (symbol.is_selected())
			symbol.sety(symbol.gety() + y);

	saved = false;
}

void MainContentComponent::scale(double factor)
{
	for (auto & symbol : symbolsList)
		if (symbol.is_selected())
			symbol.setSizeRatio(symbol.getSizeRatio()*factor);

	saved = false;
}

void MainContentComponent::rotate(double angle)
{
	for (auto & symbol : symbolsList)
		if (symbol.is_selected())
			symbol.setRotAngle(symbol.getRotAngle() + angle);
}

void MainContentComponent::errorAlert(const std::exception & exc)
{
	message.set("Error caught");

	AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Error", String(exc.what()));
}

DrawablePath * MainContentComponent::create_triangle(Point<float> a, Point<float> b, Point<float> c)
{
	static DrawablePath triangle;

	Path p;
	p.addTriangle(a, b, c);
	triangle.setPath(p);
	triangle.setFill(findColour(0x1000202)); //Using standard IDs of Juce found online
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
								MainContentComponent::SaveAs,
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

		case MainContentComponent::SaveAs:
			result.setInfo("Save as...", "Specifies where the current project should be saved", filecommands, 0);
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
	try
	{
		switch (info.commandID)
		{
		case Save:
			if (projectPath.empty())
				saveAs();
			else
				save();
			break;

		case SaveAs:
			saveAs();
			break;

		case New:
			if (projectIsSaved())
			{
				reset();

				message.set("Ready");

				saved = true;
			}
			break;

		case Open:
			open();
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
						"University of Milan\n"						//in a future release this window will be more cool
						"https://github.com/Nicknamen/Grafia"));	//I'll start by adding the Grafia symbol
			break;

		default:
			return false;
		}
	}
	catch (exception& exc)
	{
		errorAlert(exc);
	}

	return true;
}

void MainContentComponent::setNewSymbolName(std::string newName)
{
	newSymbolName = newName;

	symbolNameEditor.setText(newName, dontSendNotification);

	saved = false;
}

MainContentComponent::~MainContentComponent()
{
	menubar.reset();

	applicationCommandManager.reset();
}

void MainContentComponent::paint(Graphics& g) //standard juce function
{
    g.fillAll (Colours::lightgrey);

    g.setFont (Font (16.0f));
    g.setColour (Colours::black);
}

void MainContentComponent::resized()
{
	menubar->setBounds(0, 0, getWidth(), 20);

	message.setBounds(0, getHeight() - 20, getWidth(), 20);

	tex_image.setBounds(10, 30, getWidth() / 2 - 15, getHeight() / 2 - 15);
	tex_text.setBounds(getWidth() / 2 + 5, 50, getWidth()/4 - 10, 25);
	compile_button.setBounds(static_cast<int>(getWidth()*0.75 + 5), 80, static_cast<int>(getWidth() / 4 - 10), 25);

	symbolNameEditor.setBounds(static_cast<int>(getWidth()*0.75 + 5), 110, static_cast<int>(getWidth() / 4 - 10), 25);

	add_button.setBounds(static_cast<int>(getWidth()*0.75 + 5), 50, static_cast<int>(getWidth() / 4 - 10), 25);
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

	g.drawImage(getImage(), bounds);	//draws the png image

	g.setColour(Colours::darkgrey);

	g.drawRect(bounds);
}

int MainContentComponent::mySlider::slidersDigitsNum = 3;

String MainContentComponent::mySlider::getTextFromValue(double value)	//this function is reimplemented to elimenate
{																		//unnecessary zeros on the right
	if (textFromValueFunction != nullptr)
		return textFromValueFunction(value);

	if (getNumDecimalPlacesToDisplay() > 0)
		return String(eatRightZeros((String(value, getNumDecimalPlacesToDisplay())).toStdString())) + getTextValueSuffix();

	return String(roundToInt(value)) + getTextValueSuffix();
}

void MainContentComponent::messageComponent::paint(Graphics & g)
{
	g.setColour(Colours::dimgrey);
	g.fillAll();

	g.setFont(Font(16.0f));
	g.setColour(Colours::white);
	g.drawText(message, juce::Rectangle<int>(0, 0, getWidth() - 5, getHeight() - 2), Justification::bottomRight, true);
}

void MainContentComponent::messageComponent::set(std::string to_be_written)
{
	message = to_be_written;

	repaint();
}

int LaTeXSymbol::_symbolCount = 0;

LaTeXSymbol::LaTeXSymbol() : _symbolID(_symbolCount++),
							 _name(),
  							 _LaTeX(),
 							 _selected(),
 							 _x(),
 							 _y(),
 							 _rotAngle(),
 							 _sizeRatio()
{}

LaTeXSymbol::LaTeXSymbol(std::string name, std::string LaTeX, double x,
	double y, double rotAngle, double sizeRatio, bool selected):
	_symbolID(_symbolCount++)
{
	_name = name;
	_LaTeX = LaTeX;
	_selected = selected;
	_x = x;
	_y = y;
	_rotAngle = rotAngle;
	_sizeRatio = sizeRatio;
}

LaTeXSymbol & LaTeXSymbol::operator=(const LaTeXSymbol & other) //this operator intentionally does not copy the _symbolID
{																//as that is associated to the history of the element
	_name = other._name;
	_LaTeX = other._LaTeX;
	_x = other._x;
	_y = other._y;
	_rotAngle = other._rotAngle;
	_sizeRatio = other._sizeRatio;
	_selected = other._selected;
	return *this;
}

LaTeXSymbol::LaTeXSymbol(const LaTeXSymbol & other):
	_symbolID(other._symbolID),
	_name(other._name),
	_LaTeX(other._LaTeX),
	_x(other._x),
	_y(other._y),
	_rotAngle(other._rotAngle),
	_sizeRatio(other._sizeRatio),
	_selected(other._selected)
{}

bool LaTeXSymbol::operator==(const LaTeXSymbol & other) const
{
	if (_symbolID == other._symbolID &&
		_name == other._name &&
		_LaTeX == other._LaTeX &&
		_x == other._x &&
		_y == other._y &&
		_rotAngle == other._rotAngle &&
		_sizeRatio == other._sizeRatio)
		return true;
	else
		return false;
}

bool LaTeXSymbol::operator!=(const LaTeXSymbol & other) const
{
	return !operator==(other);
}

std::string LaTeXSymbol::getAttributeTextbyID(int id) const
{
	if (id == symbolID_id)
		return to_string(_symbolID);
	else if (id == name_id)
		return _name;
	else if (id == LaTeX_id)
		return _LaTeX;
	else if (id == selected_id)
		return _selected ? "Y" : "N";
	else if (id == x_id)
		return eatRightZeros(to_string(_x));
	else if (id == y_id)
		return eatRightZeros(to_string(_y));
	else if (id == rotAngle_id)
		return eatRightZeros(to_string(_rotAngle));
	else if (id == sizeRatio_id)
		return eatRightZeros(to_string(_sizeRatio));
	else
		return{};
}

void LaTeXSymbol::setAttributebyID(int id, std::string text)
{
	if (id == name_id)
		_name = text;
	else if (id == LaTeX_id)
		_LaTeX = text;
	else if (id == selected_id)
		_selected = ((text == "Y") ? 1 : 0);
	else if (id == x_id)
		_x = stod(text);
	else if (id == y_id)
		_y = stod(text);
	else if (id == rotAngle_id)
		_rotAngle = stod(text);
	else if (id == sizeRatio_id)
		_sizeRatio = stod(text);
}

void LaTeXSymbol::setAttributebyID(int id, double value)
{
	if (id == selected_id)
		_selected = static_cast<bool>(value);
	else if (id == x_id)
		_x = value;
	else if (id == y_id)
		_y = value;
	else if (id == rotAngle_id)
		_rotAngle = value;
	else if (id == sizeRatio_id)
		_sizeRatio = value;
}

const std::string LaTeXSymbol::getName()
{
	return _name;
}

bool LaTeXSymbol::is_selected() const
{
	return _selected;
}

void LaTeXSymbol::set_selected(const bool b)
{
	_selected = b;
}

std::string LaTeXSymbol::getLaTeX() const
{
	return _LaTeX;
}

double LaTeXSymbol::getx() const
{
	return _x;
}

void LaTeXSymbol::setx(const double x)
{
	_x = x;
}

double LaTeXSymbol::gety() const
{
	return _y;
}

void LaTeXSymbol::sety(const double y)
{
	_y = y;
}

double LaTeXSymbol::getSizeRatio() const
{
	return _sizeRatio;
}

void LaTeXSymbol::setSizeRatio(const double size_ratio)
{
	_sizeRatio = size_ratio;
}

double LaTeXSymbol::getRotAngle() const
{
	return _rotAngle;
}

void LaTeXSymbol::setRotAngle(const double rotation_angle)
{
	_rotAngle = rotation_angle;
}
