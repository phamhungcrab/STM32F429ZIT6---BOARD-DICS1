#ifndef SCREENSETTINGSVIEW_HPP
#define SCREENSETTINGSVIEW_HPP

#include <gui_generated/screensettings_screen/ScreenSettingsViewBase.hpp>
#include <gui/screensettings_screen/ScreenSettingsPresenter.hpp>
#include <touchgfx/events/ClickEvent.hpp>
#include <touchgfx/Unicode.hpp>
#include <stdint.h>

class ScreenSettingsView : public ScreenSettingsViewBase
{
public:
    ScreenSettingsView();
    virtual ~ScreenSettingsView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
    virtual void handleClickEvent(const touchgfx::ClickEvent& evt);

protected:
    static const uint16_t SETTING_SPEED_BUF_SIZE = 10;
    static const uint16_t SETTING_SCAN_BUF_SIZE  = 10;

    touchgfx::Unicode::UnicodeChar speedModeBuffer[SETTING_SPEED_BUF_SIZE];
    touchgfx::Unicode::UnicodeChar scanModeBuffer[SETTING_SCAN_BUF_SIZE];

    uint8_t tickCounter;

    void updateConfigText();
    bool isClicked(const touchgfx::Drawable& widget, const touchgfx::ClickEvent& evt);
};

#endif // SCREENSETTINGSVIEW_HPP
