#include <Wire.h>
#include <SoftwareSerial.h>
#include <robo2019.h>

/**
 * @brief 自作のオートポインタ型
 * @tparam T 保持するポインタが指すデータの型
 * @details
 *  与えられたポインタの所有権を保持する。具体的には次の通り。
 *  - 次の場合に自動的にdeleteされる。
 *      - auto_ptr型のオブジェクトが破棄されるとき
 *      - reset(new_ptr)で新しいポインタが設定されるとき
 *  所有権を解放するにはrelease()を使う
 */
template <typename T>
class auto_ptr {
private:
    T * _ptr;
public:
    auto_ptr() : _ptr(NULL) {}
    auto_ptr(T * ptr) : _ptr(ptr) {}
    auto_ptr(auto_ptr & aptr) {
        _ptr = aptr.release();
    }

    ~auto_ptr() { del(); }

    auto_ptr & operator = (const auto_ptr & rh) = delete;

    inline bool operator == (const auto_ptr & rh) { return _ptr == rh._ptr; }
    inline bool operator != (const auto_ptr & rh) { return _ptr != rh._ptr; }
    inline bool operator == (const T * ptr) { return _ptr == ptr; }
    inline bool operator != (const T * ptr) { return _ptr != ptr; }

    inline T * operator -> () { return  _ptr; }
    inline T & operator  * () { return *_ptr; }

    inline operator bool () const { return _ptr != NULL; }

    T * release() {
        T * res = _ptr;
        _ptr = NULL;
        return res;
    }

    inline void del() { if (_ptr != NULL) delete _ptr; }

    void reset(T * ptr) {
        del();
        _ptr = ptr;
    }
};

// robo::move_infoのエイリアス
namespace info {
    using namespace robo::move_info;
}

// robo::openmvのエイリアス
namespace omv {
    using namespace robo::openmv;
}

// half-pi
constexpr float HPI = PI / 2;
constexpr float QPI = PI / 4;
// 「正面」の範囲
constexpr float front_range = PI / 10;
// 機体の移動スピード
constexpr int8_t max_speed = 100;
// キッカーのピン番号(不使用)
constexpr uint8_t kicker_pin = 10;

SoftwareSerial motor_ser(12, 13);
robo::Motor motor(&motor_ser);
auto_ptr<info::MoveInfo> m_info;

// ラインセンサー群
namespace lines {
    robo::LineSensor left(1), right(2), back(3);

    /**
     * @brief センサーで読み取った値が白かどうかを判定する
     * @param val センサーの値
     * @return true 白
     * @return false 黒
     */
    constexpr bool iswhite(uint16_t val) {
        return val >= 450;
    }
}

// 超音波センサー群
namespace uss {
    robo::USSensor left(1, 2), right(3, 4), back(5, 6);
}

omv::Reader mv_reader(0x12);
using FramePtr = auto_ptr<omv::Frame>;
FramePtr frame;
robo::BNO055 bno055(0, 0x28);
robo::LCD lcd(0x27, 16, 2);

uint8_t frame_count = 0;

void setup() {
    uss::left.setup();
    uss::right.setup();
    uss::back.setup();

    lines::left.setup();
    lines::right.setup();
    lines::back.setup();

    mv_reader.setup();

    bno055.setup();

    motor_ser.begin(19200);
    motor.stop();
    Serial.begin(19200);
    m_info.reset(new info::Stop());

    lcd.setup();
}

void loop() {
    // ラインセンサーの値を取得
    #define L_BIND(_name_) w_ ## _name_ = lines::iswhite(lines::_name_.read())
    const bool L_BIND(left), L_BIND(right), L_BIND(back);
    #undef L_BIND
    const bool on_line = w_left || w_right || w_back;

    // OpenMV
    FramePtr nframe(mv_reader.read_frame());
    if (nframe) frame.reset(nframe.release());
    // エイリアス
    using PosPtr = omv::Position *;
    // ボールの座標(OpenMVが見つけていなかったらNULL)
    PosPtr ball_pos = frame ? frame->ball_pos : NULL;
    //　黄色のゴールの座標
    PosPtr y_goal_pos = frame ? frame->y_goal_pos : NULL;
    // 黄色のゴールの方向(10はとにかく大きい値というだけで深い意味なし)
    float y_goal_dir = y_goal_pos ? omv::pos2dir(*y_goal_pos) : 10;

    // BNO055で現在の方向を取得(方向の定義はrobo2019/README参照)
    float bno_dir = bno055.get_geomag_direction();

    // ラインセンサー処理(アウトオブバウンズ対策)
    if (on_line) { // 線を踏んだ
        float d = 0.0;
        if (abs(y_goal_dir) < front_range) {
            // ゴールが前にある => 前にペナルティエリアがある
            d = PI;
        } else {
            // ペナルティエリア以外
            //#define USE_USS
            #ifdef USE_USS
            #define BIND(_name_) e_ ## _name_ = uss::_name_.read()
            uint16_t BIND(left), BIND(right), BIND(back);
            #undef BIND
            d = (e_back < e_right && e_back < e_left)
                ? 0.0 // 後ろの壁が一番近い => 前に進む
                : e_left < e_right ? -HPI : HPI; // 左の方が近い ? 右に進む : 左に進む
            #else /* USE_USS */
            d = w_left == w_right
            ? (w_back ? 0.0 : PI) // 左と右どちらも同じ色 => 後ろのラインセンサーに応じて決定
            : (w_left ? -HPI : HPI); // 左右で色が違う => 左が白 ? 右に進む : 左に進む
            #endif /* USE_USS */
        }
        m_info.reset(new info::Translate(
            robo::V2_float::from_polar_coord(d, max_speed)
        ));
        goto MOTOR;
    }

    // 姿勢制御
    BNO: {
        float adir = abs(bno_dir);
        // 左右どちらかわからないが、正面を向いていない
        if (adir > front_range) {
            m_info.reset(new info::Rotate(bno_dir > 0, int8_t(adir * 25 + 20)));
            // (adir - 0) / (PI - 0) * (100 - 20) + 20
            // -> adir * 25 + 40
            goto MOTOR;
        }
    }

    // ボールを追う
    BALL:
    if (ball_pos != NULL) {
        float ball_dir = omv::pos2dir(*ball_pos);
        m_info.reset(new info::Translate(
            // ボールの角度から3/2倍した方向に動いて回り込みを実現
            robo::V2_float::from_polar_coord(ball_dir * 3 /2 , max_speed)
        ));
        goto MOTOR;
    }

    // 何もすることがないため停止
    m_info.reset(new info::Stop());

    // モーターのパワーを更新
    MOTOR: {
        if (m_info) m_info->apply(motor);
    }

    // ログをとる
    LOG:
    if (++frame_count == 10) {
        //lcd.clear();
        char buff[128] = "";
        if (ball_pos != NULL) {
            ball_pos->to_string(buff);
        } else {
            strcat_P(buff, PSTR("no ball"));
        }
        lcd.setCursor(0,0);
        lcd.print(buff);
        sprintf_P(buff, PSTR("l:%u,r:%u,b:%u"), w_left, w_right, w_back);
        lcd.setCursor(0, 1);
        lcd.print(buff);
        buff[0] = '\0';
        if (m_info) {
            m_info->to_string(buff);
        }
        Serial.println(buff);
        frame_count = 0;
    }
}
