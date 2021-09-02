#pragma once

namespace brokenithm
{
#pragma pack(1)
    struct color_t
    {
        uint8_t buf[3];

    public:
        color_t(int value)
        {
            // brg????
            buf[1] = static_cast<uint8_t>(value >> 16 & 0xff);
            buf[2] = static_cast<uint8_t>(value >> 8 & 0xff);
            buf[0] = static_cast<uint8_t>(value & 0xff);
        }
    };

    struct ipc_memory_info
    {
        uint8_t air_io_status[6];
        uint8_t slider_io_status[32];
        color_t colors[32];
        uint8_t test_btn;
        uint8_t serviceB_btn;
        uint8_t coin_insertion;
        uint8_t card_read;
    };
#pragma pack(pop)

    void init();
    mu3::io::game_buttons get_key(bool right);
    void free();
}

