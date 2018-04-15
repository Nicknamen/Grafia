/*
  ==============================================================================

    TableComponent.h
    Created: 24 Mar 2018 12:52:39pm
    Author:  nicoc_000

  ==============================================================================
*/

#pragma once

#include "MainComponent.h"

class MainContentComponent::TableComponent final : public Component,
												   public TableListBoxModel
{
public:

	TableComponent(MainContentComponent* owner_ptr);
	~TableComponent();

	void resized() override;

	String getText(const int columnNumber, const int rowNumber) const;
	void setText(const int columnNumber, const int rowNumber, const String& newText);

	int getSelection(const int rowNumber) const;
	void setSelection(const int rowNumber, const int newSelection);

	int getNumRows() override;

	void cellDoubleClicked(int rowNumber, int columnId, const MouseEvent&) override;
	void backgroundClicked(const MouseEvent&) override;

	void paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override;
	void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

	void forceRowSelected(int i);

	void update();

	TableHeaderComponent& getHeader();

//	void sortOrderChanged(int newSortColumnId, bool isForwards) override;
	Component* refreshComponentForCell(int rowNumber, int columnId, bool /*isRowSelected*/, Component* existingComponentToUpdate) override;
	int getColumnAutoSizeWidth(int columnId) override;

	String getAttributeNameForColumnId(const int columnId) const;
	static int attributeIDfromColumnID(int columnId);
	static int columnIDfromAttributeID(int attributeId);

	enum {
		object_columnId = 1,
		LaTex_columndId,
		select_columnId
	};

private:
	MainContentComponent * MainComponentOwner;

	TableListBox table{ {}, this };
	Font font{ 14.0f };
	int numRows = 0;
	int numColumns = 0;

	class TeXHeader final: public TableHeaderComponent
	{
		void columnClicked(int columnId, const ModifierKeys &mods) override;
		TableComponent & tableOwner;

	public:
		~TeXHeader() = default;
		TeXHeader(TableComponent& owner);
	} header;

	class EditableTextCustomComponent : public Label
	{
	public:
		EditableTextCustomComponent(TableComponent& td);

		void mouseDown(const MouseEvent& event) override;
		void textWasEdited() override;
		void setRowAndColumn(const int newRow, const int newColumn);

	private:
		TableComponent& owner;
		int row, columnId;
		LookAndFeel_V4 TextLookAndFeel;
	};

	class SelectionColumnCustomComponent : public Component
	{
	public:
		SelectionColumnCustomComponent(TableComponent& td);

		void resized() override;
		void setRowAndColumn(int newRow, int newColumn);

	private:
		TableComponent& owner;
		ToggleButton toggleButton;
		int row, columnId;
		LookAndFeel_V4 SelectionLookAndFeel;
	};
};