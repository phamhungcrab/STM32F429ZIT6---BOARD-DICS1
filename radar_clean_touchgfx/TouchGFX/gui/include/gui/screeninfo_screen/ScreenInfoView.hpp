#ifndef SCREENINFOVIEW_HPP
#define SCREENINFOVIEW_HPP

#include <gui_generated/screeninfo_screen/ScreenInfoViewBase.hpp>
#include <gui/screeninfo_screen/ScreenInfoPresenter.hpp>
#include <touchgfx/Unicode.hpp>
#include <stdint.h>

class ScreenInfoView : public ScreenInfoViewBase
{
public:
    ScreenInfoView();
    virtual ~ScreenInfoView() {}

    virtual void setupScreen();
    virtual void tearDownScreen();

    /*
     * TouchGFX gọi hàm này mỗi frame.
     * Ta dùng nó để cập nhật thông tin màn Info định kỳ.
     */
    virtual void handleTickEvent();

protected:
    /*
     * Buffer cho các wildcard trên ScreenInfo.
     *
     * TouchGFX Designer nên có:
     * MODE: <scanMode>
     * SPEED: <speedMode>
     * RANGE: <lastRange>
     * LAST ANGLE: <lastAngle>
     * OBJECTS: <objectCount>
     */
    static const uint16_t INFO_BUF_SIZE = 16;

    touchgfx::Unicode::UnicodeChar scanModeBuffer[INFO_BUF_SIZE];
    touchgfx::Unicode::UnicodeChar speedModeBuffer[INFO_BUF_SIZE];
    touchgfx::Unicode::UnicodeChar minRangeBuffer[INFO_BUF_SIZE];
    touchgfx::Unicode::UnicodeChar lastAngleBuffer[INFO_BUF_SIZE];
    touchgfx::Unicode::UnicodeChar objectCountBuffer[INFO_BUF_SIZE];

    uint8_t tickCounter;

    void updateInfoText();
};

#endif // SCREENINFOVIEW_HPP
