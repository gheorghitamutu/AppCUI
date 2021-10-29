#include "AppCUI.hpp"

using namespace AppCUI::Controls;
using namespace AppCUI::Dialogs;

#define MSGBOX_BUTTONS_OK            1
#define MSGBOX_BUTTONS_OK_CANCEL     2
#define MSGBOX_BUTTONS_YES_NO_CANCEL 3

bool MessageBoxWindowEventHandler(Reference<Control> control, AppCUI::Controls::Event eventType, int controlID)
{
    switch (eventType)
    {
    case Event::WindowClose:
        control.DownCast<Window>()->Exit(Result::Cancel);
        return true;
    case Event::ButtonClicked:
        control.DownCast<Window>()->Exit(controlID);
        return true;
    default:
        return false;
    }
    return false;
}

bool CreateMessageBoxWindow(
      const AppCUI::Utils::ConstString& title,
      const AppCUI::Utils::ConstString& content,
      WindowFlags flags,
      int buttonsType,
      int* result)
{
    auto wnd = Factory::Window::Create(title, "d:c,w:60,h:10", flags);

    switch (buttonsType)
    {
    case MSGBOX_BUTTONS_OK:
        Factory::Button::Create(wnd, "&Ok", "x:23,y:6,w:15", (int) Result::Ok)->SetFocus();
        break;
    case MSGBOX_BUTTONS_OK_CANCEL:
        Factory::Button::Create(wnd, "&Cancel", "x:31,y:6,w:15", (int) Result::Cancel);
        Factory::Button::Create(wnd, "&Ok", "x:15,y:6,w:15", (int) Result::Ok)->SetFocus();
        break;
    case MSGBOX_BUTTONS_YES_NO_CANCEL:
        Factory::Button::Create(wnd, "&No", "x:23,y:6,w:15", (int) Result::No);
        Factory::Button::Create(wnd, "&Cancel", "x:39,y:6,w:15", (int) Result::Cancel);
        Factory::Button::Create(wnd, "&Yes", "x:7,y:6,w:15", (int) Result::Yes)->SetFocus();
        break;
    }
    Factory::Label::Create(wnd, content, "x:1,y:1,w:56,h:3");
    // wnd->SetEventHandler(MessageBoxWindowEventHandler);
    wnd->Handlers()->OnEvent = MessageBoxWindowEventHandler;
    if (result != nullptr)
        (*result) = wnd->Show();
    return true;
}
void MessageBox::ShowError(const AppCUI::Utils::ConstString& title, const AppCUI::Utils::ConstString& message)
{
    int result;
    CreateMessageBoxWindow(title, message, WindowFlags::ErrorWindow, MSGBOX_BUTTONS_OK, &result);
}
void MessageBox::ShowNotification(const AppCUI::Utils::ConstString& title, const AppCUI::Utils::ConstString& message)
{
    int result;
    CreateMessageBoxWindow(title, message, WindowFlags::NotifyWindow, MSGBOX_BUTTONS_OK, &result);
}
void MessageBox::ShowWarning(const AppCUI::Utils::ConstString& title, const AppCUI::Utils::ConstString& message)
{
    int result;
    CreateMessageBoxWindow(title, message, WindowFlags::WarningWindow, MSGBOX_BUTTONS_OK, &result);
}
Result MessageBox::ShowYesNoCancel(const AppCUI::Utils::ConstString& title, const AppCUI::Utils::ConstString& message)
{
    int result;
    if (CreateMessageBoxWindow(title, message, WindowFlags::NotifyWindow, MSGBOX_BUTTONS_YES_NO_CANCEL, &result) ==
        false)
        return Result::Cancel;
    return (Result) result;
}
Result MessageBox::ShowOkCancel(const AppCUI::Utils::ConstString& title, const AppCUI::Utils::ConstString& message)
{
    int result;
    if (CreateMessageBoxWindow(title, message, WindowFlags::NotifyWindow, MSGBOX_BUTTONS_OK_CANCEL, &result) == false)
        return Result::Cancel;
    return (Result) result;
}