#include "AppCUI.hpp"

using namespace AppCUI;
using namespace AppCUI::Application;
using namespace AppCUI::Controls;

class SimpleWin : public Window
{
  public:
    SimpleWin() : Window("Test", "d:c,w:40,h:10", WindowFlags::None)
    {
    }
};
int main()
{
    if (!Application::InitForTests(60,20))
        return 1;
    Application::AddWindow(std::make_unique<SimpleWin>());
    Application::RunTestScript("Print()");
    return 0;
}
