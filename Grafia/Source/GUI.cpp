/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.3.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "GUI.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
GUI::GUI ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (Clickme = new TextButton ("Clickme"));
    Clickme->addListener (this);

    addAndMakeVisible (toggleButton = new ToggleButton ("new toggle button"));
    toggleButton->setButtonText (TRANS("Option"));
    toggleButton->addListener (this);

    addAndMakeVisible (textEditor = new TextEditor ("new text editor"));
    textEditor->setMultiLine (false);
    textEditor->setReturnKeyStartsNewLine (false);
    textEditor->setReadOnly (false);
    textEditor->setScrollbarsShown (true);
    textEditor->setCaretVisible (true);
    textEditor->setPopupMenuEnabled (true);
    textEditor->setText (String());

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("Name")));
    label->setFont (Font (15.00f, Font::plain));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (comboBox = new ComboBox ("new combo box"));
    comboBox->setEditableText (false);
    comboBox->setJustificationType (Justification::centredLeft);
    comboBox->setTextWhenNothingSelected (String());
    comboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox->addItem (TRANS("1"), 1);
    comboBox->addItem (TRANS("2"), 2);
    comboBox->addItem (TRANS("3"), 3);
    comboBox->addListener (this);

    addAndMakeVisible (hyperlinkButton = new HyperlinkButton (TRANS("www.google.com"),
                                                              URL ("www.google.com")));
    hyperlinkButton->setTooltip (TRANS("www.google.com"));
    hyperlinkButton->setButtonText (TRANS("www.google.com"));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

GUI::~GUI()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    Clickme = nullptr;
    toggleButton = nullptr;
    textEditor = nullptr;
    label = nullptr;
    comboBox = nullptr;
    hyperlinkButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void GUI::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void GUI::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    Clickme->setBounds (40, 112, 150, 24);
    toggleButton->setBounds (40, 80, 150, 24);
    textEditor->setBounds (40, 48, 150, 24);
    label->setBounds (40, 24, 150, 24);
    comboBox->setBounds (120, 80, 150, 24);
    hyperlinkButton->setBounds (16, 144, 150, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void GUI::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == Clickme)
    {
        //[UserButtonCode_Clickme] -- add your button handler code here..
        //[/UserButtonCode_Clickme]
    }
    else if (buttonThatWasClicked == toggleButton)
    {
        //[UserButtonCode_toggleButton] -- add your button handler code here..
        //[/UserButtonCode_toggleButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void GUI::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBox)
    {
        //[UserComboBoxCode_comboBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GUI" componentName="" parentClasses="public Component"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff"/>
  <TEXTBUTTON name="Clickme" id="9c10c518984e6fc0" memberName="Clickme" virtualName=""
              explicitFocusOrder="0" pos="40 112 150 24" buttonText="Clickme"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="new toggle button" id="197d879ee4f582cb" memberName="toggleButton"
                virtualName="" explicitFocusOrder="0" pos="40 80 150 24" buttonText="Option"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTEDITOR name="new text editor" id="afa16025443d04a3" memberName="textEditor"
              virtualName="" explicitFocusOrder="0" pos="40 48 150 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="new label" id="683359505402c617" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="40 24 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Name" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <COMBOBOX name="new combo box" id="121ea3ad45ebf517" memberName="comboBox"
            virtualName="" explicitFocusOrder="0" pos="120 80 150 24" editable="0"
            layout="33" items="1&#10;2&#10;3" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <HYPERLINKBUTTON name="new hyperlink" id="4958f5d731612a13" memberName="hyperlinkButton"
                   virtualName="" explicitFocusOrder="0" pos="16 144 150 24" tooltip="www.google.com"
                   buttonText="www.google.com" connectedEdges="0" needsCallback="0"
                   radioGroupId="0" url="www.google.com"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
