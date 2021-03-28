#include <Arduino.h>
#include "openmv.h"

//implementations of robo::openmv::Frame
#define POS_ARGS(_name_) uint16_t _name_ ## _x, uint16_t _name_ ## _y
#define INIT_POS(_name_) _name_ ## _pos(new Position(_name_ ## _x, _name_ ## _y))
robo::openmv::Frame::Frame(
    POS_ARGS(ball), POS_ARGS(y_goal), POS_ARGS(b_goal)
) : INIT_POS(ball), INIT_POS(y_goal), INIT_POS(b_goal) {}
#undef POS_ARGS
#undef INIT_POS

robo::openmv::Frame::Frame(
    robo::openmv::Position * ball_pos,
    robo::openmv::Position * y_goal_pos,
    robo::openmv::Position * b_goal_pos
) : ball_pos(ball_pos), y_goal_pos(y_goal_pos), b_goal_pos(b_goal_pos) {}

robo::openmv::Frame::~Frame()
{
    #define DELETE(_name_) if (_name_ ## _pos != NULL) delete _name_ ## _pos;
    DELETE(ball);
    DELETE(y_goal);
    DELETE(b_goal);
    #undef DELETE
}

uint8_t robo::openmv::Frame::to_string(char * dst)
{
    if (dst == NULL) return 0;
    char *ptr = dst;
    #define WRITE(_name_) \
    ptr += sprintf_P(ptr, PSTR(#_name_ " pos: ")); \
    if (_name_ ## _pos != NULL) ptr += _name_ ## _pos->to_string(ptr); \
    *(ptr++) = '\n';

    WRITE(ball)
    WRITE(y_goal)
    WRITE(b_goal)

    #undef WRITE
    *ptr = '\0';
    return ptr - dst;
}

//implementations of robo::openmv::Reader
robo::openmv::Reader::Reader(uint8_t addr, TwoWire & wire) : _wire(wire), address(addr) {}

void robo::openmv::Reader::pass_data(uint8_t size)
{
    for (uint8_t i = 0; i < size; i++) _wire.read();
}

uint16_t robo::openmv::Reader::read_2byte()
{
    return _wire.read() | (_wire.read() << 8);
}

robo::openmv::Position * robo::openmv::Reader::read_pos()
{
    constexpr uint16_t default_value = 0xffff;
    uint16_t x = read_2byte();
    uint16_t y = read_2byte();
    if (x == default_value && y == default_value) return NULL;
    return new robo::openmv::Position(x, y);
}

void robo::openmv::Reader::setup()
{
    _wire.begin();
}

robo::openmv::Frame * robo::openmv::Reader::read_frame()
{
    constexpr uint8_t req_size = 3 * 4;
    uint8_t res_size = _wire.requestFrom(address, req_size);
    robo::openmv::Frame *frame = NULL;
    robo::openmv::Position *ball_pos, *y_goal_pos, *b_goal_pos;
    if (res_size != req_size) {
        pass_data(res_size);
        goto FINAL;
    }
    ball_pos = read_pos();
    y_goal_pos = read_pos();
    b_goal_pos = read_pos();
    if (ball_pos == NULL && y_goal_pos == NULL && b_goal_pos == NULL) goto FINAL;
    frame = new robo::openmv::Frame(ball_pos, y_goal_pos, b_goal_pos);
    // goto FINAL;
    FINAL:
    _wire.beginTransmission(address);
    _wire.write(1);
    _wire.endTransmission();
    return frame;
}

