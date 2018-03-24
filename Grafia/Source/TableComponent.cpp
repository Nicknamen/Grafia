/*
  ==============================================================================

    TableComponent.cpp
    Created: 24 Mar 2018 12:52:39pm
    Author:  nicoc_000

  ==============================================================================
*/

#include "TableComponent.h"

MainContentComponent::TableComponent::TableComponent()
{
	addAndMakeVisible(table);
	table.getHeader().addColumn(getAttributeNameForColumnId(1), 1, 200, 50, 400, TableHeaderComponent::defaultFlags);
	table.getHeader().addColumn(getAttributeNameForColumnId(2), 2, 40, 40, 400, TableHeaderComponent::defaultFlags);
	table.setColour(ListBox::outlineColourId, Colours::grey);
	table.setOutlineThickness(1);
}

void MainContentComponent::TableComponent::setTableOwner(MainContentComponent * owner_ptr)
{
	MainComponentOwner = owner_ptr;
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
//	symbolsList[rowNumber].setAttributebyID(columnNumber, newText.toStdString());
}

int MainContentComponent::TableComponent::getNumRows()
{
	return numRows;
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
	g.setColour(rowIsSelected ? Colours::darkblue : getLookAndFeel().findColour(ListBox::textColourId)); // [5]
	g.setFont(font);

	g.setColour(getLookAndFeel().findColour(ListBox::backgroundColourId));
	g.fillRect(width - 1, 0, 1, height);
}

void MainContentComponent::TableComponent::sortOrderChanged(int newSortColumnId, bool isForwards)
{
	if (newSortColumnId != 0)
	{
		table.updateContent();
	}
}

Component * MainContentComponent::TableComponent::refreshComponentForCell(int rowNumber, int columnId, bool, Component * existingComponentToUpdate)
{
	if (columnId == 9) // [8]
	{
		/*		auto* selectionBox = static_cast<SelectionColumnCustomComponent*> (existingComponentToUpdate);
		if (selectionBox == nullptr)
		selectionBox = new SelectionColumnCustomComponent(*this);
		selectionBox->setRowAndColumn(rowNumber, columnId);
		return selectionBox;
		*/
	}
	if (columnId == 8) // [9]
	{
		//		auto* textLabel = static_cast<EditableTextCustomComponent*> (existingComponentToUpdate);
		//		if (textLabel == nullptr)
		//			textLabel = new EditableTextCustomComponent(*this);
		//		textLabel->setRowAndColumn(rowNumber, columnId);
		//		return textLabel;
	}
	jassert(existingComponentToUpdate == nullptr);
	return nullptr;    // [10]
}

int MainContentComponent::TableComponent::getColumnAutoSizeWidth(int columnId)
{
	if (columnId == 9)
		return 50;
	int widest = 32;
	for (auto i = getNumRows(); --i >= 0;)
	{
		/*		if (auto* rowElement = dataList->getChildElement(i))
		{
		auto text = rowElement->getStringAttribute(getAttributeNameForColumnId(columnId));
		widest = jmax(widest, font.getStringWidth(text));
		}
		*/
	}
	return widest + 8;
}

String MainContentComponent::TableComponent::getAttributeNameForColumnId(const int columnId) const
{
	if (columnId == 1)
		return String("Object");
	else if (columnId == 2)
		return String("Selcected");
	else
		return{};
}

void MainContentComponent::TableComponent::EditableTextCustomComponent::mouseDown(const MouseEvent & event)
{
	owner.table.selectRowsBasedOnModifierKeys(row, event.mods, false);

	Label::mouseDown(event);
}
