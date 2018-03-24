/*
  ==============================================================================

    TableComponent.h
    Created: 24 Mar 2018 12:52:39pm
    Author:  nicoc_000

  ==============================================================================
*/

#pragma once

#include "MainComponent.h"

class MainContentComponent::TableComponent : public Component,
											 public TableListBoxModel
{
public:

	TableComponent();

	void setTableOwner(MainContentComponent* owner_ptr);

	void resized() override;

	String getText(const int columnNumber, const int rowNumber) const;
	void setText(const int columnNumber, const int rowNumber, const String& newText);

	int getNumRows() override;
	void paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override;
	void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
	void sortOrderChanged(int newSortColumnId, bool isForwards) override;
	Component* refreshComponentForCell(int rowNumber, int columnId, bool /*isRowSelected*/, Component* existingComponentToUpdate) override;
	int getColumnAutoSizeWidth(int columnId) override;

	String getAttributeNameForColumnId(const int columnId) const;

private:

	MainContentComponent * MainComponentOwner;

	TableListBox table{ {}, this };
	Font font{ 14.0f };
	int numRows = 0;

	class EditableTextCustomComponent : public Label
	{
	public:
		EditableTextCustomComponent(TableComponent& td)
			: owner(td)
		{
			setEditable(false, true, false);
		}

		void mouseDown(const MouseEvent& event) override;

		void textWasEdited() override
		{
			owner.setText(columnId, row, getText());
		}

		void setRowAndColumn(const int newRow, const int newColumn)
		{
			row = newRow;
			columnId = newColumn;
			setText(owner.getText(columnId, row), dontSendNotification);
		}

	private:
		TableComponent& owner;
		int row, columnId;
		Colour textColour;
	};
};