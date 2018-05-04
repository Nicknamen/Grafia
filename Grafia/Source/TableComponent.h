/*
  ==============================================================================

    TableComponent.h
	Project:	Grafia
    Created:	24 Mar 2018 12:52pm
    Author:		Nicolo' Cavalleri
	Github:		Nicknamen

  ==============================================================================
*/

#pragma once

#include "MainComponent.h"
#include <map>

/** This class implements the table shown on the bottom right of the Grafia main window*/
class MainContentComponent::TableComponent final : public Component,
												   public TableListBoxModel
{
public:
	/** Some methods of the table will actively modify members of MainContentComponent, so I need a pointer to it*/
	TableComponent(MainContentComponent* owner_ptr);

	// Standard Juce component function
	void resized() override;

	// Access methods for cell content
	String getText(const int columnNumber, const int rowNumber) const;
	void setText(const int columnNumber, const int rowNumber, const String& newText);

	/** Returns the number of rows to be created in the table */
	int getNumRows() override;

	/** When a row is clicked it is displayed the symbol relative to that row. Also the symbol gets selected */
	void cellClicked(int rowNumber, int columnId, const MouseEvent&) override;
	/** When the backround of the table is clicked, the displayed item is hidden */
	void backgroundClicked(const MouseEvent&) override;

	// Standard Juce Table virtual methods
	void paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override;
	void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

	// Handy for moveUp and moveDown buttons
	void forceRowSelected(int i);

	// Updates the content of the table by reloading data from symbolsList
	void update();

	TableHeaderComponent& getHeader();

	// other standard juce functions... I copied them from the demo
	Component* refreshComponentForCell(int rowNumber, int columnId, bool /*isRowSelected*/, Component* existingComponentToUpdate) override;
	int getColumnAutoSizeWidth(int columnId) override;

	enum // to work abstractly with columns
	{
		object_columnId = 1,
		LaTeX_columndId,
		select_columnId
	};

private:
	MainContentComponent * MainComponentOwner;

	TableListBox table{ {}, this }; // don't understand this syntax
	Font font{ 14.0f };
	int numRows = 0;
	int numColumns = 0;

	// Dictonaries. Self-explained in titles
	static std::map<int, String> attributeNamefromColumnId;
	static std::map<int, int> attributeIDfromColumnID;	// these two will be subsituted by the standard library
	static std::map<int, int> columnIDfromAttributeID;	// when bimap will be part of it

	//to keep track of the previous selection value of a clicked symbol
	bool wasSelectedPriorToBeClicked = false;

	// I need to reimplement the header because I want the user to be able to select everything by clicking on the select header
	class TeXHeader final: public TableHeaderComponent
	{
		void columnClicked(int columnId, const ModifierKeys &mods) override;
		TableComponent & tableOwner;

	public:
		~TeXHeader() = default;
		TeXHeader(TableComponent& owner);
	};

	// copied this from the juce demo. Allows to modify a table cell content directly from it. Linked to the _name of the symbol
	class EditableTextCustomComponent : public Label
	{
	public:
		EditableTextCustomComponent(TableComponent& td);
		~EditableTextCustomComponent();
		
		void mouseDown(const MouseEvent& event) override;
		void textWasEdited() override;
		void setRowAndColumn(const int newRow, const int newColumn);

	private:
		TableComponent& owner;			// obviously need to keep track of the parent component.
		int row, columnId;
		LookAndFeel_V4 TextLookAndFeel;	// need it to be independent from the main one. White vs black
	};

	// the toggle button at the end column, linked to the _selected variable of the symbol
	class SelectionColumnCustomComponent : public Component
	{
	public:
		SelectionColumnCustomComponent(TableComponent& td);
		~SelectionColumnCustomComponent();

		void resized() override;
		void setRowAndColumn(int newRow, int newColumn);

	private:
		TableComponent& owner;
		ToggleButton toggleButton;
		int row, columnId;
		LookAndFeel_V4 SelectionLookAndFeel;	// need it to be independent from the main one. White vs black
	};
};