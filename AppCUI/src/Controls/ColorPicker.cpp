#include "ControlContext.hpp"

namespace AppCUI
{
constexpr int32 COLORPICEKR_HEIGHT = 7;
void ColorPickerContext::OnExpandView(Graphics::Clip& expandedClip)
{
    Size size;
    if (!AppCUI::Application::GetApplicationSize(size))
        return;
    expandedClip.ClipRect.Height = COLORPICEKR_HEIGHT;
    this->headerYOffset          = 0;
    this->yOffset                = 1;
    if (expandedClip.ScreenPosition.Y + COLORPICEKR_HEIGHT >= (int32) size.Height)
    {
        this->headerYOffset = COLORPICEKR_HEIGHT - 1;
        this->yOffset       = 0;
        expandedClip.ScreenPosition.Y -= this->headerYOffset;
        expandedClip.ClipRect.Y = expandedClip.ScreenPosition.Y;
    }    
}
void ColorPickerContext::PaintHeader(int x, int y, uint32 width, Graphics::Renderer& renderer)
{
    auto* cbc = &this->Cfg->ComboBox.Normal;
    if ((this->Flags & GATTR_ENABLE) == 0)
        cbc = &this->Cfg->ComboBox.Inactive;
    if (this->Focused)
        cbc = &this->Cfg->ComboBox.Focus;
    else if (this->MouseIsOver)
        cbc = &this->Cfg->ComboBox.Hover;

    if (width > 5)
    {
        renderer.FillHorizontalLine(x, y, x + (int) width - 6, ' ', cbc->Text);
        renderer.WriteSingleLineText(x + 3, y, width - 4, ColorUtils::GetColorName(this->color), cbc->Text);
        renderer.WriteSpecialCharacter(
              x + 1, y, SpecialChars::BlockCentered, ColorPair{ this->color, Color::Transparent });
    }
    if (width >= 3)
    {
        renderer.WriteSingleLineText(x + (int) width - 3, y, "   ", cbc->Button);
        renderer.WriteSpecialCharacter(x + (int) width - 2, y, SpecialChars::TriangleDown, cbc->Button);
    }
}
void ColorPickerContext::PaintColorBox(Graphics::Renderer& renderer)
{
    const auto col = Cfg->ComboBox.Focus.Text;
    renderer.FillRect(0, this->yOffset, this->Layout.Width - 1, this->yOffset + COLORPICEKR_HEIGHT - 2, ' ', col);
    renderer.DrawRect(0, this->yOffset, this->Layout.Width - 1, this->yOffset + COLORPICEKR_HEIGHT - 2, col, false);
    // draw colors
    for (auto y = 0U; y < 4; y++)
    {
        for (auto x = 0U; x < 4; x++)
        {
            auto c = static_cast<Color>(y * 4 + x);
            renderer.FillHorizontalLineSize(x * 3 + 1, y + 1 + this->yOffset, 3, ' ', ColorPair{Color::Black,c});
        }
    }
}
void ColorPickerContext::Paint(Graphics::Renderer& renderer)
{
    PaintHeader(0, this->headerYOffset, this->Layout.Width, renderer);
    if (this->Flags & GATTR_EXPANDED)
        PaintColorBox(renderer);
}

ColorPicker::ColorPicker(string_view layout, Graphics::Color _color)
    : Control(new ColorPickerContext(), "", layout, false)
{
    auto Members              = reinterpret_cast<ColorPickerContext*>(this->Context);
    Members->Layout.MinWidth  = 7;
    Members->Layout.MinHeight = 1;
    Members->Layout.MaxHeight = 1;
    Members->Flags            = GATTR_ENABLE | GATTR_VISIBLE | GATTR_TABSTOP;
    Members->color            = _color;
    Members->headerYOffset    = 0;
    Members->yOffset          = 1;
}
ColorPicker::~ColorPicker()
{
}
void ColorPicker::Paint(Graphics::Renderer& renderer)
{
    reinterpret_cast<ColorPickerContext*>(this->Context)->Paint(renderer);
}
bool ColorPicker::OnKeyEvent(Input::Key keyCode, char16 UnicodeChar)
{
    NOT_IMPLEMENTED(false);
}
void ColorPicker::OnHotKey()
{
    SetChecked(!IsChecked());
    if (IsChecked())
        this->ExpandView();
    else
    {
        this->PackView();
    }
}
bool ColorPicker::OnMouseLeave()
{
    return true;
}
bool ColorPicker::OnMouseEnter()
{
    return true;
}
bool ColorPicker::OnMouseOver(int x, int y)
{
    NOT_IMPLEMENTED(false);
}
void ColorPicker::OnMousePressed(int x, int y, Input::MouseButton button)
{
    OnHotKey();
}
void ColorPicker::OnExpandView(Graphics::Clip& expandedClip)
{
    reinterpret_cast<ColorPickerContext*>(this->Context)->OnExpandView(expandedClip);
}
void ColorPicker::OnPackView()
{
    reinterpret_cast<ColorPickerContext*>(this->Context)->headerYOffset = 0; // reset position
}

} // namespace AppCUI