/*
  ==============================================================================

    TableComponent.cpp
	Project:	Grafia
	Created:	24 Mar 2018 12:52pm
	Author:		Nicolo' Cavalleri
	Github:		Nicknamen

  ==============================================================================
*/

#include "TableComponent.h"

using namespace std;

extern std::string eatRightZeros(std::string & input);
extern std::string eatRightZeros(std::string && input);

MainContentComponent::TableComponent::TableComponent(MainContentComponent * owner_ptr) :
	MainComponentOwner(owner_ptr)
{
	table.setHeader(new TeXHeader(*this));

	numRows = MainComponentOwner->symbolsList.size();

	addAndMakeVisible(table);

	numColumns = select_columnId; // supposing select_columnId is always the last one

	table.setColour(ListBox::outlineColourId, Colours::grey);
	table.setOutlineThickness(1);

	table.setMultipleSelectionEnabled(false);	// no reasons for it to be true
}

void MainContentComponent::TableComponent::resized()
{
	table.setBounds(getLocalBounds());
}

String MainContentComponent::TableComponent::getText(const int columnNumber, const int rowNumber) const
{
	return MainComponentOwner->symbolsList[rowNumber].getAttributeTextbyID(columnNumber);
}

void MainContentComponent::TableComponent::setText(const int columnNumber, const int rowNumber, const String & newText)
{
	MainComponentOwner->symbolsList[rowNumber].setAttributebyID(columnNumber, newText.toStdString());
}

int MainContentComponent::TableComponent::getNumRows()
{
	return numRows = MainComponentOwner->symbolsList.size();
}

void MainContentComponent::TableComponent::cellClicked(int rowNumber, int /*columnId*/, const MouseEvent &)
{
	//the displayed object is also selected, but when it is deselected its selection value is restored to its previous value
	if (MainComponentOwner->selected_symbol != nullptr)
		MainComponentOwner->selected_symbol->set_selected(wasSelectedPriorToBeClicked);

	//to keep track of the previous selection value
	wasSelectedPriorToBeClicked = MainComponentOwner->symbolsList[rowNumber].is_selected();

	//displays the clicked symbol
	MainComponentOwner->symbolsList[rowNumber].set_selected(true);
	MainComponentOwner->selected_symbol = &(MainComponentOwner->symbolsList[rowNumber]);
	MainComponentOwner->update_displayed();	//sets the clicked symbol as selected

	update();
}

void MainContentComponent::TableComponent::backgroundClicked(const MouseEvent &)
{
	//the displayed object is also selected, but when it is deselected its selection value is restored to its previous value
	if (MainComponentOwner->selected_symbol != nullptr)
		MainComponentOwner->selected_symbol->set_selected(wasSelectedPriorToBeClicked);

	//otherwise the line stays hilighted
	forceRowSelected(-1);

	//hides the displayed information
	MainComponentOwner->zero_displayed();
}

void MainContentComponent::TableComponent::paintRowBackground(Graphics & g, int rowNumber, int, int, bool rowIsSelected)
{
	auto alternateColour = getLookAndFeel().findColour(ListBox::backgroundColourId)
		.interpolatedWith(getLookAndFeel().findColour(ListBox::textColourId), 0.03f);	//copied from the demo
																						//don't know what exactly is going on

	if (rowIsSelected)
		g.fillAll(Colours::lightblue);
	else if (rowNumber % 2)
		g.fillAll(alternateColour);
}

void MainContentComponent::TableComponent::paintCell
(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
	if (rowNumber < numRows) //obscure things happen without this statement
	{
		//standard juce table code
		g.setColour(rowIsSelected? Colours::darkblue : getLookAndFeel().findColour(ListBox::textColourId));
		g.setFont(font);

		if (LaTeXSymbol* rowElement = &MainComponentOwner->symbolsList[rowNumber])
		{
			string text = rowElement->getAttributeTextbyID(attributeIDfromColumnID[columnId]);
			g.drawText(text, 2, 0, width - 4, height, Justification::centredLeft, true);
		}

		g.setColour(getLookAndFeel().findColour(ListBox::backgroundColourId));
		g.fillRect(width - 1, 0, 1, height);
	}
}

void MainContentComponent::TableComponent::forceRowSelected(int i)
{
	table.selectRow(i);
}

void MainContentComponent::TableComponent::update()
{
	table.updateContent();

	repaint();
}

TableHeaderComponent& MainContentComponent::TableComponent::getHeader()
{
	return table.getHeader();
}

Component * MainContentComponent::TableComponent::refreshComponentForCell(int rowNumber, int columnId, bool, Component * existingComponentToUpdate)
{
	//this code handles the components to be shown in the cells. I copied it from the demo
	if (columnId == object_columnId)
	{
		auto* textLabel = static_cast<EditableTextCustomComponent*> (existingComponentToUpdate);

		if (textLabel == nullptr)
			textLabel = new EditableTextCustomComponent(*this);

		textLabel->setRowAndColumn(rowNumber, columnId);

		return textLabel;
	}
	if (columnId == select_columnId)
	{
		auto* selectionBox = static_cast<SelectionColumnCustomComponent*> (existingComponentToUpdate);

		if (selectionBox == nullptr)
			selectionBox = new SelectionColumnCustomComponent(*this);

		selectionBox->setRowAndColumn(rowNumber, columnId);

		return selectionBox;
	}
	
	jassert(existingComponentToUpdate == nullptr);
	return nullptr;
}

int MainContentComponent::TableComponent::getColumnAutoSizeWidth(int columnId)
{
	//Again, I copied this code from the demo. It sets the width of columns at the beginning based on the content
	if (columnId == 2)
		return 50;
	int widest = 32;
	for (auto i = getNumRows(); --i >= 0;)
	{
		if (auto* rowElement = &MainComponentOwner->symbolsList[i])
		{
		auto text = rowElement->getAttributeTextbyID(attributeIDfromColumnID[columnId]);
		widest = jmax(widest, font.getStringWidth(text));
		}
	}
	return widest + 8;
}

std::map<int, String> MainContentComponent::TableComponent::attributeNamefromColumnId =
{
	pair<int, String>(object_columnId, "Object name"),
	pair<int, String>(LaTeX_columndId, "LaTeX code"),
	pair<int, String>(select_columnId, "Select")
};

std::map<int, int> MainContentComponent::TableComponent::attributeIDfromColumnID =
{
	pair<int, int>(object_columnId, LaTeXSymbol::name_id),
	pair<int, int>(LaTeX_columndId, LaTeXSymbol::LaTeX_id),
	pair<int, int>(select_columnId, LaTeXSymbol::selected_id)
};

std::map<int, int> MainContentComponent::TableComponent::columnIDfromAttributeID =
{
	pair<int, int>(LaTeXSymbol::name_id, object_columnId),
	pair<int, int>(LaTeXSymbol::LaTeX_id, LaTeX_columndId),
	pair<int, int>(LaTeXSymbol::selected_id, select_columnId)
};

MainContentComponent::TableComponent::EditableTextCustomComponent::EditableTextCustomComponent(TableComponent & td) : owner(td)
{
	setEditable(false, true, false);

	TextLookAndFeel.setColour(Label::textColourId, Colours::white);
	TextLookAndFeel.setColour(Label::textWhenEditingColourId, Colours::darkblue);

	setLookAndFeel(&TextLookAndFeel);
}

MainContentComponent::TableComponent::EditableTextCustomComponent::~EditableTextCustomComponent()
{
	setLookAndFeel(nullptr);
}

void MainContentComponent::TableComponent::EditableTextCustomComponent::mouseDown(const MouseEvent & event)
{
	owner.table.selectRowsBasedOnModifierKeys(row, event.mods, false);

	Label::mouseDown(event);
}

void MainContentComponent::TableComponent::EditableTextCustomComponent::textWasEdited()
{
	owner.setText(columnId, row, getText());
}

void MainContentComponent::TableComponent::EditableTextCustomComponent::setRowAndColumn(const int newRow, const int newColumn)
{
	row = newRow;
	columnId = newColumn;
	setText(owner.getText(columnId, row), dontSendNotification);
}

MainContentComponent::TableComponent::SelectionColumnCustomComponent::SelectionColumnCustomComponent(TableComponent & td)
	: owner(td)
{
	SelectionLookAndFeel.setColour(ToggleButton::ColourIds::textColourId, Colours::white);	//needs to be white
	SelectionLookAndFeel.setColour(ToggleButton::ColourIds::tickColourId, Colours::white);	//otherwise invisible
	SelectionLookAndFeel.setColour(ToggleButton::ColourIds::tickDisabledColourId, Colours::white);

	setLookAndFeel(&SelectionLookAndFeel);

	addAndMakeVisible(toggleButton);

	toggleButton.addListener(td.MainComponentOwner);

	//no need to create a listener for this. A lambda function is fine.
	toggleButton.onClick = [&] { owner.MainComponentOwner->symbolsList[row].set_selected(toggleButton.getToggleState()); };
}

MainContentComponent::TableComponent::SelectionColumnCustomComponent::~SelectionColumnCustomComponent()
{
	setLookAndFeel(nullptr);
}

void MainContentComponent::TableComponent::SelectionColumnCustomComponent::resized()
{
	toggleButton.setBoundsInset(BorderSize<int>(2));
}

void MainContentComponent::TableComponent::SelectionColumnCustomComponent::setRowAndColumn(int newRow, int newColumn)
{
	row = newRow;
	columnId = newColumn;
	toggleButton.setToggleState(owner.MainComponentOwner->symbolsList[row].is_selected(), dontSendNotification);
}

MainContentComponent::TableComponent::TeXHeader::TeXHeader(TableComponent& owner) : tableOwner(owner)
{
	addColumn(tableOwner.attributeNamefromColumnId[object_columnId], 1, 120, 50, 400, TableHeaderComponent::defaultFlags);
	addColumn(tableOwner.attributeNamefromColumnId[LaTeX_columndId], 2, 120, 50, 400, TableHeaderComponent::defaultFlags);
	addColumn(tableOwner.attributeNamefromColumnId[select_columnId], 3, 60, 40, 400, TableHeaderComponent::defaultFlags);
}

void MainContentComponent::TableComponent::TeXHeader::columnClicked(int columnId, const ModifierKeys & /*mods*/)
{
	//if the header of the Select column is clicked all the symbols are selected
	if (columnId == select_columnId)
	{
		for (auto symbol : tableOwner.MainComponentOwner->symbolsList)
			if (!symbol.is_selected())					//as soon as it finds an unselected symbol...
			{
				for (auto & write_symbol : tableOwner.MainComponentOwner->symbolsList)
					write_symbol.set_selected(true);	//...it selects everything...

				tableOwner.update();
				return;									//...and exits the function
			}

		for (auto & write_symbol : tableOwner.MainComponentOwner->symbolsList)	//if all the symbols are selceted
			write_symbol.set_selected(false);									//it deselects everything

		tableOwner.update();
	}
}
