#include "MainFrame.h"
#include <wx/wx.h>
#include <vector>
#include <string>
#include "Task.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	CreateControls();
	BindEventHandlers();
	AddSavedTasks();
}

void MainFrame::CreateControls()  //Function that creates interactive window controls and properties
{
	this->SetMinSize(wxSize(800, 650)); 	//Sets the minimum and maximum size of the window
	this->SetMaxSize(wxSize(800, 650));

	wxFont headlineFont(wxFontInfo(wxSize(0, 36)).Bold());
	wxFont mainFont(wxFontInfo(wxSize(0, 24)));

	panel = new wxPanel(this);
	panel->SetFont(mainFont);
	panel->SetBackgroundColour(wxColour(50, 50, 50));
	this->SetTransparent(220);

	headlineText = new wxStaticText(panel, wxID_ANY, "To-Do List", wxPoint(0, 22), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
	headlineText->SetFont(headlineFont);
	headlineText->SetForegroundColour(wxColour(255, 255, 255));

	inputField = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(100, 80), wxSize(495, 35), wxTE_PROCESS_ENTER);
	inputField->SetBackgroundColour(wxColour(50, 50, 50));
	inputField->SetForegroundColour(wxColour(255, 255, 255));

	addButton = new wxButton(panel, wxID_ANY, "Add", wxPoint(600, 80), wxSize(100, 35));
	addButton->SetBackgroundColour(wxColour(18, 112, 195));
	addButton->SetForegroundColour(wxColour(0, 0, 0));

	checkListBox = new wxCheckListBox(panel, wxID_ANY, wxPoint(100, 120), wxSize(600, 400));
	checkListBox->SetBackgroundColour(wxColour(50, 50, 50));
	checkListBox->SetForegroundColour(wxColour(0, 0, 0));

	clearButton = new wxButton(panel, wxID_ANY, "Clear", wxPoint(100, 525), wxSize(100, 35));
	clearButton->SetBackgroundColour(wxColour(18, 112, 195));
	clearButton->SetForegroundColour(wxColour(0, 0, 0));

}

void MainFrame::BindEventHandlers() //Functions that handles all events
{
	addButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddButtonClicked, this);
	inputField->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnInputEnter, this);
	checkListBox->Bind(wxEVT_KEY_DOWN, &MainFrame::OnListKeyDowwn, this);
	clearButton->Bind(wxEVT_BUTTON, &MainFrame::OnClearButtonClicked, this);
	this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnWindowClosed, this);
}

void MainFrame::AddSavedTasks() //Function that adds the previous tasks to the file when reopening it after closing
{
	std::vector<Task> tasks = LoadTasksFromFile("Tasks.txt");

	for (const Task& task : tasks) {
		int index = checkListBox->GetCount();
		checkListBox->Insert(task.description, index);
		checkListBox->Check(index, task.done);
	}
}

void MainFrame::OnAddButtonClicked(wxCommandEvent& evt) //Add button event handler
{
	AddTaskFromInput();
}

void MainFrame::OnInputEnter(wxCommandEvent& evt) //Clicking enter to add tasks event handler
{
	AddTaskFromInput();
}

void MainFrame::OnListKeyDowwn(wxKeyEvent& evt) //Checkbox list keyboard event handler
{
	switch (evt.GetKeyCode()) {
	case WXK_DELETE: //Deletes selected tasks
		DeleteSelectedTask();
		break;
	case WXK_UP://Moves selected tasks upward
		MoveSelectedTask(-1);
		break;
	case WXK_DOWN://Moves selected tasks downward
		MoveSelectedTask(1);
		break;
	}
}

void MainFrame::OnClearButtonClicked(wxCommandEvent& evt) //Clear button event handler
{
	if (checkListBox->IsEmpty()) {
		return;
	}

	wxMessageDialog dialog(this, "Are you sure you want to clear all tasks?", "Clear Tasks", wxYES_NO | wxCANCEL);//Displays dialog box choice selection regarding clearing tasks
	int result = dialog.ShowModal();

	if (result == wxID_YES) {
		checkListBox->Clear();
	}
}

void MainFrame::OnWindowClosed(wxCloseEvent& evt) //Handles events for when the application windows gets closed
{
	std::vector<Task> tasks;

	for (int i = 0; i < checkListBox->GetCount(); i++) {
		Task task;
		task.description = checkListBox->GetString(i);
		task.done = checkListBox->IsChecked(i);
		tasks.push_back(task);
	}

	SaveTasksToFile(tasks, "Tasks.txt");//Saves the current tasks to a txt file when the application window is closed
	evt.Skip();
}

void MainFrame::AddTaskFromInput()//Function that defines geting data from the textbox into the check list
{
	wxString description = inputField->GetValue();

	if (!description.IsEmpty()) {
		checkListBox->Insert(description, checkListBox->GetCount());
		inputField->Clear();
	}

	inputField->SetFocus();
}

void MainFrame::DeleteSelectedTask() //Function that defines deleting selected tasks
{
	int selectedIndex = checkListBox->GetSelection();

	if (selectedIndex == wxNOT_FOUND) {
		return;
	}

	checkListBox->Delete(selectedIndex);
}

void MainFrame::MoveSelectedTask(int offset)//Function that defines moving selected tasks
{
	int selectedIndex = checkListBox->GetSelection();

	if (selectedIndex == wxNOT_FOUND) {
		return;
	}

	int newIndex = selectedIndex + offset;// Generating a new index for moving a task

	if (newIndex >= 0 && newIndex < checkListBox->GetCount()) {
		SwapTasks(selectedIndex, newIndex);
		checkListBox->SetSelection(newIndex, true);
	}
}

void MainFrame::SwapTasks(int i, int j)// Function that defines how tasks are moved
{
	Task taskI{ checkListBox->GetString(i).ToStdString(), checkListBox->IsChecked(i) };
	Task taskJ{ checkListBox->GetString(j).ToStdString(), checkListBox->IsChecked(j) };

	checkListBox->SetString(i, taskJ.description);
	checkListBox->Check(i, taskJ.done);

	checkListBox->SetString(j, taskI.description);
	checkListBox->Check(j, taskI.done);
}
