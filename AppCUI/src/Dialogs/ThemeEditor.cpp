#include "AppCUI.hpp"

using namespace AppCUI::Controls;
using namespace AppCUI::Graphics;

namespace AppCUI::Dialogs
{
constexpr int BUTTON_CMD_CLOSE = 1;

enum class CatID : uint32
{
    None = 0,
    Desktop,
    Menu,
    ParentMenu,
    Window,
    ToolTip,
    ProgressBar,
    Button,
    Text,

    Count // must be the last one
};
constexpr string_view catNames[static_cast<uint32>(CatID::Count)] = {
    "", "Desktop", "Menu", "Menu (parent)", "Window", "ToolTip", "Progress Bar", "Buttons", "Text"
};

enum class PropID : uint32
{
    None,

    // Desktop
    DesktopChar,
    DesktopColor,

    // menus
    MenuTextNormal,
    MenuTextHovered,
    MenuTextSelected,
    MenuHotKeyNormal,
    MenuHotKeyHovered,
    MenuHotKeySelected,
    MenuShortCutNormal,
    MenuShortCutHovered,
    MenuShortCutSelected,
    MenuSymbolNormal,
    MenuSymbolHovered,
    MenuSymbolSelected,
    MenuInactive,

    // parent menu
    ParentMenuTextNormal,
    ParentMenuTextHovered,
    ParentMenuHotKeyNormal,
    ParentMenuHotKeyHovered,
    ParentMenuShortCutNormal,
    ParentMenuShortCutHovered,
    ParentMenuSymbolNormal,
    ParentMenuSymbolHovered,
    ParentMenuInactive,

    // Windows
    WindowNormal,
    WindowInactive,
    WindowError,
    WindowInfo,
    WindowWarning,

    // ToolTip
    ToolTipText,
    ToolTipArrow,

    // Progress BAr
    ProgressBarEmpty,
    ProgressBarFull,

    // Button
    ButtonTextNormal,
    ButtonTextHovered,
    ButtonTextSelected,
    ButtonTextInactive,
    ButtonHotKeyNormal,
    ButtonHotKeyHovered,
    ButtonHotKeySelected,
    ButtonHotKeyInactive,
    ButtonShadow,

    // Text
    TextNormal,
    TextHotKey,
    TextInactive,
    TextError,
    TextWarning,
    TextFocused,
    TextHovered,
    TextHighlighted,
    TextEmphasized1,
    TextEmphasized2,

};
class ConfigProperty : public PropertiesInterface
{
    AppCUI::Application::Config obj;
    CatID catID;
    PropID propID;

  public:
    ConfigProperty(const AppCUI::Application::Config& config) : obj(config), catID(CatID::None), propID(PropID::None)
    {
    }
    void SetCategoryAndProperty(string_view name, PropID pID)
    {
        propID = pID;
        for (auto i = 0U; i < static_cast<uint32>(CatID::Count); i++)
        {
            if (catNames[i] == name)
            {
                catID = static_cast<CatID>(i);
                return;
            }
        }
        catID = CatID::None;
    }
    void DrawWindow(
          Graphics::Renderer& r,
          int left,
          int top,
          int right,
          int bottom,
          string_view title,
          Color backColor,
          bool focused = true)
    {
        r.FillRect(left, top, right, bottom, ' ', { Color::Black, backColor });
        if (focused)
            r.DrawRect(left, top, right, bottom, obj.Border.Focused, LineType::Double);
        else
            r.DrawRect(left, top, right, bottom, obj.Border.Normal, LineType::Single);

        if ((int) title.size() < ((right - left) - 4))
        {
            r.WriteSingleLineText(
                  (left + right) >> 1,
                  top,
                  title,
                  focused ? obj.Text.Focused : obj.Text.Normal,
                  TextAlignament::Center);
        }
    }
    void PaintDesktop(Graphics::Renderer& r)
    {
        r.ClearWithSpecialChar(SpecialChars::Block50, obj.Symbol.Desktop);
    }
    void PaintMenusAndCommandBar(Graphics::Renderer& r, Size sz)
    {
        r.FillHorizontalLine(0, 0, (int) sz.Width, ' ', obj.Menu.Text.Normal);
        r.WriteSingleLineText(1, 0, " File ", obj.Menu.Text.PressedOrSelected, obj.Menu.HotKey.PressedOrSelected, 1);
        r.WriteSingleLineText(7, 0, " View ", obj.Menu.Text.Normal, obj.Menu.HotKey.Normal, 8);
        r.WriteSingleLineText(14, 0, " Help ", obj.Menu.Text.Hovered, obj.Menu.HotKey.Hovered, 15);

        auto y = ((int) (sz.Height)) - 1;
        r.FillHorizontalLine(0, y, (int) sz.Width, ' ', obj.Menu.Text.Normal);
        r.WriteSingleLineText(0, y, " Alt+ ", obj.Menu.Text.Inactive);
        r.WriteSingleLineText(7, y, " F1 ", obj.Menu.ShortCut.PressedOrSelected);
        r.WriteSingleLineText(11, y, "Run ", obj.Menu.Text.PressedOrSelected);
        r.WriteSingleLineText(15, y, " F2 ", obj.Menu.ShortCut.Normal);
        r.WriteSingleLineText(19, y, "Quit ", obj.Menu.Text.Normal);
        r.WriteSingleLineText(24, y, " F3 ", obj.Menu.ShortCut.Hovered);
        r.WriteSingleLineText(28, y, "Reload ", obj.Menu.Text.Hovered);

        r.FillRect(0, 1, 20, 9, ' ', obj.Menu.Text.Normal);
        r.DrawRect(0, 1, 20, 9, obj.Menu.Text.Normal, LineType::Single);
        // item Save
        r.WriteSingleLineText(2, 2, "Save", obj.Menu.Text.Normal);
        r.WriteCharacter(2, 2, 'S', obj.Menu.HotKey.Normal);
        r.WriteSingleLineText(18, 2, "Ctrl+S", obj.Menu.ShortCut.Normal, TextAlignament::Right);
        // item Open
        r.WriteSingleLineText(2, 3, "Open", obj.Menu.Text.Inactive);
        r.WriteCharacter(2, 3, 'O', obj.Menu.HotKey.Inactive);
        r.WriteSingleLineText(18, 3, "Ctrl+O", obj.Menu.ShortCut.Inactive, TextAlignament::Right);
        // line
        r.DrawHorizontalLine(1, 4, 19, obj.Menu.Text.Normal, true);
        // options
        r.WriteSingleLineText(2, 5, "  Option 1", obj.Menu.Text.Normal);
        r.WriteSingleLineText(2, 6, "  Option 2", obj.Menu.Text.Normal);
        r.WriteSpecialCharacter(2, 5, SpecialChars::CheckMark, obj.Menu.Symbol.Normal);
        // line
        r.DrawHorizontalLine(1, 7, 19, obj.Menu.Text.Normal, true);
        // item close all
        r.WriteSingleLineText(1, 8, " Close         ", obj.Menu.Text.Hovered);
        r.WriteCharacter(2, 8, 'C', obj.Menu.HotKey.Hovered);
        r.WriteSingleLineText(19, 8, "F10 ", obj.Menu.ShortCut.Hovered, TextAlignament::Right);
    }
    void PaintParentMenusAndCommandBar(Graphics::Renderer& r, Size sz)
    {
        r.FillRect(2, 1, 22, 9, ' ', obj.ParentMenu.Text.Normal);
        r.DrawRect(2, 1, 22, 9, obj.ParentMenu.Text.Normal, LineType::Single);
        // item Save
        r.WriteSingleLineText(4, 2, "Save", obj.ParentMenu.Text.Normal);
        r.WriteCharacter(4, 2, 'S', obj.ParentMenu.HotKey.Normal);
        r.WriteSingleLineText(20, 2, "Ctrl+S", obj.ParentMenu.ShortCut.Normal, TextAlignament::Right);
        // item Open
        r.WriteSingleLineText(4, 3, "Open", obj.ParentMenu.Text.Inactive);
        r.WriteCharacter(4, 3, 'O', obj.ParentMenu.HotKey.Inactive);
        r.WriteSingleLineText(20, 3, "Ctrl+O", obj.ParentMenu.ShortCut.Inactive, TextAlignament::Right);
        // line
        r.DrawHorizontalLine(3, 4, 19, obj.ParentMenu.Text.Normal, true);
        // options
        r.WriteSingleLineText(4, 5, "  Option 1", obj.ParentMenu.Text.Normal);
        r.WriteSingleLineText(4, 6, "  Option 2", obj.ParentMenu.Text.Normal);
        r.WriteSpecialCharacter(4, 5, SpecialChars::CheckMark, obj.ParentMenu.Symbol.Normal);
        // line
        r.DrawHorizontalLine(3, 7, 19, obj.ParentMenu.Text.Normal, true);
        // item close all
        r.WriteSingleLineText(3, 8, " Copy              ", obj.ParentMenu.Text.Hovered);
        r.WriteCharacter(4, 8, 'C', obj.ParentMenu.HotKey.Hovered);
        r.WriteSpecialCharacter(20, 8, SpecialChars::TriangleRight, obj.ParentMenu.Text.Hovered);
        // draw a child menu
        r.FillRect(21, 7, 35, 12, ' ', obj.Menu.Text.Normal);
        r.DrawRect(21, 7, 35, 12, obj.Menu.Text.Normal, LineType::Single);
        r.WriteSingleLineText(22, 8, " Slot 1", obj.Menu.Text.Normal);
        r.WriteSingleLineText(22, 9, " Slot 2", obj.Menu.Text.Normal);
        r.WriteSingleLineText(22, 10, " Slot 3      ", obj.Menu.Text.Hovered);
        r.WriteSingleLineText(22, 11, " Slot 4", obj.Menu.Text.Inactive);
        r.FillHorizontalLineSize(27, 7, 3, ' ', obj.Menu.Text.Normal);
        r.WriteSpecialCharacter(28, 7, SpecialChars::TriangleUp, obj.Menu.Text.Inactive);
        r.FillHorizontalLineSize(27, 12, 3, ' ', obj.Menu.Text.Normal);
        r.WriteSpecialCharacter(28, 12, SpecialChars::TriangleDown, obj.Menu.Text.Normal);
    }
    void PaintWindow(Graphics::Renderer& r, Size sz)
    {
        if ((sz.Width < 4) || (sz.Height < 4))
            return;
        switch (propID)
        {
        case PropID::WindowError:
            DrawWindow(r, 2, 1, sz.Width - 3, sz.Height - 2, " Error ", obj.Window.Background.Error);
            break;
        case PropID::WindowInfo:
            DrawWindow(r, 2, 1, sz.Width - 3, sz.Height - 2, " Info ", obj.Window.Background.Info);
            break;
        case PropID::WindowWarning:
            DrawWindow(r, 2, 1, sz.Width - 3, sz.Height - 2, " Warning ", obj.Window.Background.Warning);
            break;
        case PropID::WindowInactive:
            DrawWindow(r, 2, 1, sz.Width - 3, sz.Height - 2, " Title ", obj.Window.Background.Inactive, false);
            break;
        default:
            DrawWindow(r, 2, 1, sz.Width - 3, sz.Height - 2, " Title ", obj.Window.Background.Normal);
            break;
        }
    }
    void PaintToolTip(Graphics::Renderer& r, Size sz)
    {
        DrawWindow(r, 2, 3, sz.Width - 3, sz.Height - 3, " Title ", obj.Window.Background.Normal);
        r.WriteSingleLineText(sz.Width / 2, 1, " Tool tip text ", obj.ToolTip.Text, TextAlignament::Center);
        r.WriteSpecialCharacter(sz.Width / 2, 2, SpecialChars::ArrowDown, obj.ToolTip.Arrow);
    }
    void PaintProgressBar(Graphics::Renderer& r, Size sz)
    {
        DrawWindow(r, 2, sz.Height / 2 - 2, sz.Width - 3, sz.Height / 2 + 2, " Status ", obj.Window.Background.Normal);
        r.FillHorizontalLine(4, sz.Height / 2, sz.Width - 5, ' ', obj.ProgressStatus.Empty);
        r.FillHorizontalLine(4, sz.Height / 2, sz.Width / 2, ' ', obj.ProgressStatus.Full);
    }
    void PaintOneButton(Graphics::Renderer& r, int x, int y, string_view text, ControlState state, bool hasShadow)
    {
        auto txCol = obj.Button.Text.GetColor(state);
        auto hkCol = obj.Button.HotKey.GetColor(state);
        r.WriteSingleLineText(x, y, text, txCol, hkCol, 2, TextAlignament::Left);
        if (hasShadow)
        {
            r.FillHorizontalLineWithSpecialChar(
                  x + 1, y + 1, x + (int) text.length(), SpecialChars::BlockUpperHalf, obj.Button.ShadowColor);
            r.WriteSpecialCharacter(x + (int) text.length(), y, SpecialChars::BlockLowerHalf, obj.Button.ShadowColor);
        }
    }
    void PaintButtons(Graphics::Renderer& r, Size sz)
    {
        int x, y;
        
        x = (int)(sz.Width/2) - 14;
        y = (int) (sz.Height / 2) - 3;
        DrawWindow(r, x-2, y-1, x+27, y + 4, " Buttons ", obj.Window.Background.Normal);
        PaintOneButton(r, x, y, "  Regular  ",  ControlState::Normal, true);
        PaintOneButton(r, x, y+2, "  Hovered  ",  ControlState::Hovered, true);
        PaintOneButton(r, x + 14, y, "  Inactiv  ",  ControlState::Inactive, true);
        PaintOneButton(r, x + 14, y+2, "  Pressed  ",  ControlState::PressedOrSelected, false);
    }
    void Paint(Graphics::Renderer& r, Size sz)
    {
        switch (catID)
        {
        case CatID::None:
            r.Clear();
            break;
        case CatID::Desktop:
            PaintDesktop(r);
            break;
        case CatID::Menu:
            PaintDesktop(r);
            PaintMenusAndCommandBar(r, sz);
            break;
        case CatID::ParentMenu:
            PaintDesktop(r);
            PaintParentMenusAndCommandBar(r, sz);
            break;
        case CatID::Window:
            PaintDesktop(r);
            PaintWindow(r, sz);
            break;
        case CatID::ToolTip:
            PaintDesktop(r);
            PaintToolTip(r, sz);
            break;
        case CatID::ProgressBar:
            PaintDesktop(r);
            PaintProgressBar(r, sz);
            break;
        case CatID::Button:
            PaintDesktop(r);
            PaintButtons(r, sz);
            break;
        }
    }
    bool GetPropertyValue(uint32 propertyID, PropertyValue& value) override
    {
        switch (static_cast<PropID>(propertyID))
        {
        case PropID::DesktopChar:
            value = (char16) 186;
            return true;
        case PropID::DesktopColor:
            value = obj.Symbol.Desktop;
            return true;

        // Menus
        case PropID::MenuTextNormal:
            value = obj.Menu.Text.Normal;
            return true;
        case PropID::MenuTextHovered:
            value = obj.Menu.Text.Hovered;
            return true;
        case PropID::MenuTextSelected:
            value = obj.Menu.Text.PressedOrSelected;
            return true;
        case PropID::MenuHotKeyNormal:
            value = obj.Menu.HotKey.Normal;
            return true;
        case PropID::MenuHotKeyHovered:
            value = obj.Menu.HotKey.Hovered;
            return true;
        case PropID::MenuHotKeySelected:
            value = obj.Menu.HotKey.PressedOrSelected;
            return true;
        case PropID::MenuShortCutNormal:
            value = obj.Menu.ShortCut.Normal;
            return true;
        case PropID::MenuShortCutHovered:
            value = obj.Menu.ShortCut.Hovered;
            return true;
        case PropID::MenuShortCutSelected:
            value = obj.Menu.ShortCut.PressedOrSelected;
            return true;
        case PropID::MenuInactive:
            value = obj.Menu.Text.Inactive.Foreground;
            return true;
        case PropID::MenuSymbolNormal:
            value = obj.Menu.Symbol.Normal.Foreground;
            return true;
        case PropID::MenuSymbolHovered:
            value = obj.Menu.Symbol.Hovered.Foreground;
            return true;
        case PropID::MenuSymbolSelected:
            value = obj.Menu.Symbol.PressedOrSelected.Foreground;
            return true;

        // Parent Menus
        case PropID::ParentMenuTextNormal:
            value = obj.ParentMenu.Text.Normal;
            return true;
        case PropID::ParentMenuTextHovered:
            value = obj.ParentMenu.Text.Hovered;
            return true;
        case PropID::ParentMenuHotKeyNormal:
            value = obj.ParentMenu.HotKey.Normal;
            return true;
        case PropID::ParentMenuHotKeyHovered:
            value = obj.ParentMenu.HotKey.Hovered;
            return true;
        case PropID::ParentMenuShortCutNormal:
            value = obj.ParentMenu.ShortCut.Normal;
            return true;
        case PropID::ParentMenuShortCutHovered:
            value = obj.ParentMenu.ShortCut.Hovered;
            return true;
        case PropID::ParentMenuInactive:
            value = obj.ParentMenu.Text.Inactive.Foreground;
            return true;
        case PropID::ParentMenuSymbolNormal:
            value = obj.ParentMenu.Symbol.Normal.Foreground;
            return true;
        case PropID::ParentMenuSymbolHovered:
            value = obj.ParentMenu.Symbol.Hovered.Foreground;
            return true;

        // Window
        case PropID::WindowNormal:
            value = obj.Window.Background.Normal;
            return true;
        case PropID::WindowInactive:
            value = obj.Window.Background.Inactive;
            return true;
        case PropID::WindowError:
            value = obj.Window.Background.Error;
            return true;
        case PropID::WindowInfo:
            value = obj.Window.Background.Info;
            return true;
        case PropID::WindowWarning:
            value = obj.Window.Background.Warning;
            return true;

        // Tooltip
        case PropID::ToolTipText:
            value = obj.ToolTip.Text;
            return true;
        case PropID::ToolTipArrow:
            value = obj.ToolTip.Arrow;
            return true;

        // ProgressBar
        case PropID::ProgressBarEmpty:
            value = obj.ProgressStatus.Empty.Background;
            return true;
        case PropID::ProgressBarFull:
            value = obj.ProgressStatus.Full.Background;
            return true;

        // Buttons
        case PropID::ButtonTextNormal:
            value = obj.Button.Text.Normal;
            return true;
        case PropID::ButtonTextHovered:
            value = obj.Button.Text.Hovered;
            return true;
        case PropID::ButtonTextInactive:
            value = obj.Button.Text.Inactive;
            return true;
        case PropID::ButtonTextSelected:
            value = obj.Button.Text.PressedOrSelected;
            return true;
        case PropID::ButtonHotKeyNormal:
            value = obj.Button.HotKey.Normal;
            return true;
        case PropID::ButtonHotKeyHovered:
            value = obj.Button.HotKey.Hovered;
            return true;
        case PropID::ButtonHotKeyInactive:
            value = obj.Button.HotKey.Inactive;
            return true;
        case PropID::ButtonHotKeySelected:
            value = obj.Button.HotKey.PressedOrSelected;
            return true;
        case PropID::ButtonShadow:
            value = obj.Button.ShadowColor.Foreground;
            return true;

        // Text
        case PropID::TextNormal:
            value = obj.Text.Normal.Foreground;
            return true;
        case PropID::TextHotKey:
            value = obj.Text.HotKey.Foreground;
            return true;
        case PropID::TextInactive:
            value = obj.Text.Inactive.Foreground;
            return true;
        case PropID::TextError:
            value = obj.Text.Error.Foreground;
            return true;
        case PropID::TextWarning:
            value = obj.Text.Warning.Foreground;
            return true;
        case PropID::TextFocused:
            value = obj.Text.Focused.Foreground;
            return true;
        case PropID::TextHovered:
            value = obj.Text.Hovered.Foreground;
            return true;
        case PropID::TextHighlighted:
            value = obj.Text.Highlighted.Foreground;
            return true;
        case PropID::TextEmphasized1:
            value = obj.Text.Emphasized1.Foreground;
            return true;
        case PropID::TextEmphasized2:
            value = obj.Text.Emphasized2.Foreground;
            return true;
        }


        return false;
    }
    bool SetPropertyValue(uint32 propertyID, const PropertyValue& value, String& error) override
    {
        switch (static_cast<PropID>(propertyID))
        {
        case PropID::DesktopColor:
            obj.Symbol.Desktop = std::get<ColorPair>(value);
            return true;

        case PropID::MenuTextNormal:
            obj.Menu.Text.Normal              = std::get<ColorPair>(value);
            obj.Menu.Symbol.Normal.Background = obj.Menu.Text.Normal.Background;
            obj.Menu.Text.Inactive.Background = obj.Menu.Text.Normal.Background;
            return true;
        case PropID::MenuTextHovered:
            obj.Menu.Text.Hovered              = std::get<ColorPair>(value);
            obj.Menu.Symbol.Hovered.Background = obj.Menu.Text.Hovered.Background;
            obj.Menu.Symbol.Hovered.Background = obj.Menu.Text.Hovered.Background;
            return true;
        case PropID::MenuTextSelected:
            obj.Menu.Text.PressedOrSelected              = std::get<ColorPair>(value);
            obj.Menu.Symbol.PressedOrSelected.Background = obj.Menu.Text.PressedOrSelected.Background;
            obj.Menu.Symbol.PressedOrSelected.Background = obj.Menu.Text.PressedOrSelected.Background;
            return true;
        case PropID::MenuHotKeyNormal:
            obj.Menu.HotKey.Normal              = std::get<ColorPair>(value);
            obj.Menu.HotKey.Inactive.Background = obj.Menu.HotKey.Normal.Background;
            return true;
        case PropID::MenuHotKeyHovered:
            obj.Menu.HotKey.Hovered = std::get<ColorPair>(value);
            return true;
        case PropID::MenuHotKeySelected:
            obj.Menu.HotKey.PressedOrSelected = std::get<ColorPair>(value);
            return true;
        case PropID::MenuShortCutNormal:
            obj.Menu.ShortCut.Normal              = std::get<ColorPair>(value);
            obj.Menu.ShortCut.Inactive.Background = obj.Menu.ShortCut.Normal.Background;
            return true;
        case PropID::MenuShortCutHovered:
            obj.Menu.ShortCut.Hovered = std::get<ColorPair>(value);
            return true;
        case PropID::MenuShortCutSelected:
            obj.Menu.ShortCut.PressedOrSelected = std::get<ColorPair>(value);
            return true;
        case PropID::MenuSymbolNormal:
            obj.Menu.Symbol.Normal.Foreground = std::get<Color>(value);
            return true;
        case PropID::MenuSymbolHovered:
            obj.Menu.Symbol.Hovered.Foreground = std::get<Color>(value);
            return true;
        case PropID::MenuSymbolSelected:
            obj.Menu.Symbol.PressedOrSelected.Foreground = std::get<Color>(value);
            return true;
        case PropID::MenuInactive:
            obj.Menu.Text.Inactive.Foreground     = std::get<Color>(value);
            obj.Menu.HotKey.Inactive.Foreground   = std::get<Color>(value);
            obj.Menu.ShortCut.Inactive.Foreground = std::get<Color>(value);
            obj.Menu.Symbol.Inactive.Foreground   = std::get<Color>(value);
            return true;

        case PropID::ParentMenuTextNormal:
            obj.ParentMenu.Text.Normal              = std::get<ColorPair>(value);
            obj.ParentMenu.Symbol.Normal.Background = obj.ParentMenu.Text.Normal.Background;
            obj.ParentMenu.Text.Inactive.Background = obj.ParentMenu.Text.Normal.Background;
            return true;
        case PropID::ParentMenuTextHovered:
            obj.ParentMenu.Text.Hovered              = std::get<ColorPair>(value);
            obj.ParentMenu.Symbol.Hovered.Background = obj.ParentMenu.Text.Hovered.Background;
            obj.ParentMenu.Symbol.Hovered.Background = obj.ParentMenu.Text.Hovered.Background;
            return true;
        case PropID::ParentMenuHotKeyNormal:
            obj.ParentMenu.HotKey.Normal              = std::get<ColorPair>(value);
            obj.ParentMenu.HotKey.Inactive.Background = obj.ParentMenu.HotKey.Normal.Background;
            return true;
        case PropID::ParentMenuHotKeyHovered:
            obj.ParentMenu.HotKey.Hovered = std::get<ColorPair>(value);
            return true;
        case PropID::ParentMenuShortCutNormal:
            obj.ParentMenu.ShortCut.Normal              = std::get<ColorPair>(value);
            obj.ParentMenu.ShortCut.Inactive.Background = obj.ParentMenu.ShortCut.Normal.Background;
            return true;
        case PropID::ParentMenuShortCutHovered:
            obj.ParentMenu.ShortCut.Hovered = std::get<ColorPair>(value);
            return true;
        case PropID::ParentMenuSymbolNormal:
            obj.ParentMenu.Symbol.Normal.Foreground = std::get<Color>(value);
            return true;
        case PropID::ParentMenuSymbolHovered:
            obj.ParentMenu.Symbol.Hovered.Foreground = std::get<Color>(value);
            return true;
        case PropID::ParentMenuInactive:
            obj.ParentMenu.Text.Inactive.Foreground     = std::get<Color>(value);
            obj.ParentMenu.HotKey.Inactive.Foreground   = std::get<Color>(value);
            obj.ParentMenu.ShortCut.Inactive.Foreground = std::get<Color>(value);
            obj.ParentMenu.Symbol.Inactive.Foreground   = std::get<Color>(value);
            return true;

        case PropID::WindowNormal:
            obj.Window.Background.Normal = std::get<Color>(value);
            return true;
        case PropID::WindowInactive:
            obj.Window.Background.Inactive = std::get<Color>(value);
            return true;
        case PropID::WindowError:
            obj.Window.Background.Error = std::get<Color>(value);
            return true;
        case PropID::WindowWarning:
            obj.Window.Background.Warning = std::get<Color>(value);
            return true;
        case PropID::WindowInfo:
            obj.Window.Background.Info = std::get<Color>(value);
            return true;

        case PropID::ToolTipText:
            obj.ToolTip.Text = std::get<ColorPair>(value);
            return true;
        case PropID::ToolTipArrow:
            obj.ToolTip.Arrow = std::get<ColorPair>(value);
            return true;

        case PropID::ProgressBarEmpty:
            obj.ProgressStatus.Empty.Background = std::get<Color>(value);
            return true;
        case PropID::ProgressBarFull:
            obj.ProgressStatus.Full.Background = std::get<Color>(value);
            return true;

        case PropID::ButtonTextNormal:
            obj.Button.Text.Normal = std::get<ColorPair>(value);
            return true;
        case PropID::ButtonTextHovered:
            obj.Button.Text.Hovered = std::get<ColorPair>(value);
            return true;
        case PropID::ButtonTextInactive:
            obj.Button.Text.Inactive = std::get<ColorPair>(value);
            return true;
        case PropID::ButtonTextSelected:
            obj.Button.Text.PressedOrSelected = std::get<ColorPair>(value);
            return true;
        case PropID::ButtonHotKeyNormal:
            obj.Button.HotKey.Normal = std::get<ColorPair>(value);
            return true;
        case PropID::ButtonHotKeyHovered:
            obj.Button.HotKey.Hovered = std::get<ColorPair>(value);
            return true;
        case PropID::ButtonHotKeyInactive:
            obj.Button.HotKey.Inactive = std::get<ColorPair>(value);
            return true;
        case PropID::ButtonHotKeySelected:
            obj.Button.HotKey.PressedOrSelected = std::get<ColorPair>(value);
            return true;
        case PropID::ButtonShadow:
            obj.Button.ShadowColor.Foreground = std::get<Color>(value);
            return true;

        // Text
        case PropID::TextNormal:
            obj.Text.Normal.Foreground = std::get<Color>(value);
            return true;
        case PropID::TextHotKey:
            obj.Text.HotKey.Foreground = std::get<Color>(value);
            return true;
        case PropID::TextInactive:
            obj.Text.Inactive.Foreground = std::get<Color>(value);
            return true;
        case PropID::TextError:
            obj.Text.Error.Foreground = std::get<Color>(value);
            return true;
        case PropID::TextWarning:
           obj.Text.Warning.Foreground = std::get<Color>(value);
            return true;
        case PropID::TextFocused:
            obj.Text.Focused.Foreground = std::get<Color>(value);
            return true;
        case PropID::TextHovered:
            obj.Text.Hovered.Foreground = std::get<Color>(value);
            return true;
        case PropID::TextHighlighted:
            obj.Text.Highlighted.Foreground = std::get<Color>(value);
            return true;
        case PropID::TextEmphasized1:
            obj.Text.Emphasized1.Foreground = std::get<Color>(value);
            return true;
        case PropID::TextEmphasized2:
            obj.Text.Emphasized2.Foreground = std::get<Color>(value);
            return true;

        }
        error.SetFormat("Invalid property id (%d)", propertyID);
        return false;
    }
    void SetCustomPropertyValue(uint32 propertyID) override
    {
    }
    bool IsPropertyValueReadOnly(uint32 propertyID) override
    {
        NOT_IMPLEMENTED(false);
    }
    const vector<Property> GetPropertiesList() override
    {
#define PT(t)  static_cast<uint32>(t)
#define CAT(t) catNames[static_cast<uint32>(t)]
        return {
            { PT(PropID::DesktopChar), CAT(CatID::Desktop), "Symbol", PropertyType::Char16 },
            { PT(PropID::DesktopColor), CAT(CatID::Desktop), "Color", PropertyType::ColorPair },
            // Menus
            { PT(PropID::MenuTextNormal), CAT(CatID::Menu), "Text (normal)", PropertyType::ColorPair },
            { PT(PropID::MenuTextHovered), CAT(CatID::Menu), "Text (hovered)", PropertyType::ColorPair },
            { PT(PropID::MenuTextSelected), CAT(CatID::Menu), "Text (selected)", PropertyType::ColorPair },
            { PT(PropID::MenuHotKeyNormal), CAT(CatID::Menu), "HotKey (normal)", PropertyType::ColorPair },
            { PT(PropID::MenuHotKeyHovered), CAT(CatID::Menu), "HotKey (hovered)", PropertyType::ColorPair },
            { PT(PropID::MenuHotKeySelected), CAT(CatID::Menu), "HotKey (selected)", PropertyType::ColorPair },
            { PT(PropID::MenuShortCutNormal), CAT(CatID::Menu), "ShortCut (normal)", PropertyType::ColorPair },
            { PT(PropID::MenuShortCutHovered), CAT(CatID::Menu), "ShortCut (hovered)", PropertyType::ColorPair },
            { PT(PropID::MenuShortCutSelected), CAT(CatID::Menu), "ShortCut (selected)", PropertyType::ColorPair },
            { PT(PropID::MenuInactive), CAT(CatID::Menu), "Inactive", PropertyType::Color },
            { PT(PropID::MenuSymbolNormal), CAT(CatID::Menu), "Symbols (normal)", PropertyType::Color },
            { PT(PropID::MenuSymbolSelected), CAT(CatID::Menu), "Symbols (selected)", PropertyType::Color },
            { PT(PropID::MenuSymbolHovered), CAT(CatID::Menu), "Symbols (hovered)", PropertyType::Color },
            // parent menu
            { PT(PropID::ParentMenuTextNormal), CAT(CatID::ParentMenu), "Text (normal)", PropertyType::ColorPair },
            { PT(PropID::ParentMenuTextHovered), CAT(CatID::ParentMenu), "Text (hovered)", PropertyType::ColorPair },
            { PT(PropID::ParentMenuHotKeyNormal), CAT(CatID::ParentMenu), "HotKey (normal)", PropertyType::ColorPair },
            { PT(PropID::ParentMenuHotKeyHovered),
              CAT(CatID::ParentMenu),
              "HotKey (hovered)",
              PropertyType::ColorPair },
            { PT(PropID::ParentMenuShortCutNormal),
              CAT(CatID::ParentMenu),
              "ShortCut (normal)",
              PropertyType::ColorPair },
            { PT(PropID::ParentMenuShortCutHovered),
              CAT(CatID::ParentMenu),
              "ShortCut (hovered)",
              PropertyType::ColorPair },
            { PT(PropID::ParentMenuInactive), CAT(CatID::ParentMenu), "Inactive", PropertyType::Color },
            { PT(PropID::ParentMenuSymbolNormal), CAT(CatID::ParentMenu), "Symbols (normal)", PropertyType::Color },
            { PT(PropID::ParentMenuSymbolHovered), CAT(CatID::ParentMenu), "Symbols (hovered)", PropertyType::Color },
            // Window
            { PT(PropID::WindowNormal), CAT(CatID::Window), "Regular", PropertyType::Color },
            { PT(PropID::WindowInactive), CAT(CatID::Window), "Inactive", PropertyType::Color },
            { PT(PropID::WindowError), CAT(CatID::Window), "Error", PropertyType::Color },
            { PT(PropID::WindowInfo), CAT(CatID::Window), "Notification", PropertyType::Color },
            { PT(PropID::WindowWarning), CAT(CatID::Window), "Warning", PropertyType::Color },
            // ToolTip
            { PT(PropID::ToolTipText), CAT(CatID::ToolTip), "Text", PropertyType::ColorPair },
            { PT(PropID::ToolTipArrow), CAT(CatID::ToolTip), "Arrow", PropertyType::ColorPair },
            // Progress Bar
            { PT(PropID::ProgressBarEmpty), CAT(CatID::ProgressBar), "Empty", PropertyType::Color },
            { PT(PropID::ProgressBarFull), CAT(CatID::ProgressBar), "Full", PropertyType::Color },
            // Buttons
            { PT(PropID::ButtonTextNormal), CAT(CatID::Button), "Text (normal)", PropertyType::ColorPair },
            { PT(PropID::ButtonTextHovered), CAT(CatID::Button), "Text (hovered)", PropertyType::ColorPair },
            { PT(PropID::ButtonTextSelected), CAT(CatID::Button), "Text (pressed)", PropertyType::ColorPair },
            { PT(PropID::ButtonTextInactive), CAT(CatID::Button), "Text (inactive)", PropertyType::ColorPair },
            { PT(PropID::ButtonHotKeyNormal), CAT(CatID::Button), "HotKey (normal)", PropertyType::ColorPair },
            { PT(PropID::ButtonHotKeyHovered), CAT(CatID::Button), "HotKey (hovered)", PropertyType::ColorPair },
            { PT(PropID::ButtonHotKeySelected), CAT(CatID::Button), "HotKey (pressed)", PropertyType::ColorPair },
            { PT(PropID::ButtonHotKeyInactive), CAT(CatID::Button), "HotKey (inactive)", PropertyType::ColorPair },
            { PT(PropID::ButtonShadow), CAT(CatID::Button), "Shaddow", PropertyType::Color },
            // TExt
            { PT(PropID::TextNormal), CAT(CatID::Text), "Regular", PropertyType::Color },
            { PT(PropID::TextHotKey), CAT(CatID::Text), "Hot Key", PropertyType::Color },
            { PT(PropID::TextInactive), CAT(CatID::Text), "Inactive", PropertyType::Color },
            { PT(PropID::TextError), CAT(CatID::Text), "Error", PropertyType::Color },
            { PT(PropID::TextWarning), CAT(CatID::Text), "Warning", PropertyType::Color },
            { PT(PropID::TextFocused), CAT(CatID::Text), "Focused", PropertyType::Color },
            { PT(PropID::TextHovered), CAT(CatID::Text), "HOvered", PropertyType::Color },
            { PT(PropID::TextHighlighted), CAT(CatID::Text), "Highlighted", PropertyType::Color },
            { PT(PropID::TextEmphasized1), CAT(CatID::Text), "Emphasized (1)", PropertyType::Color },
            { PT(PropID::TextEmphasized2), CAT(CatID::Text), "Emphasized (2)", PropertyType::Color },

        };
#undef PT
#undef CAT
    };
};
class PreviewControl : public UserControl
{
    Reference<ConfigProperty> config;

  public:
    PreviewControl() : UserControl("d:c")
    {
    }
    void Paint(Graphics::Renderer& r) override
    {
        Size sz;
        GetSize(sz);
        if (config.IsValid())
            config->Paint(r, sz);
    }
    void SetConfig(Reference<ConfigProperty> _config)
    {
        config = _config;
    }
};
class ThemeEditorDialog : public Window
{
    Reference<PropertyList> prop;
    ConfigProperty cfg;
    Reference<PreviewControl> pc;

  public:
    ThemeEditorDialog(const AppCUI::Application::Config& configObject)
        : Window("Theme editor", "d:c,w:80,h:24", WindowFlags::Sizeable), cfg(configObject)
    {
        auto sp = Factory::Splitter::Create(this, "l:0,t:0,b:3,r:0", true);
        sp->SetSecondPanelSize(30);
        pc = sp->CreateChildControl<PreviewControl>();
        pc->SetConfig(&cfg);
        prop = Factory::PropertyList::Create(sp, "d:c", &cfg, PropertyListFlags::None);
        UpdateCategoryAndProperty();
        Factory::Button::Create(this, "&Close", "r:1,b:0,w:12", BUTTON_CMD_CLOSE);
    }
    void UpdateCategoryAndProperty()
    {
        auto propID = prop->GetCurrentItemID();
        if (propID.has_value())
        {
            cfg.SetCategoryAndProperty(prop->GetCurrentItemCategory(), static_cast<PropID>(propID.value()));
        }
        else
        {
            cfg.SetCategoryAndProperty(prop->GetCurrentItemCategory(), PropID::None);
        }
    }
    bool OnEvent(Reference<Control> control, Event eventType, int ID) override
    {
        if (Window::OnEvent(control, eventType, ID))
            return true;
        if (eventType == Event::ButtonClicked)
        {
            switch (ID)
            {
            case BUTTON_CMD_CLOSE:
                this->Exit(0);
                return true;
            }
        }
        if (eventType == Event::PropertyItemChanged)
        {
            UpdateCategoryAndProperty();
            return true;
        }
        return false;
    }
};

void ThemeEditor::Show()
{
    auto* cfg = Application::GetAppConfig();
    if (cfg)
    {
        ThemeEditorDialog dlg(*cfg);
        dlg.Show();
    }
}
} // namespace AppCUI::Dialogs