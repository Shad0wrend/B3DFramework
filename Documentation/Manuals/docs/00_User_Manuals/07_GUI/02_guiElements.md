---
title: GUI elements
---

A GUI element is a basic primitive that GUI is constructed from. Elements can be text, buttons, input boxes, images, scroll areas, and more. Before exploring specific element types, we'll first cover functionality common to all GUI elements.

# Logical vs. physical units

The GUI system uses two types of pixel units:

**Logical pixels** are device-independent units defined at 1/96th of one logical inch. At 96 DPI (the most common desktop display setting), one logical pixel equals one physical pixel. On higher DPI displays (such as mobile devices or high-resolution monitors), logical pixels are scaled appropriately:

~~~~~~~~~~~~~{.cpp}
// Logical unit types
GUILogicalUnit width = 100;  // 100 logical pixels
GUILogicalPoint position(10, 20);  // Position in logical pixels
GUILogicalSize size(100, 50);  // Size in logical pixels
GUILogicalArea bounds(0, 0, 100, 50);  // Area in logical pixels
~~~~~~~~~~~~~

**Physical pixels** represent actual pixels on the output monitor:

~~~~~~~~~~~~~{.cpp}
// Physical unit types
GUIPhysicalUnit actualWidth = 100;  // 100 physical pixels
GUIPhysicalPoint screenPosition(10, 20);  // Position in physical pixels
GUIPhysicalSize actualSize(100, 50);  // Size in physical pixels
GUIPhysicalArea screenBounds(0, 0, 100, 50);  // Area in physical pixels
~~~~~~~~~~~~~

The relationship between logical and physical pixels is:

```
physical pixel = logical pixel * DPI scale
logical pixel = physical pixel / DPI scale
```

Most GUI element APIs use logical pixels for positioning and sizing, while rendering and hit testing use physical pixels internally. This ensures consistent visual appearance across different display densities.

# Displaying GUI elements

To display a GUI element, first create it using the static `Create()` method:

~~~~~~~~~~~~~{.cpp}
// GUILabel displays text on screen
GUILabel* label = GUILabel::Create(HString("Hello!"));
~~~~~~~~~~~~~

Creating an element alone is not enough. Register it with a **GUIWidget** by retrieving the primary @b3d::GUIPanel and adding the element to it:

~~~~~~~~~~~~~{.cpp}
GUIPanel* mainPanel = guiWidget->GetPanel();
mainPanel->AddElement(label);
~~~~~~~~~~~~~

**GUIPanel** is a special type of GUI element called a "layout" that serves as a container for other elements. At this point, the GUI element will be displayed.

![Simple GUI](../../Images/guiBasic.png)

# Destroying GUI elements

GUI elements registered with a layout (such as **GUIPanel**) are destroyed automatically when their parent layout is destroyed. If the parent layout is connected to the **GUIWidget** root panel, all layouts and elements are destroyed with the widget.

To manually destroy a GUI element, call @b3d::GUIElement::Destroy:

~~~~~~~~~~~~~{.cpp}
GUIElement::Destroy(label);
~~~~~~~~~~~~~

The element will automatically be removed from its parent layout if it has one.

# Customizing GUI elements

All GUI elements share methods for customizing position, size, color, and other properties.

## Changing position

Change element position by calling @b3d::GUIElement::SetPosition. Position is in logical pixels, relative to the top-left corner of the parent:

~~~~~~~~~~~~~{.cpp}
// Move text to coordinates (50, 50)
label->SetPosition(50, 50);
~~~~~~~~~~~~~

## Changing size

Change element size by calling @b3d::GUIElement::SetWidth, @b3d::GUIElement::SetHeight, or @b3d::GUIElement::SetSize:

~~~~~~~~~~~~~{.cpp}
// Make label 30 pixels high and 100 pixels wide
label->SetSize(GUILogicalSize(100, 30));

// Or set dimensions individually
label->SetWidth(100);
label->SetHeight(30);
~~~~~~~~~~~~~

For flexible sizing based on content and layout constraints, use @b3d::GUIElement::SetFlexibleWidth and @b3d::GUIElement::SetFlexibleHeight:

~~~~~~~~~~~~~{.cpp}
// Allow label to resize between 50 and 200 pixels wide
label->SetFlexibleWidth(50, 200);

// Allow label to resize based on content (no maximum)
label->SetFlexibleHeight(10, 0);
~~~~~~~~~~~~~

## Hiding and disabling

Temporarily hide an element with @b3d::GUIElement::SetHidden. Hidden elements remain in the layout but are not visible:

~~~~~~~~~~~~~{.cpp}
label->SetHidden(true);  // Hide
label->SetHidden(false);  // Show
~~~~~~~~~~~~~

Use @b3d::GUIElement::SetActive to remove an element from the layout entirely:

~~~~~~~~~~~~~{.cpp}
label->SetActive(false);  // Remove from layout
label->SetActive(true);   // Add back to layout
~~~~~~~~~~~~~

Disable user interaction with @b3d::GUIElement::SetDisabled. Disabled elements appear faded and cannot be interacted with:

~~~~~~~~~~~~~{.cpp}
label->SetDisabled(true);  // Disable
label->SetDisabled(false);  // Enable
~~~~~~~~~~~~~

# GUI element types

The framework provides a comprehensive library of GUI element types. We'll cover the most important ones here.

## Label

Labels display textual strings with no user interaction. Create labels with @b3d::GUILabel::Create:

~~~~~~~~~~~~~{.cpp}
GUILabel* label = GUILabel::Create(HString("Hello!"));
mainPanel->AddElement(label);
~~~~~~~~~~~~~

Change the displayed text with @b3d::GUILabel::SetContent:

~~~~~~~~~~~~~{.cpp}
label->SetContent(HString("New text!"));
~~~~~~~~~~~~~

![Label](../../Images/guiLabel.png)

## Texture

Texture elements display **SpriteImage** objects on screen. Create texture elements with @b3d::GUITexture::Create:

~~~~~~~~~~~~~{.cpp}
// Create a sprite texture
HTexture texture = GetImporter().Import<Texture>("logo.png");
HSpriteTexture spriteTexture = SpriteTexture::Create(texture);

// Create texture GUI element
GUITexture* guiTexture = GUITexture::Create(spriteTexture);

// Position and size the texture
guiTexture->SetPosition(GUILogicalPoint(250, 90));
guiTexture->SetSize(GUILogicalSize(150, 150));

mainPanel->AddElement(guiTexture);
~~~~~~~~~~~~~

![Texture](../../Images/guiTexture.png)

## Button

Buttons display text or images and report user interaction events. Create buttons with @b3d::GUIButton::Create:

~~~~~~~~~~~~~{.cpp}
GUIButton* button = GUIButton::Create(HString("Click me!"));
mainPanel->AddElement(button);
~~~~~~~~~~~~~

Subscribe to button events:

~~~~~~~~~~~~~{.cpp}
auto buttonClicked = []()
{
	B3D_LOG(Info, GUI, "Button clicked!");
};

button->OnClick.Connect(buttonClicked);
~~~~~~~~~~~~~

Available button events include:
- @b3d::GUIButton::OnClick - Triggered when button is clicked
- @b3d::GUIButton::OnHover - Triggered when mouse hovers over button
- @b3d::GUIButton::OnOut - Triggered when mouse leaves button area
- @b3d::GUIButton::OnDoubleClick - Triggered on double-click

![GUI buttons](../../Images/guiButton.png)

## Toggle

Toggle buttons remain in a toggled state after being pressed. Create toggle buttons with @b3d::GUIToggle::Create:

~~~~~~~~~~~~~{.cpp}
GUIToggle* toggle = GUIToggle::Create(HString(""));
mainPanel->AddElement(toggle);
~~~~~~~~~~~~~

For radio button groups where only one button can be active, create a toggle group:

~~~~~~~~~~~~~{.cpp}
SPtr<GUIToggleGroup> group = GUIToggle::CreateToggleGroup();

GUIToggle* radio0 = GUIToggle::Create(HString("Option 1"), group);
GUIToggle* radio1 = GUIToggle::Create(HString("Option 2"), group);
GUIToggle* radio2 = GUIToggle::Create(HString("Option 3"), group);

mainPanel->AddElement(radio0);
mainPanel->AddElement(radio1);
mainPanel->AddElement(radio2);
~~~~~~~~~~~~~

Subscribe to toggle state changes with @b3d::GUIToggle::OnToggled:

~~~~~~~~~~~~~{.cpp}
auto elementToggled = [](bool toggled)
{
	if (toggled)
		B3D_LOG(Info, GUI, "Toggled on!");
	else
		B3D_LOG(Info, GUI, "Toggled off!");
};

toggle->OnToggled.Connect(elementToggled);
~~~~~~~~~~~~~

![GUI toggle](../../Images/guiToggle.png)

## Input box

Input boxes allow keyboard text input. They can be single-line (default) or multi-line. Create input boxes with @b3d::GUIInputBox::Create:

~~~~~~~~~~~~~{.cpp}
GUIInputBox* singleLineInput = GUIInputBox::Create();
GUIInputBox* multiLineInput = GUIInputBox::Create(true);

mainPanel->AddElement(singleLineInput);
mainPanel->AddElement(multiLineInput);
~~~~~~~~~~~~~

Retrieve the current text with @b3d::GUIInputBox::GetText:

~~~~~~~~~~~~~{.cpp}
String userInput = singleLineInput->GetText();
~~~~~~~~~~~~~

Set text programmatically with @b3d::GUIInputBox::SetText:

~~~~~~~~~~~~~{.cpp}
multiLineInput->SetText("Type here!");
~~~~~~~~~~~~~

Subscribe to text changes with @b3d::GUIInputBox::OnValueChanged:

~~~~~~~~~~~~~{.cpp}
auto respondToInput = [](const String& text)
{
	B3D_LOG(Info, GUI, "New input: {0}", text);
};

multiLineInput->OnValueChanged.Connect(respondToInput);
~~~~~~~~~~~~~

Restrict input using a filter callback with @b3d::GUIInputBox::SetFilter:

~~~~~~~~~~~~~{.cpp}
auto integerFilter = [](const String& text)
{
	// Use regex to match only integers
	return std::regex_match(text, std::regex("-?(\\d+)?"));
};

singleLineInput->SetFilter(integerFilter);
~~~~~~~~~~~~~

![Input boxes](../../Images/guiInputBox.png)

## List box

List boxes display multiple selectable items. They support single or multi-selection. Create list boxes with @b3d::GUIListBox::Create:

~~~~~~~~~~~~~{.cpp}
Vector<HString> listElements =
{
	HString("Orange"),
	HString("Apple"),
	HString("Banana"),
	HString("Strawberry")
};

// Single-select list
GUIListBox* listBox = GUIListBox::Create(listElements);

// Multi-select list
GUIListBox* multiSelectListBox = GUIListBox::Create(listElements, true);

mainPanel->AddElement(listBox);
mainPanel->AddElement(multiSelectListBox);
~~~~~~~~~~~~~

Retrieve selection state with @b3d::GUIListBox::GetElementStates:

~~~~~~~~~~~~~{.cpp}
auto selection = multiSelectListBox->GetElementStates();
u32 index = 0;
for (bool isSelected : selection)
{
	if (isSelected)
	{
		String selectedValue = listElements[index].GetValue();
		B3D_LOG(Info, GUI, "Selected: {0}", selectedValue);
	}
	index++;
}
~~~~~~~~~~~~~

Subscribe to selection changes with @b3d::GUIListBox::OnSelectionToggled:

~~~~~~~~~~~~~{.cpp}
auto selectionToggled = [=](u32 index, bool enabled)
{
	String value = listElements[index].GetValue();

	if (enabled)
		B3D_LOG(Info, GUI, "Selected: {0}", value);
	else
		B3D_LOG(Info, GUI, "Deselected: {0}", value);
};

listBox->OnSelectionToggled.Connect(selectionToggled);
~~~~~~~~~~~~~

![List boxes](../../Images/guiListBox.png)

## Slider

Sliders allow numeric value selection by dragging. They can be vertical or horizontal, represented by @b3d::GUISliderVert and @b3d::GUISliderHorz:

~~~~~~~~~~~~~{.cpp}
// Vertical slider
GUISliderVert* sliderVertical = GUISliderVert::Create();

// Horizontal slider
GUISliderHorz* sliderHorizontal = GUISliderHorz::Create();

mainPanel->AddElement(sliderVertical);
mainPanel->AddElement(sliderHorizontal);
~~~~~~~~~~~~~

Retrieve slider position with @b3d::GUISlider::GetPercent (returns value in range [0, 1]):

~~~~~~~~~~~~~{.cpp}
float currentPosition = sliderHorizontal->GetPercent();
~~~~~~~~~~~~~

Subscribe to position changes with @b3d::GUISlider::OnChanged:

~~~~~~~~~~~~~{.cpp}
auto sliderChanged = [](float percent)
{
	B3D_LOG(Info, GUI, "Slider at: {0}", percent);
};

sliderHorizontal->OnChanged.Connect(sliderChanged);
~~~~~~~~~~~~~

Set custom range with @b3d::GUISlider::SetRange:

~~~~~~~~~~~~~{.cpp}
// Range from 0 to 360 (e.g., degrees)
sliderHorizontal->SetRange(0.0f, 360.0f);

// Get value in custom range
float value = sliderHorizontal->GetValue();
~~~~~~~~~~~~~

Set step increment with @b3d::GUISlider::SetStep:

~~~~~~~~~~~~~{.cpp}
// 36 increments (10 degree steps)
sliderHorizontal->SetStep(10.0f / 360.0f);
~~~~~~~~~~~~~

![Vertical and a horizontal slider](../../Images/guiSlider.png)

## Scroll area

Scroll areas contain other GUI elements and provide scrollbars when content exceeds visible area. Create scroll areas with @b3d::GUIScrollArea::Create:

~~~~~~~~~~~~~{.cpp}
GUIScrollArea* scrollArea = GUIScrollArea::Create();

// Scroll areas require explicit size
scrollArea->SetSize(GUILogicalSize(100, 150));

mainPanel->AddElement(scrollArea);
~~~~~~~~~~~~~

Control scrollbar visibility using @b3d::ScrollBarType:

~~~~~~~~~~~~~{.cpp}
// Vertical scrollbar when needed, never show horizontal
GUIScrollArea* customScrollArea = GUIScrollArea::Create(
	ScrollBarType::ShowIfDoesntFit,
	ScrollBarType::NeverShow
);
~~~~~~~~~~~~~

Add elements to the scroll area's layout:

~~~~~~~~~~~~~{.cpp}
GUILayout& layout = scrollArea->GetLayout();
for (u32 i = 0; i < 20; i++)
{
	GUIButton* button = GUIButton::Create(HString("Entry #" + ToString(i)));
	layout.AddElement(button);
}
~~~~~~~~~~~~~

![Scroll area](../../Images/guiScrollArea.png)
