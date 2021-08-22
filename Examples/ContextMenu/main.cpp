#include "AppCUI.hpp"

using namespace AppCUI;
using namespace AppCUI::Application;
using namespace AppCUI::Controls;
using namespace AppCUI::Graphics;
using namespace AppCUI::Input;

#define MENU_CMD_SAVE           1000
#define MENU_CMD_OPEN           1001
#define MENU_CMD_CLEAR          1002

class MyUserControl: public AppCUI::Controls::UserControl
{
    Menu ctxMenu;
  public:
    void Create(Control* parent);
    void Paint(AppCUI::Graphics::Renderer& renderer) override;
    void OnMousePressed(int x, int y, MouseButton button) override;
};
void MyUserControl::Create(Control* parent)
{
    UserControl::Create(parent, "x:50%,y:50%,w:2,h:1");
    // build a menu
    ctxMenu.AddCommandItem("&Save content", MENU_CMD_SAVE, Key::F2);
    ctxMenu.AddCommandItem("&Open content", MENU_CMD_OPEN, Key::F3);
    ctxMenu.AddCommandItem("&Reset content", MENU_CMD_CLEAR, Key::R|Key::Ctrl|Key::Alt);
    ctxMenu.AddSeparator();
    ctxMenu.AddCheckItem("Flag &1");
    ctxMenu.AddCheckItem("Flag &2");
    ctxMenu.AddCheckItem("Flag &3");
    ctxMenu.AddSeparator();
    ctxMenu.AddRadioItem("Select option &A");
    ctxMenu.AddRadioItem("Select option &B");
    ctxMenu.AddRadioItem("Select option &C");
    ctxMenu.AddSeparator();
    auto smHandle = ctxMenu.AddSubMenu("Color");
    ctxMenu.GetSubMenu(smHandle)->AddRadioItem("Red");
    ctxMenu.GetSubMenu(smHandle)->AddRadioItem("Green");
    ctxMenu.GetSubMenu(smHandle)->AddRadioItem("Blue");
}
void MyUserControl::OnMousePressed(int x, int y, MouseButton button)
{
    if ((button & MouseButton::Right) != MouseButton::None)
    {
        ctxMenu.Show(this, x, y);
    }
}
void MyUserControl::Paint(AppCUI::Graphics::Renderer& renderer)
{
    renderer.Clear(' ', ColorPair{ Color::White, Color::Red });
}
class ContextMenuExample : public AppCUI::Controls::Window
{
    Label l1;
    MyUserControl m;
  public:
    ContextMenuExample()
    {
        this->Create("Context menu", "a:c,w:64,h:10");
        l1.Create(this, "Right click on the red-square below to view a context menu", "x:1,y:1,w:62");
        m.Create(this);
    }
    bool OnEvent(const void* sender, Event eventType, int controlID) override
    {
        if (eventType == Event::EVENT_WINDOW_CLOSE)
        {
            Application::Close();
            return true;
        }
        return false;
    }
};
int main()
{
    if (!Application::Init())
        return 1;
    Application::AddWindow(new ContextMenuExample());
    Application::Run();
    return 0;
}
