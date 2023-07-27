#include "App.h"
#include "MainFrame.h"

#pragma warning(suppress: 28251)

wxIMPLEMENT_APP(App);

bool App::OnInit()//This creates an entry point for the program and initializes the application window
{
    MainFrame* mainFrame = new MainFrame("ToDoList");
    mainFrame->SetClientSize(800, 600);
    mainFrame->Centre();
    mainFrame->Show();

    return true;
}
