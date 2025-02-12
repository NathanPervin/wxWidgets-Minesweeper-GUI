#include "MainFrame.h"
#include <wx/wx.h>
#include <map>
#include <random>
#include <wx/msgdlg.h>
#include <cmath>

#include <windows.h>

// Create Event table to handle Top bar buttons
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(homeButtonID, MainFrame::OnHomeClicked)
EVT_BUTTON(hintButtonID, MainFrame::OnButtonHintClicked)
EVT_BUTTON(flagButtonID, MainFrame::OnButtonFlagClicked)
EVT_BUTTON(restartButtonID, MainFrame::OnButtonRestartClicked)
wxEND_EVENT_TABLE()

// Create window, call Start to build UI
MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	Start();
}

// Creates Main Menu and status bar when program is started
void MainFrame::Start() {
	SetupMainMenu();
	CreateStatusBar();
}

// This creates the panels, sizers, and controls for the main menu
void MainFrame::SetupMainMenu() {

	// Create panel (will autofit to window)
	panel = new wxPanel(this);
	panel->SetFont(mainFont);

	// Create top bar text and assign a font
	headlineText = new wxStaticText(panel, wxID_ANY, "Minesweeper");
	headlineText->SetFont(headlineFont);

	// Create the main sizer and the sizer for the top bar buttons 
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* topBarSizer = new wxBoxSizer(wxHORIZONTAL);

	// Create sizers for the options (grid size select, and start button)
	wxBoxSizer* middleSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* optionsSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* vertOptionsSizer = new wxBoxSizer(wxVERTICAL);

	// Add headline text to the top bar and add the top bar to the main sizer
	topBarSizer->Add(headlineText, wxALL | wxEXPAND);
	mainSizer->Add(topBarSizer, wxSizerFlags().CenterHorizontal());

	// Add the grid size options and create the radio box 
	gridSizeOptions.Add("9x9 -> 15 mines");
	gridSizeOptions.Add("16x16 -> 50 mines");
	gridSizeOptions.Add("30x16 -> 100 mines");
	gridSizeOptions.Add("Custom");
	gridSizerSelector = new wxRadioBox(panel, wxID_ANY,
		"Select a Grid Size", wxDefaultPosition,
		wxDefaultSize, gridSizeOptions, 4, wxRA_VERTICAL);

	// assign the radio box to the OnGridSizeChange method for dynamic event handling 
	gridSizerSelector->Bind(wxEVT_RADIOBOX, &MainFrame::OnGridSizeChange, this);

	// add the radio box to the middle sizer (below the top bar)
	middleSizer->Add(gridSizerSelector);

	// Create the "Start" button, add it to the middle sizer, 
	// and bind it to OnStartClicked for dynamic event handling
	startButton = new wxButton(panel, wxID_ANY, "Start", wxDefaultPosition, wxSize(200, 100));
	middleSizer->Add(startButton, wxSizerFlags().Center().Border(wxLEFT, 20));
	startButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::OnStartClicked, this);

	// add sizer holding radio box and start button to a vertical sizer
	vertOptionsSizer->Add(middleSizer, wxSizerFlags());

	// Set up the manual input fields for grid size and number of bombs
	SetupCustomInputs();

	// Add manual input field to vertical sizer, add the vertical sizer to a horizontal sizer
	// so that it will be alligned with the headline text when assigned to the main sizer
	vertOptionsSizer->Add(lowerSizer, wxSizerFlags().Border(wxTOP, 5).Expand());
	optionsSizer->Add(vertOptionsSizer);
	mainSizer->Add(optionsSizer, wxSizerFlags().CenterHorizontal());

	// Hide the custom input field (until user presses "Custom" button)
	HideCustomInputs();

	// assign main sizer to the panel and format properly
	panel->SetSizerAndFit(mainSizer);
	mainSizer->SetSizeHints(this);

}

// When the main menu start button is pressed, this will determine what grid size to use
// and will begin the game
void MainFrame::OnStartClicked(wxCommandEvent& evt) {
	
	// Checks if user has clicked on the Custom button before hitting start
	if (gridSizerSelector->GetSelection() == 3)  
	{
		// Checks if the user's inputs are valid (also assigns grid size and # bombs if it passes)
		if (!HandleCustomSelection()) {
			return;
		}
	}
	else if (gridSizerSelector->GetSelection() == 2) { // 30x16, 100 mines 
		nRows = 30;
		nCols = 16;
		numberOfBombs = 100;
	}
	else if (gridSizerSelector->GetSelection() == 1) {// 16x16, 50 mines 
		nRows = 16;
		nCols = 16;
		numberOfBombs = 50;
	}
	else { // 9x9, 15 mines
		nRows = 9;
		nCols = 9;
		numberOfBombs = 15;
	}

	// resize vector of vectors to proper grid size specified by user's choice
	buttons.resize(nCols, std::vector<ButtonData>(nRows));

	// Hide the main menu, set up the UI elements for the grid
	HideMainMenu();
	SetupPanels();
	SetupSizers();

}

// Determines if the user entered valid custom grid size and number of bombs
bool MainFrame::HandleCustomSelection() {

	// Gets the values from the input fields 
	wxString gridWidthStr = gridWidthTextCtrl->GetValue();
	wxString gridHeightStr = gridHeightTextCtrl->GetValue();
	wxString numBombsStr = numBombsTextCtrl->GetValue();

	// Determines If user input for grid width is allowed 
	if (!ValidateInput(gridWidthStr, false)) {
		wxMessageBox("Invalid Grid Width! Please enter a number between 5 and 50.",
			"Input Error",
			wxOK | wxICON_ERROR);
		return false;
	}

	// Determines If user input for grid height is allowed 
	if (!ValidateInput(gridHeightStr, false)) {
		wxMessageBox("Invalid Grid Height! Please enter a number between 5 and 50.",
			"Input Error",
			wxOK | wxICON_ERROR);
		return false;
	}

	// Determines If user input for number of bombs is allowed 
	if (!ValidateInput(numBombsStr, true)) {
		wxMessageBox("Invalid Bomb Count! Please enter a number between 5 and 2490.",
			"Input Error",
			wxOK | wxICON_ERROR);
		return false;
	}

	// initialize intermediate variables 
	long gridWidth;
	long gridHeight;
	long numBombs;

	// Convert strings to longs (ideally a short int would be used but ToLong is the simpliest way
	// for converting a wxString to a number 
	gridWidthStr.ToLong(&gridWidth);
	gridHeightStr.ToLong(&gridHeight);
	numBombsStr.ToLong(&numBombs);

	// convert longs to ints, store in class variables
	nRows = static_cast<int>(gridWidth);
	nCols = static_cast<int>(gridHeight);
	numberOfBombs = static_cast<int>(numBombs);

	// Ensures that the user cannot enter more bombs than possible places for bombs to be in 
	if (numberOfBombs >= ((nRows * nCols) - 9)) {
		wxMessageBox("Invalid Bomb Count! Too many bombs!",
			"Input Error",
			wxOK | wxICON_ERROR);
		return false;
	}

	return true;
}

// hides UI elements for the main menu when the user enters a game 
void MainFrame::HideMainMenu() {

	// Hide all elements of the main menu
	headlineText->Hide();
	gridSizerSelector->Hide();
	gridWidthStaticText->Hide();
	gridWidthTextCtrl->Hide();
	gridHeightStaticText->Hide();
	gridHeightTextCtrl->Hide();
	numBombsStaticText->Hide();
	numBombsTextCtrl->Hide();
	startButton->Hide();

	// Update layout 
	panel->Layout();
}

// Creates input fields for grid width, height, and number of bombs
void MainFrame::SetupCustomInputs() {

	// Create sizer to hold all text & input fields
	lowerSizer = new wxBoxSizer(wxVERTICAL);

	// Create labels for input fields 
	gridWidthStaticText = new wxStaticText(panel, wxID_ANY, "Enter Grid Width");
	gridHeightStaticText = new wxStaticText(panel, wxID_ANY, "Enter Grid Height");
	numBombsStaticText = new wxStaticText(panel, wxID_ANY, "Enter Bomb Count");
	
	// Create text input fields 
	gridWidthTextCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200, 40));
	gridHeightTextCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200, 40));
	numBombsTextCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200, 40));

	// Assign fonts to the label texts
	gridWidthStaticText->SetFont(fieldInputFont);
	gridHeightStaticText->SetFont(fieldInputFont);
	numBombsStaticText->SetFont(fieldInputFont);

	// Add all text lables and input fields to the lower sizer (under radio box)
	lowerSizer->Add(gridWidthStaticText);
	lowerSizer->Add(gridWidthTextCtrl, wxSizerFlags());
	lowerSizer->Add(gridHeightStaticText, wxSizerFlags().Border(wxTOP, 10));
	lowerSizer->Add(gridHeightTextCtrl, wxSizerFlags());
	lowerSizer->Add(numBombsStaticText, wxSizerFlags().Border(wxTOP, 10));
	lowerSizer->Add(numBombsTextCtrl, wxSizerFlags());
}

// Displays the custom input fields
void MainFrame::ShowCustomInputs() {
	
	gridWidthStaticText->Show();
	gridWidthTextCtrl->Show();
	gridHeightStaticText->Show();
	gridHeightTextCtrl->Show();
	numBombsStaticText->Show();
	numBombsTextCtrl->Show();

	panel->Layout();

	// change size of window to ensure that input fields are visible 
	this->SetSize(500, 560);
}

// Hide the custom input fields
void MainFrame::HideCustomInputs() {

	gridWidthStaticText->Hide();
	gridWidthTextCtrl->Hide();
	gridHeightStaticText->Hide();
	gridHeightTextCtrl->Hide();
	numBombsStaticText->Hide();
	numBombsTextCtrl->Hide();

	panel->Layout();
}

// This is executed when the user changes the radio box selection
void MainFrame::OnGridSizeChange(wxCommandEvent& event) {

	// Hide the custom input field by default
	HideCustomInputs();

	// If user switches to the custom option, display input fields
	if (gridSizerSelector->GetSelection() == 3)  // Custom Input
	{
		ShowCustomInputs();
	}
}

// Closes the window and opens a new one, restarting program 
void MainFrame::GoToMainMenu() {

	// close current window
	this->Close(true);

	// Create a new instance of the MainFrame 
	MainFrame* newFrame = new MainFrame("Minesweeper");
	newFrame->SetClientSize(500, 500);
	newFrame->Center();
	newFrame->Show(true);

}

// Creates top bar buttons for the game screen & sets fonts
void MainFrame::SetupPanels() {

	homeButton = new wxButton(panel, homeButtonID, homeChar, wxDefaultPosition, wxSize(50, 50));
	homeButton->SetFont(headlineFont);

	hintButton = new wxButton(panel, hintButtonID, hintChar, wxDefaultPosition, wxSize(50, 50));
	hintButton->SetFont(headlineFont);

	flagButton = new wxButton(panel, flagButtonID, flagSetChar, wxDefaultPosition, wxSize(50,50));
	flagButton->SetFont(mainFont);

	restartButton = new wxButton(panel, restartButtonID, restartChar, wxDefaultPosition, wxSize(50, 50));
	restartButton->SetFont(headlineFont);

}

// Create sizers (main, top bar, and grid)
void MainFrame::SetupSizers() {

	// Create headline text for in use in the game and assign it a font
	headlineTextGame = new wxStaticText(panel, wxID_ANY, "Minesweeper", wxDefaultPosition, wxDefaultSize);
	headlineTextGame->SetFont(headlineFont);

	// Create sizers for the game window, top bar, and grid 
	wxBoxSizer* mainSizerGame = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* topBarSizerGame = new wxBoxSizer(wxHORIZONTAL);
	gridSizer = new wxGridSizer(nCols, nRows, 0, 0);

	// Add the hint button, flag button, headline text, and restart button to the top bar 
	topBarSizerGame->Add(homeButton, wxSizerFlags().Border(wxRIGHT, 20));
	topBarSizerGame->Add(hintButton, wxSizerFlags().Border(wxRIGHT, 20));
	topBarSizerGame->Add(flagButton, wxSizerFlags().Border(wxRIGHT, 20));
	topBarSizerGame->Add(headlineTextGame, wxALL | wxEXPAND);
	topBarSizerGame->Add(restartButton, wxSizerFlags().Border(wxLEFT, 20));

	// add top bar sizer to the main sizer
	mainSizerGame->Add(topBarSizerGame, wxSizerFlags().CenterHorizontal());

	// Create the scrolled window for the grid
	scrolledWindow = new wxScrolledWindow(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL | wxHSCROLL);
	scrolledWindow->SetScrollRate(5, 5);  
	scrolledWindow->Bind(wxEVT_MOUSEWHEEL, &MainFrame::OnMouseWheelScroll, this);

	// Call function to generate the button matrix, assigning each onto the gridSizer
	CreateMatrix();

	// Add grid sizer to a scrolled window
	scrolledWindow->SetSizerAndFit(gridSizer);

	// Add scrolled window to the main sizer
	mainSizerGame->Add(scrolledWindow, wxSizerFlags().Expand().Proportion(1));

	// Assigns main sizer to the panel, adjusts size to fit window size
	panel->SetSizerAndFit(mainSizerGame);
	
	// Resize and center the window
	this->SetSize(700, 700);
	this->Center();
}

// This handles the scrolledWindow scroll up and down (wxWidgets takes care of the behavior)
void MainFrame::OnMouseWheelScroll(wxMouseEvent& evt) {
	evt.Skip();
}

// Assign values to a matrix (vector of vectors) of ButtonData enum.
// Assigns coordinate, wxID, creates and assigns buttons to the grid sizer
// Also binds each button to the dynamic event handler 
void MainFrame::CreateMatrix() {

	// Loop through all cells in the grid
	for (int i = 0; i < nCols; i++) {
		for (int j = 0; j < nRows; j++) {

			// save coordinate & wxID for each button
			buttons[i][j].xPos = i;
			buttons[i][j].yPos = j;
			buttons[i][j].wxID = 1000 + (i + (j * nCols));

			// Create button, set a minimum size, add it to the grid sizer
			buttons[i][j].button = new wxButton(scrolledWindow, buttons[i][j].wxID);
			buttons[i][j].button->SetMinSize(buttonSize);
			gridSizer->Add(buttons[i][j].button, 1, wxEXPAND | wxALL);

			// Use dynamic event handling for right and left clicking on a cell
			buttons[i][j].button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::OnButtonClicked, this);
			buttons[i][j].button->Bind(wxEVT_RIGHT_DOWN, &MainFrame::OnRightClick, this);

			// Create a map of buttons and their corresponding wxIDs (for use in dynamic event handler)
			buttonMap[buttons[i][j].wxID] = &buttons[i][j];

			// Save all coordinates in a vector 
			possibleCoordinates.push_back({i, j});
		}
	}
}

// Gets called when any of the grid buttons are pressed (dynamic event)
void MainFrame::OnButtonClicked(wxCommandEvent &evt) {

	// get ID of button clicked
	int buttonID = evt.GetId();

	// Get the button coordinates (saved to class variable)
	FindButtonCoordinates(buttonID);

	// handles first button press
	if (IsFirstButtonPress) {
		OnFirstButtonPressed(xPos, yPos);
		IsFirstButtonPress = false;
	}
	else { // handles all button presses after the first
		OnButtonPressed(xPos, yPos);
	}

	evt.Skip();
}

// Gets called when the user right clicks on any of the grid buttons 
void MainFrame::OnRightClick(wxMouseEvent& evt) {

	// get ID of button clicked
	int buttonID = evt.GetId();

	// Get the button coordinates (saved to class variable)
	FindButtonCoordinates(buttonID);

	// If the button has not been cleared then place a flag 
	if (!buttons[xPos][yPos].IsCleared) {
		SetFlag(xPos, yPos);
	}

	evt.Skip();
}

// Finds the coordinates of a button based on its wxID
void MainFrame::FindButtonCoordinates(int buttonID) {

	// Use map to find the coordinates of the button that was clicked (based off its wxID)
	auto mapIterator = buttonMap.find(buttonID);
	if (mapIterator != buttonMap.end()) {
		ButtonData* buttonData = mapIterator->second; // mapIterator->second returns ButtonData pointer 

		// the coordinates are obtained from the ButtonData pointer found above
		xPos = buttonData->xPos;
		yPos = buttonData->yPos;
	}
}

// Ensures that first button pressed cannot be a bomb
// generate bombs in random positions (not in initial 3x3)
// clear empty cells around initial click
void MainFrame::OnFirstButtonPressed(int xPos, int yPos) {
	HideButton(xPos, yPos);
	RemoveUnallowedCoordinates(xPos, yPos);
	RandomizeCoordinateOrder();
	PlaceBombs();
	ClearEmpty(xPos, yPos);
}

// Determines if button click is a flag placement, revealed cell, empty cell, or bomb
void MainFrame::OnButtonPressed(int xPos, int yPos) {

	// If user has entered flag mode (handled using event table and flag button)
	if (InFlagMode) {

		// Ensure that user cannot place a flag on a revealed cell (has a number or is empty)
		if (!buttons[xPos][yPos].IsCleared) {
			SetFlag(xPos, yPos);
		}
	}
	else { // User is not in flag mode 

		// Cell clicked is a bomb and not flagged, end game, prompt a restart 
		if (IsUnflaggedBomb(xPos, yPos)) {
			buttons[xPos][yPos].button->SetLabel(bombChar);
			if (wxMessageBox(wxT("KABOOM! \t Restart?"), wxT("Game Over"), wxYES_NO | wxICON_ERROR) == wxYES) {
				ReStart();
			}
		} 
		// If cell clicked is not a bomb, and not flagged
		else if (!buttons[xPos][yPos].IsFlagged) { 

			int surroundingBombs = CountBombsSurrounding(xPos, yPos);

			// Check if there are no surrounding bombs (cell is empty)
			if (surroundingBombs == 0) {
				ClearEmpty(xPos, yPos);
				CheckForVictory();
			}
			else { // has surrounding bombs: Label how many bombs surround the cell
				LabelCell(xPos, yPos, surroundingBombs);

				if (IsGridCleared()) {
					CheckForVictory();
				}
			}
		}
	}	
}

// Makes button inoperatble, transparent, and removes the text
void MainFrame::HideButton(int xPos, int yPos) {
	buttons[xPos][yPos].button->SetBackgroundColour(panel->GetBackgroundColour());
	buttons[xPos][yPos].button->Disable();
	buttons[xPos][yPos].button->SetLabel("");
}

// Removes coordinates inside of a 3x3 area around the initial clicked cell
void MainFrame::RemoveUnallowedCoordinates(int xPos, int yPos) {

	// Loop through all coordinates from beginning to end in the possibleCoordinates vector
	for (auto iterator = possibleCoordinates.begin(); iterator != possibleCoordinates.end();) {
		
		// get current coordinate
		int x = iterator->first;
		int y = iterator->second;

		if (x >= xPos - 1 && x <= xPos + 1 && y >= yPos - 1 && y <= yPos + 1) {
			iterator = possibleCoordinates.erase(iterator); // remove coordinate (erase automatically incriments iterator)
		}
		else { // If coordinate is not within 5x5 area, manually incriment iterator
			++iterator;
		}
	}
}

// Randomize coordinates order (so that bombs are placed at random)
void MainFrame::RandomizeCoordinateOrder() {
	std::random_device rd;               
	std::mt19937 gen(rd());              
	std::shuffle(possibleCoordinates.begin(), possibleCoordinates.end(), gen);
}

// Places bombs at the first numberOfBombs coordinates in the randomized possibleCoordinates vector 
void MainFrame::PlaceBombs() {
	
	// loop through first numberOfBombs of possible coordinates vector, place bomb at each
	for (int i = 0; i < numberOfBombs; i++) {
		int x_coordinate = possibleCoordinates[i].first;
		int y_coordinate = possibleCoordinates[i].second;
		buttons[x_coordinate][y_coordinate].IsBomb = true;
	}
}

// Uses Flood Fill algorithm to clear all empty cells around the initial coordinate input
void MainFrame::ClearEmpty(int xPos, int yPos) {

	// Check if cell is within bounds of grid, is not a bomb, and has not already been cleared
	if (xPos < 0 || yPos < 0 || xPos >= nCols || yPos >= nRows || buttons[xPos][yPos].IsBomb || buttons[xPos][yPos].IsCleared) {
		return; 
	}

	// Mark the current cell as cleared and hide it
	buttons[xPos][yPos].IsCleared = true;
	HideButton(xPos, yPos);

	// Define all possible single movements from an initial coordinate
	const int dx[] = { -1, 0, 1, 0, -1, -1, 1, 1 };
	const int dy[] = { 0, -1, 0, 1, -1, 1, -1, 1 };

	// Count how many bombs are surrounding the current cell
	int surroundingBombs = CountBombsSurrounding(xPos, yPos);

	// If cell is empty (no bombs surrounding it)
	if (surroundingBombs == 0) {

		// loop through the 8 cells surrounding the current cell
		for (int i = 0; i < 8; ++i) {

			int newX = xPos + dx[i];
			int newY = yPos + dy[i];

			// If the cell is within the bounds of the grid, call ClearEmpty (recursion)
			if (newX >= 0 && newY >= 0 && newX < nCols && newY < nRows) {
				ClearEmpty(newX, newY);  
			}
		}
	}
	else { // If there are bomb(s) surrounding the current cell
		buttons[xPos][yPos].numMinesSurrounding = surroundingBombs;
		LabelCell(xPos, yPos, surroundingBombs);
	}
}

// This checks if the grid has been cleared 
bool MainFrame::IsGridCleared() {

	// Loop through all of the cells in the grid 
	for (int i = 0; i < nCols; i++) {
		for (int j = 0; j < nRows; j++) {

			// If cell is not cleared nor a bomb, grid is not cleared 
			if (!buttons[i][j].IsCleared && !buttons[i][j].IsBomb) {
				return false; 
			}
		}
	}

	return true; // if grid is cleared
}

// Counts how many bumbs are surrounding the cell at inputted coordinates
int MainFrame::CountBombsSurrounding(int xPos, int yPos) {

	// Define all possible single movements from an initial coordinate
	const int dx[] = { -1, 0, 1, 0, -1, -1, 1, 1 };
	const int dy[] = { 0, -1, 0, 1, -1, 1, -1, 1 };

	int count = 0;

	// Iterate through the 8 surrounding cells, incriment count if there is a bomb  
	for (int i = 0; i < 8; ++i) {
		int newX = xPos + dx[i];
		int newY = yPos + dy[i];
		if (newX >= 0 && newY >= 0 && newX < nCols && newY < nRows) {
			if (buttons[newX][newY].IsBomb) {
				++count;  
			}
		}
	}

	return count;
}

// Creates cell label (always a number) and sets its color
void MainFrame::LabelCell(int xPos, int yPos, int surroundingBombs) {

	buttons[xPos][yPos].IsCleared = true;

	// Make sure button is enabled, set text to the number of surrounding bombs
	buttons[xPos][yPos].button->Enable();
	buttons[xPos][yPos].button->SetBackgroundColour(panel->GetBackgroundColour());
	buttons[xPos][yPos].button->SetLabel(wxString::Format("%d", surroundingBombs));
	
	// Set text color based on # of surrounding bombs
	switch (surroundingBombs) {
		case 1: // color: blue
			buttons[xPos][yPos].button->SetForegroundColour(wxColour(0, 0, 255));
			break;
		case 2: // color: green
			buttons[xPos][yPos].button->SetForegroundColour(wxColour(0, 128, 0));
			break;
		case 3: // color: red
			buttons[xPos][yPos].button->SetForegroundColour(wxColour(255, 0, 0));
			break;
		case 4: // color: dark blue
			buttons[xPos][yPos].button->SetForegroundColour(wxColour(0, 0, 139));
			break;
		case 5: // color: dark red
			buttons[xPos][yPos].button->SetForegroundColour(wxColour(139, 0, 0));
			break;
		case 6: // color: cadet blue
			buttons[xPos][yPos].button->SetForegroundColour(wxColour(95, 158, 160));
			break;
		case 7: // color: black
			buttons[xPos][yPos].button->SetForegroundColour(wxColour(0, 0, 0));
			break;
		case 8: // color: gray
			buttons[xPos][yPos].button->SetForegroundColour(wxColour(128, 128, 128));
			break;
	}
	buttons[xPos][yPos].button->Refresh();
}

// Sets cell label to a flag, also allowes user to remove flags  
void MainFrame::SetFlag(int xPos, int yPos) {

	// If there isn't a flag in the cell, add one 
	if (!buttons[xPos][yPos].IsFlagged) { 
		buttons[xPos][yPos].button->SetLabel(flagSetChar);
		buttons[xPos][yPos].IsFlagged = true;
	}
	else { // Remove Flag
		if (InFlagMode) { // If user is in flag mode and clicks on a flag,
			
			// Set flag icon is changed to a box 
			buttons[xPos][yPos].button->SetLabel(flagIndicatorChar);
		}
		else { // If user is not in flag mode (right click) and clicks on a flag

			// Remove the flag icon 
			buttons[xPos][yPos].button->SetLabel("");
		}

		buttons[xPos][yPos].IsFlagged = false;
	}	
}

// Check if the player has won, ends game if so
void MainFrame::CheckForVictory() {

	bool IsVictory = false;

	// Determine if the grid is empty (player has won)
	if (IsGridCleared()) {
		IsVictory = true;
	}

	// If Player has won, display a message and prompt a restart
	if (IsVictory) {

		// ask user if they want to restart 
		int userResponse = wxMessageBox(wxT("VICTORY! \t Restart?"), wxT("Mines Sweeped!"), wxYES_NO | wxICON_ASTERISK);
			
		// If user wants to restart 
		if (userResponse == wxYES) {
			ReStart();
		}
	}
}

// When the home button is clicked: go to main menu
void MainFrame::OnHomeClicked(wxCommandEvent& evt) {
	GoToMainMenu();
	evt.Skip();
}

// Shows the first bomb that isn't flagged
void MainFrame::OnButtonHintClicked(wxCommandEvent& evt) {

	bool FoundBomb = false;

	// If not already in hint mode: on entering hint mode 
	if (!InHintMode) {

		InHintMode = true;

		// loop through all of the cells 
		for (int i = 0; i < nCols; i++) {
			for (int j = 0; j < nRows; j++) {

				// Find the first unflagged bomb & display it 
				if (IsUnflaggedBomb(i, j)) {
					FoundBomb = true;
					buttons[i][j].button->SetLabel(bombChar);
					break;
				}
			}
			if (FoundBomb) { break; }
		}
	}
	else { // on exiting hint mode 

		InHintMode = false;
		
		// loop through all of the cells 
		for (int i = 0; i < nCols; i++) {
			for (int j = 0; j < nRows; j++) {

				// hide all of the unflagged bombs 
				if (IsUnflaggedBomb(i, j)) {
					buttons[i][j].button->SetLabel("");
				}
			}
		}
	}

	evt.Skip();
}

// Is executed when the flag button in the top bar is pressed 
void MainFrame::OnButtonFlagClicked(wxCommandEvent& evt) {

	// if not in flag mode, enter flag mode 
	if (!InFlagMode) { 
		InFlagMode = true;

		// loop through all cells
		for (int i = 0; i < nCols; i++) {
			for (int j = 0; j < nRows; j++) {

				// check if cell is flaggable, if so they display an indication that the player can flag it 
				if (IsFlaggable(i, j)) {
					buttons[i][j].button->SetLabel(flagIndicatorChar);
				}
			}
		}
	}
	else { // if in flag mode, exit flag mode
		 InFlagMode = false;

		 // loop through all cells
		 for (int i = 0; i < nCols; i++) {
		 	 for (int j = 0; j < nRows; j++) {

				 // If cell is flaggable, remove the indication that the player can flag it 
		 	 	 if (IsFlaggable(i, j)) {
					buttons[i][j].button->SetLabel("");
				 }
			 }
		 }
	}
	evt.Skip();
}

// Is executed when the restart button in the top bar is pressed
void MainFrame::OnButtonRestartClicked(wxCommandEvent& evt) {
	ReStart();
	evt.Skip();
}

// Restart the game with the same grid size and # of bombs
void MainFrame::ReStart() {

	// Loop through all of the cells are set all to their default state
	for (int i = 0; i < nCols; i++) {
		for (int j = 0; j < nRows; j++) {
			buttons[i][j].IsBomb = false;
			buttons[i][j].IsCleared = false;
			buttons[i][j].IsFlagged = false;
			buttons[i][j].button->SetLabel("");
			buttons[i][j].button->Enable();
			buttons[i][j].button->SetBackgroundColour(wxColour(*wxWHITE));
			buttons[i][j].button->Show();
			buttons[i][j].button->SetForegroundColour(wxColor(0, 0, 0));
		}
	}

	// Reset booleans for game beginning
	IsFirstButtonPress = true;
	InFlagMode = false;
	InHintMode = false;
}

// Determines if user inputted grid width or height is valid 
bool MainFrame::ValidateInput(wxString& inputStr, bool IsBombEntry) {

	// initialize intermediate variable 
	long value;

	// Check if the string can be converted to a number 
	if (!inputStr.ToLong(&value)) {
		return false;
	}

	// Check if the input is for number of bombs
	if (!IsBombEntry) {
		// Check if the number is within the allowed range for width and height
		if (value < 5 || value > 50) {
			return false;
		}
	}
	else {
		// Check if the number is within the allowed range for number of bombs
		if (value < 5 || value > 2490) {
			return false;
		}
	}

	return true;
}

// Determines if the cell is an unflagged bomb
bool MainFrame::IsUnflaggedBomb(int xPos, int yPos) {

	if (buttons[xPos][yPos].IsBomb && !buttons[xPos][yPos].IsFlagged) {
		return true;
	}
	else {
		return false;
	}
}

// Determines if the cell is within the grid's index
bool MainFrame::IsInGridIndex(int xPos, int yPos) {
	if (xPos >= 0 && yPos >= 0 && xPos < nCols && yPos < nRows) {
		return true;
	}
	else {
		return false;
	}
}

// Determmines if the cell has not been cleared, and has not been flagged
bool MainFrame::IsFlaggable(int xPos, int yPos) {

	if (!buttons[xPos][yPos].IsCleared && !buttons[xPos][yPos].IsFlagged) {
		return true;
	}
	else {
		return false;
	}
}