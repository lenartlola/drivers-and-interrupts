#include "kps_driver.h"

t_key_data en_us[0x5E] = {
		[0x00] = {"None", 0x0},
		[0x01] = {"Escape", 0x0},
		[0x02] = {"1", '1'},
		[0x03] = {"2", '2'},
		[0x04] = {"3", '3'},
		[0x05] = {"4", '4'},
		[0x06] = {"5", '5'},
		[0x07] = {"6", '6'},
		[0x08] = {"7", '7'},
		[0x09] = {"8", '8'},
		[0x0A] = {"9", '9'},
		[0x0B] = {"0", '0'},
		[0x0C] = {"-", '-'},
		[0x0D] = {"+", '+'},
		[0x0E] = {"BackSP", 0x0},
		[0x0F] = {"Tab", 0x0},
		[0x10] = {"Q", 'q'},
		[0x11] = {"W", 'w'},
		[0x12] = {"E", 'e'},
		[0x13] = {"R", 'r'},
		[0x14] = {"T", 't'},
		[0x15] = {"Y", 'y'},
		[0x16] = {"U", 'u'},
		[0x17] = {"I", 'i'},
		[0x18] = {"O", 'o'},
		[0x19] = {"P", 'p'},
		[0x1A] = {"[", '['},
		[0x1B] = {"]", ']'},
		[0x1C] = {"Enter", '\n'},
		[0x1D] = {"Left Control", 0x0},
		[0x1E] = {"A", 'a'},
		[0x1F] = {"S", 's'},
		[0x20] = {"D", 'd'},
		[0x21] = {"F", 'f'},
		[0x22] = {"G", 'g'},
		[0x23] = {"H", 'h'},
		[0x24] = {"J", 'j'},
		[0x25] = {"K", 'k'},
		[0x26] = {"L", 'l'},
		[0x27] = {";", ';'},
		[0x28] = {"Single Quote", '\''},
		[0x29] = {"Back Tick", '`'},
		[0x2A] = {"Left Shift", 0x0},
		[0x2B] = {"Back Slash", '\\'},
		[0x2C] = {"Z", 'z'},
		[0x2D] = {"X", 'x'},
		[0x2E] = {"C", 'c'},
		[0x2F] = {"V", 'v'},
		[0x30] = {"B", 'b'},
		[0x31] = {"N", 'n'},
		[0x32] = {"M", 'm'},
		[0x33] = {"Comma", ','},
		[0x34] = {"Full Stop", '.'},
		[0x35] = {"Slash", '/'},
		[0x36] = {"Right Shift", 0x0},
		[0x37] = {"Star", '*'},
		[0x38] = {"Left Alt", 0x0},
		[0x39] = {"Space", ' '},
		[0x3A] = {"Caps Lock", 0x0},
		[0x3B] = {"F1", 0x0},
		[0x3C] = {"F2", 0x0},
		[0x3D] = {"F3", 0x0},
		[0x3E] = {"F4", 0x0},
		[0x3F] = {"F5", 0x0},
		[0x40] = {"F6", 0x0},
		[0x41] = {"F7", 0x0},
		[0x42] = {"F8", 0x0},
		[0x43] = {"F9", 0x0},
		[0x44] = {"F10", 0x0},
		[0x45] = {"Num Lock", 0x0},
		[0x46] = {"Scroll Lock", 0x0},
		[0x47] = {"7", '7'},
		[0x48] = {"8", '8'},
		[0x49] = {"9", '9'},
		[0x4A] = {"-", '-'},
		[0x4B] = {"4", '4'},
		[0x4C] = {"5", '5'},
		[0x4D] = {"6", '6'},
		[0x4E] = {"+", '+'},
		[0x4F] = {"1", '1'},
		[0x50] = {"2", '2'},
		[0x51] = {"3", '3'},
		[0x52] = {"0", '0'},
		[0x53] = {"Full Stop", '.'},
		[0x54] = {"None", 0x0},
		[0x55] = {"None", 0x0},
		[0x56] = {"None", 0x0},
		[0x57] = {"None", 0x0},
		[0x58] = {"F12", 0x0},
		[0x59] = {"None", 0x0},
		[0x5A] = {"None", 0x0},
		[0x5B] = {"LWin", 0x0},
		[0x5C] = {"RWin", 0x0},
		[0x5D] = {"Apps", 0x0},
};

