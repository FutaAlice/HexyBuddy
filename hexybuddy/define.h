#pragma once

namespace hexybuddy {

#define TARGET_IAMGE_NAME	L"Hexy.exe"
#define TARGET_WINDOW_CAPTION	L"Hexy"

enum
{
    pawnNum_offset = 0x01B0C5D0,
    boardSize_offset = 0x0042303C,
    boardRatio_offset = 0x01B0BFC8,
    gameOver_offset = 0x01B0CCE0,
    canvasCenterX_offset = 0x01b0c328,
    canvasCenterY_offset = 0x01b0c32c,
    globalPosList_offset = 0x01B073A0,
    hexygonCenterPairList_offset = 0x01B075C0,
};

}
