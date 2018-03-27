/*
  ==============================================================================

    TableComponent.cpp
    Created: 24 Mar 2018 12:52:39pm
    Author:  nicoc_000

  ==============================================================================
*/

#include "TableComponent.h"

MainContentComponent::TableComponent::TableComponent(MainContentComponent * owner_ptr)
{
	MainComponentOwner = owner_ptr;
	
	numRows = MainComponentOwner->symbolsList.size();

	addAndMakeVisible(table);

	table.getHeader().addColumn(getAttributeNameForColumnId(object_columnId), 1, 200, 50, 400, TableHeaderComponent::defaultFlags);
	table.getHeader().addColumn(getAttributeNameForColumnId(select_columnId), 2, 60, 40, 400, TableHeaderComponent::defaultFlags);

	table.setColour(ListBox::outlineColourId, Colours::grey);
	table.setOutlineThickness(1);

	table.setMultipleSelectionEnabled(true);
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

int MainContentComponent::TableComponent::getSelection(const int rowNumber) const
{
	return MainComponentOwner->symbolsList[rowNumber].get_selected();
}

void MainContentComponent::TableComponent::setSelection(const int rowNumber, const int newSelection)
{
	MainComponentOwner->symbolsList[rowNumber].set_selected(newSelection);
}

int MainContentComponent::TableComponent::getNumRows()
{
	return numRows = MainComponentOwner->symbolsList.size();
}

void MainContentComponent::TableComponent::paintRowBackground(Graphics & g, int rowNumber, int, int, bool rowIsSelected)
{
	auto alternateColour = getLookAndFeel().findColour(ListBox::backgroundColourId)
		.interpolatedWith(getLookAndFeel().findColour(ListBox::textColourId), 0.03f);

	if (rowIsSelected)
		g.fillAll(Colours::lightblue);
	else if (rowNumber % 2)
		g.fillAll(alternateColour);
}

void MainContentComponent::TableComponent::paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
	if (rowNumber < numRows)
	{
		g.setColour(rowIsSelected ? Colours::darkblue : getLookAndFeel().findColour(ListBox::textColourId));
		g.setFont(font);

		if (LaTexSymbol* rowElement = &MainComponentOwner->symbolsList[rowNumber])
		{
			auto text = rowElement->getAttributeTextbyID(attributeIDfromColumnID(columnId));
			g.drawText(text, 2, 0, width - 4, height, Justification::centredLeft, true);
		}

		g.setColour(getLookAndFeel().findColour(ListBox::backgroundColourId));
		g.fillRect(width - 1, 0, 1, height);

	}
}

void MainContentComponent::TableComponent::update()
{
	table.updateContent();

	repaint();
}

/*void MainContentComponent::TableComponent::sortOrderChanged(int newSortColumnId, bool isForwards)
{
	if (newSortColumnId != 0)
	{
		table.updateContent();
	}
}
*/
Component * MainContentComponent::TableComponent::refreshComponentForCell(int rowNumber, int columnId, bool, Component * existingComponentToUpdate)
{
	if (columnId == 2)
	{
		auto* selectionBox = static_cast<SelectionColumnCustomComponent*> (existingComponentToUpdate);

		if (selectionBox == nullptr)
			selectionBox = new SelectionColumnCustomComponent(*this);

		selectionBox->setRowAndColumn(rowNumber, columnId);

		return selectionBox;
	}
	if (columnId == 1)
	{
		auto* textLabel = static_cast<EditableTextCustomComponent*> (existingComponentToUpdate);

		if (textLabel == nullptr)
			textLabel = new EditableTextCustomComponent(*this);

		textLabel->setRowAndColumn(rowNumber, columnId);

		return textLabel;
	}
	jassert(existingComponentToUpdate == nullptr);
	return nullptr;
}

int MainContentComponent::TableComponent::getColumnAutoSizeWidth(int columnId)
{
	if (columnId == 2)
		return 50;
	int widest = 32;
	for (auto i = getNumRows(); --i >= 0;)
	{
		if (auto* rowElement = &MainComponentOwner->symbolsList[i])
		{
		auto text = rowElement->getAttributeTextbyID(attributeIDfromColumnID(columnId));
		widest = jmax(widest, font.getStringWidth(text));
		}
	}
	return widest + 8;
}

String MainContentComponent::TableComponent::getAttributeNameForColumnId(const int columnId) const
{
	if (columnId == object_columnId)
		return String("Object name");
	else if (columnId == LaTex_columndId)
		return String("LaTex code");
	else if (columnId == select_columnId)
		return String("Selcect");
	else
		return{};
}

int MainContentComponent::TableComponent::attributeIDfromColumnID(int columnId) const
{
	if (columnId == 1)
		return LaTexSymbol::name_id;
	else if (columnId == 2)
		return LaTexSymbol::selected_id;
	else
		return -1;
}

MainContentComponent::TableComponent::EditableTextCustomComponent::EditableTextCustomComponent(TableComponent & td) : owner(td)
{
	setEditable(false, true, false);
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
	addAndMakeVisible(toggleButton);

	toggleButton.onClick = [this] { owner.setSelection(row, (int)toggleButton.getToggleState()); };
}

void MainContentComponent::TableComponent::SelectionColumnCustomComponent::resized()
{
	toggleButton.setBoundsInset(BorderSize<int>(2));
}

void MainContentComponent::TableComponent::SelectionColumnCustomComponent::setRowAndColumn(int newRow, int newColumn)
{
	row = newRow;
	columnId = newColumn;
	toggleButton.setToggleState((bool)owner.getSelection(row), dontSendNotification);
}
