#pragma once
#include <wx/wx.h>
#include <vector>
#include <map>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);

private:

	// Define methods to Setup Main menu UI elements 
	void Start();
	void SetupMainMenu();
	void SetupCustomInputs();

	// Define methods that are used for Custom selection in main menu
	bool HandleCustomSelection(); 
	void OnGridSizeChange(wxCommandEvent& event);
	void HideCustomInputs();
	void ShowCustomInputs();
	bool ValidateInput(wxString&, bool IsBombEntry);

	// Define methods that handle transition from main menu to the game
	void OnStartClicked(wxCommandEvent& evt);
	void HideMainMenu();

	// Define methods to setup the game UI elements 
	void SetupPanels();
	void SetupSizers();
	void CreateMatrix();

	// Define methods used to handle gameplay
	void FindButtonCoordinates(int buttonID);
	void OnButtonPressed(int xPos, int yPos);
	void OnFirstButtonPressed(int xPos, int yPos);
	void HideButton(int xPos, int yPos);
	void RemoveUnallowedCoordinates(int xPos, int yPos);
	void RandomizeCoordinateOrder();
	void PlaceBombs();
	int CountBombsSurrounding(int xPos, int yPos);
	void ClearEmpty(int xPos, int yPos);
	bool IsGridCleared();
	void LabelCell(int xPos, int yPos, int surroundingBombs);
	void SetFlag(int, int);
	bool CheckForVictory();
	void ReStart();
	void RevealSurroundingCells(int xPos, int yPos);
	int CountFlagsSurrounding(int xPos, int yPos);
	void ClearSurroundingCells(int xPos, int yPos);
	void ShowMinesAndFalseFlags();
	void UpdateBombsRemaining();

	// Define method that will bring user back to main menu
	void GoToMainMenu();

	// Define methods that are used for in-game event handling
	wxDECLARE_EVENT_TABLE();
	void OnButtonClicked(wxCommandEvent& evt);
	void OnMouseWheelScroll(wxMouseEvent& event);
	void OnHomeClicked(wxCommandEvent& evt);
	void OnButtonHintClicked(wxCommandEvent& evt);
	void OnButtonFlagClicked(wxCommandEvent& evt);
	void OnButtonRestartClicked(wxCommandEvent& evt);
	void OnRightClick(wxMouseEvent& evt);

	// Define methods that check for various cell conditions 
	bool IsUnflaggedBomb(int xPos, int yPos);
	bool IsInGridIndex(int xPos, int yPos);
	bool IsFlaggable(int xPos, int yPos);

	// Define Universal UI elements (used in menu and game)
	wxPanel* panel;
	wxSize buttonSize = wxSize(60, 60);
	wxFont headlineFont = wxFont(wxFontInfo(wxSize(0, 36)).Bold());
	wxFont mainFont = wxFont(wxFontInfo(wxSize(0, 24)).Bold());
	wxFont fieldInputFont = wxFont(wxFontInfo(wxSize(0, 24)));
	wxFont bombCountFont = wxFont(wxFontInfo(wxSize(0, 30)).Bold());
	wxColor backgroundColor = wxColour(230, 230, 230);

	// Define Unicode characters for UI & cells
	wxUniChar restartChar = wxUniChar(0x000021BA);
	wxUniChar bombChar = wxUniChar(0x0001F4A3);
	wxUniChar hintChar = wxUniChar(0x0001F4A1);
	wxUniChar flagSetChar = wxUniChar(0x0001F6A9);
	wxUniChar flagIndicatorChar = wxUniChar(0x0001F3F3);
	wxUniChar homeChar = wxUniChar(0x0001F3E0);
	wxUniChar falseFlagChar = wxUniChar(0x00002716); // 0x00002612

	// Define sizers for Main Menu
	wxBoxSizer* lowerSizer;
	wxRadioBox* gridSizerSelector;

	// Define Static Text, Text Controls (input fields), and buttons for Main Menu
	wxTextCtrl* gridWidthTextCtrl;
	wxTextCtrl* gridHeightTextCtrl;
	wxTextCtrl* numBombsTextCtrl;
	wxStaticText* gridWidthStaticText;
	wxStaticText* gridHeightStaticText;
	wxStaticText* numBombsStaticText;
	wxStaticText* headlineTextGame;
	wxArrayString gridSizeOptions;
	wxButton* startButton;
	wxTextCtrl* numBombsRemainingTextCtrl;
	wxString numBombsRemainingString;

	// Define variables used for game UI elements 
	wxStaticText* headlineText;
	wxGridSizer* gridSizer;
	wxSizerFlags buttonFlags;
	wxBoxSizer* mainSizer;
	wxScrolledWindow* scrolledWindow;
	int numberFlagsPlaced = 0;

	// Define game UI buttons 
	wxButton* homeButton;
	wxButton* hintButton;
	wxButton* flagButton;
	wxButton* restartButton;

	// Define wxIDs for game UI buttons
	enum IDs {
		homeButtonID = 5,
		hintButtonID = 4,
		flagButtonID = 3,
		restartButtonID = 2
	};

	// Define struct used to store the data for each cell in the grid
	struct ButtonData {
		int wxID;
		int numMinesSurrounding;
		bool IsBomb = false;
		wxButton* button;
		int xPos;
		int yPos;
		bool IsCleared = false;
		bool IsFlagged = false;
	};

	// Define vector of vectors that stores all of the grid cells
	std::vector<std::vector<ButtonData>> buttons;

	// Define a map that links a cell with its respective coordinates 
	std::map<int, ButtonData*> buttonMap;

	// Define a vector that stores coordinates that bombs can be placed in
	std::vector<std::pair<int, int>> possibleCoordinates;

	// Define variables that are used in game initialization 
	int nRows;
	int nCols;
	int numberOfBombs;

	// Define variables that are used in event handling 
	int xPos = 0;
	int yPos = 0;

	// Define booleans to drive intended behaviors
	bool IsFirstButtonPress = true;
	bool InFlagMode = false;
	bool InHintMode = false;
	bool HasFlaggedBeforeFirstClick = false;

};