#pragma once

namespace mu3::io
{
    enum op_buttons : uint8_t
    {
        test = 0x01,
        service = 0x02,
    };

    enum game_buttons : uint8_t
    {
        none = 0x0,
        a = 0x01,
        b = 0x02,
        c = 0x04,
        side = 0x08,
        menu = 0x10,
    };
}

extern "C"
{
    uint16_t mu3_io_get_api_version(void);
    HRESULT mu3_io_init(void);
    HRESULT mu3_io_poll(void);
    void mu3_io_get_opbtns(uint8_t* opbtn);
    void mu3_io_get_gamebtns(uint8_t* left, uint8_t* right);
    void mu3_io_get_lever(int16_t* pos);
}
