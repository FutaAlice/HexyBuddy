#pragma once

namespace hexybuddy {

enum class Command : unsigned {
    NewGame = 25,
    OpenGame = 26,
    SaveGameAs = 27,
    AboutHexy = 36,
    Registration = 37,
    LevelBeginner = 41,
    LevelIntermediate = 42,
    LevelAdvanced = 43,
    LevelExpert = 44,
    Hint = 50,
    Exit = 52,
    HexyOff = 60,
    HexyOn = 61,
    Demo = 62,
    Move = 63,
    Rules = 100,
    HowToPlay = 101,
    More_help = 102,
    UserArgeement = 103,
    Size_4 = 104,
    Size_5 = 105,
    Size_6 = 106,
    Size_7 = 107,
    Size_8 = 108,
    Size_9 = 109,
    Size_10 = 110,
    Size_11 = 111,
    Clock = 127,
    Sound = 200,
    Coord = 201,
    ShapeDiamond = 210,
    ShapeFlat_1 = 211,
    ShapeFlat_2 = 212,
    Guess = 230,
    SwapOff = 240,
    SwapOn = 241,
    Score = 400,
    Random = 511,
    Unknown = 729,
    Numbering = 730,
};

} // namespace hexybuddy