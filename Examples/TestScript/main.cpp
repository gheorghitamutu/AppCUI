#include "AppCUI.hpp"

using namespace AppCUI;
using namespace AppCUI::Application;
using namespace AppCUI::Controls;

class SimpleWin : public Window
{
  public:
    SimpleWin() : Window("Test", "d:c,w:40,h:10", WindowFlags::None)
    {
        Factory::CheckBox::Create(this, "A checkbox", "x:1,y:1,w:20");
        Factory::TextField::Create(this, "", "x:1,y:3,w:30");
        Factory::TextField::Create(this, "", "x:1,y:5,w:30");
    }
};
int main()
{
    if (!Application::InitForTests(60,20, InitializationFlags::None, true))
        return 1;
    Application::AddWindow(std::make_unique<SimpleWin>());
    Application::RunTestScript(R"SCRIPT(
            Print()
            # click on the checkbox
            Mouse.Click(14,7,Left)
            # move to the next control
            Key.Press(Tab)
            # type something in the text field
            Key.Type(Hello)
            Print()
            # copy last two characters
            Key.Hold(Shift)
            Key.Press(Left)
            Key.Press(Left)
            
    )SCRIPT");
    return 0;
}
