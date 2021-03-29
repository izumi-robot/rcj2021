#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial mySer(10, 11); //RX, TX
LiquidCrystal_I2C lcd(0x27, 16, 2);

Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28);    //コンパス

volatile bool switch_con = true;
bool judge_bno = true;
bool judge_stay = true;

const uint8_t color_pin[] = {1, 2, 3};

uint8_t push_count = 0;

//モーターパワー
uint8_t m[4];   //回転の向き決定用  左前、右前、右後ろ、左後ろ
uint8_t m_power[4];
const String Direction[] = {"F", "R"};
const uint8_t motor_pin[] = {1, 2, 4, 3};


void print_lcd(String words, uint8_t k) {
  lcd.setCursor(0, k);
  lcd.print(words);
}

void interrupt() {
  switch_con != switch_con;
}

void stop_motor() {
  m_power[0] = m_power[1] = m_power[2] = m_power[3] = 0;
}

void init_motor() {
  m_power[1] = m_power[3] = m_power[0] = m_power[2] = 30;
}

void motor_ctrl() {
  String d = Direction[m[0]];
  String p = "0" + String(m_power[0]);
  if (p.length() == 2) {
    p = "0" + p;
  }
  mySer.println(String(motor_pin[0] + d + p));

  d = Direction[m[1]];
  p = "0" + String(m_power[1]);
  if (p.length() == 2) {
    p = "0" + p;
  }
  mySer.println(String(motor_pin[1] + d + p));
  

  d = Direction[m[2]];
  p = "0" + String(m_power[2]);
  if (p.length() == 2) {
    p = "0" + p;
  }
  mySer.println(String(motor_pin[2] + d + p));

  d = Direction[m[3]];
  p = "0" + String(m_power[3]);
  if (p.length() == 2) {
    p = "0" + p;
  }
  mySer.println(String(motor_pin[3] + d + p));
}

struct CamPos {
public:
  uint16_t x, y;
  CamPos(uint8_t x, uint8_t y) : x(x), y(y) {}
  uint8_t to_string(char *dst)
  {
    if (dst == NULL)
      return 0;
    return sprintf(dst, "(%u, %u)", x, y);
  }
};

bool operator==(const CamPos &lh, const CamPos &rh)
{
  return lh.x == rh.x && lh.y == rh.y;
}

bool operator!=(const CamPos &lh, const CamPos &rh)
{
  return lh.x != rh.x || lh.y != rh.y;
}

class Frame
{
public:
  CamPos *ball_pos, *yellow_goal_pos, *blue_goal_pos;
#define X(_name_) _name_##_x
#define Y(_name_) _name_##_y
#define PARAM(_name_) uint16_t X(_name_), uint16_t Y(_name_)
#define NEW_POS(_name_) new CamPos(X(_name_), Y(_name_))
  Frame(PARAM(ball), PARAM(yellow_goal), PARAM(blue_goal))
      : ball_pos(NEW_POS(ball)), yellow_goal_pos(NEW_POS(yellow_goal)), blue_goal_pos(NEW_POS(blue_goal))
  {
  }
#undef X
#undef Y
#undef PARAM
#undef NEW_POS
  Frame(CamPos *ball_pos, CamPos *yellow_goal_pos, CamPos *blue_goal_pos)
      : ball_pos(ball_pos), yellow_goal_pos(yellow_goal_pos), blue_goal_pos(blue_goal_pos)
  {
  }
  ~Frame()
  {
#define DELETE(_name_)      \
  if (_name_##_pos != NULL) \
    delete _name_##_pos;
    DELETE(ball)
    DELETE(yellow_goal)
    DELETE(blue_goal)
#undef DELETE
  }
  uint8_t to_string(char *dst)
  {
    if (dst == NULL)
      return 0;
    char *ptr = dst;
    if (ball_pos != NULL)
    {
      ptr += sprintf(ptr, "ball_pos: ");
      ptr += ball_pos->to_string(ptr);
      *(ptr++) = '\n';
    }
    if (yellow_goal_pos != NULL)
    {
      ptr += sprintf(ptr, "yellow_goal_pos: ");
      ptr += yellow_goal_pos->to_string(ptr);
      *(ptr++) = '\n';
    }
    if (blue_goal_pos != NULL)
    {
      ptr += sprintf(ptr, "blue_goal_pos: ");
      ptr += blue_goal_pos->to_string(ptr);
      *(ptr++) = '\n';
    }
    *ptr = '\0';
    return ptr - dst;
  }
};

class OpenMV
{
private:
  TwoWire &_wire;

  uint16_t read_2byte() { return _wire.read() | ((uint16_t)_wire.read() << 8); }
  CamPos *read_pos()
  {
    constexpr uint16_t default_value = 0xffff;
    uint16_t x = read_2byte();
    uint16_t y = read_2byte();
    return (x != default_value || y != default_value) ? new CamPos(x, y) : NULL;
  }

public:
  const uint8_t address;

  OpenMV() : _wire(Wire), address(0x12) {}
  OpenMV(uint8_t addr) : _wire(Wire), address(addr) {}
  OpenMV(TwoWire &wire, uint8_t addr) : _wire(wire), address(addr) {}
  OpenMV(uint8_t addr, TwoWire &wire) : _wire(wire), address(addr) {}

  void setup() { _wire.begin(); }
  Frame *read_frame()
  {
    constexpr uint8_t req_size = 3 * 4;
    uint8_t res_size = _wire.requestFrom(address, req_size);
    if (res_size != req_size)
      return NULL;
    CamPos *ball = read_pos();
    CamPos *yellow = read_pos();
    CamPos *blue = read_pos();
    _wire.beginTransmission(address);
    _wire.write(1);
    _wire.endTransmission();
    if (ball == NULL && yellow == NULL && blue == NULL)
      return NULL;
    return new Frame(ball, yellow, blue);
  }
};

OpenMV *openmv;
char buffer[128] = "";


int read_compass() {  //地磁気読み込み  正面0 //x回転のみ調べてる
  static int16_t compass_x_pri;
  static bool t = true;

  double euler_x = bno.getVector(Adafruit_BNO055::VECTOR_EULER).x();

  if (t) { // 最初の方向の取得(一回のみ)
    compass_x_pri = euler_x;
    t = false;
  }

  return int16_t(euler_x - compass_x_pri) % 360 - 180; //反時計回りに180～-180
}

void compass() { //コンパスチェック→角度、距離を出す→動く
  int16_t com = read_compass();

  if (abs(com) < 165) {
    judge_bno = true;
    if (com > 0) {
      m[0] = 1 , m[1] = 0 , m[2] = 0 , m[3] = 1;
      for (uint8_t i = 0; i < 4; i++) {
        m_power[i] = min(int(abs(com - 180.0) / 180.0 * 100), 30);
        m_power[i] = max(m_power[i], 20);
      }
    } else {
      m[0] = 0 , m[1] = 1 , m[2] = 1 , m[3] = 0;
      for (uint8_t i = 0; i < 4; i++) {
        m_power[i] = min(int(abs(com + 180.0) / 180.0 * 100), 30);
        m_power[i] = max(m_power[i], 20);
      }
    }
    motor_ctrl();
  } else {
    judge_bno = false;
  }
}

void motor_dire(uint8_t m1, uint8_t m2, uint8_t m3, uint8_t m4) { //後ろならfalse
  m[0] = m1, m[1] = m2, m[2] = m3, m[3] = m4;
}

void push(uint8_t pos_y ) {
  push_count = 0;
  
  m_power[0] = m_power[1] = m_power[2] = m_power[3] = 60;
  motor_dire(1, 1, 1, 1);
  motor_ctrl();
  delay(abs(pos_y - 60) * 10);

  stop_motor();
  delay(20);
  
  m_power[0] = m_power[1] = m_power[2] = m_power[3] = 40;
  motor_dire(0, 0, 0, 0);
  motor_ctrl();
  delay(abs(pos_y - 60) * 10);
}

void Line() {
  static uint8_t count_move = 0;
  static bool goal_color;
  uint8_t pos_x, pos_y;
  Frame *frame = openmv->read_frame();
  
  if(frame->ball_pos != NULL){
    pos_x = frame->ball_pos->x;
    pos_y = frame->ball_pos->y;
    
    if (pos_x < 73 || pos_x > 87) {
      if (pos_x > 87) {
        motor_dire(0, 1, 0, 1);
      } else if (pos_x < 73) {
        motor_dire(1, 0, 1, 0);
      }
      uint8_t pro = max(20, abs(uint8_t(pos_x) - 80)*3);
      m_power[0] = m_power[1] = m_power[2] = m_power[3] = min(40, pro);
      motor_ctrl();
      push_count = 0;
    } else if (73 <= pos_x && pos_x <= 87 && pos_y <= 40 && pos_y >= 21) {
      push_count++;
      if(push_count > 30){
        push(int(pos_y));
      }
      stop_motor();
      motor_ctrl();
    }
    delete frame;
  } else if (count_move > 6) {
      stop_motor();
      count_move = 0;
      motor_ctrl();
  } else {
      count_move++;
  }
  
}

void stay() {
  init_motor();
  bool j_move=false;
  uint16_t c1 = analogRead(1), c2 = analogRead(2), c3 = analogRead(3);  //1::left  2::right  3::back
  Frame *frame = openmv->read_frame();
  static bool color, first=true;
  uint8_t goal_pos_y;

  if(c1 > 550){
    motor_dire(1, 0, 1, 0);
    judge_stay = j_move = true;
    motor_ctrl();
    delay(400);
  }else if(c2 > 550){
    motor_dire(0, 1, 0, 1);
    judge_stay = j_move = true; 
    motor_ctrl();
    delay(400);
  }else if(c3 > 550){
    motor_dire(1, 1, 1, 1);
    judge_stay = j_move = true;
    motor_ctrl();
    delay(200);
  }
  
  if(first){
    if(frame->blue_goal_pos->y > frame->yellow_goal_pos->y)  color = true;
    else                                                     color = false; //false::blue=back
    first = false;
  }
  if(color){
    if(frame->yellow_goal_pos != NULL){
      goal_pos_y = frame->yellow_goal_pos->y;
      if(goal_pos_y > 103){
        motor_dire(0, 0, 0, 0);
        motor_ctrl();
        judge_stay = j_move = true;
        Serial.println("Too long");
      } else if(goal_pos_y < 92){
        motor_dire(1, 1, 1, 1);
        motor_ctrl();
        judge_stay = j_move = true;
      }
      Serial.println(String(goal_pos_y));
    }
  } else {
    if(frame->blue_goal_pos != NULL){
      goal_pos_y = frame->blue_goal_pos->y;
      if(goal_pos_y > 103){
        motor_dire(0, 0, 0, 0);
        motor_ctrl();
        judge_stay = j_move = true;
      } else if(goal_pos_y < 92){
        motor_dire(1, 1, 1, 1);
        motor_ctrl();
        judge_stay = j_move = true;
      }
    }
  }
  if(frame != NULL)  delete frame;
  
  if(judge_stay == true && j_move == false){
    stop_motor();
    motor_ctrl();
    judge_stay = false;
  }
}

void setup() {

  Serial.begin(9600);
  openmv = new OpenMV(0x12);
  
  Wire.begin();
  lcd.init();
  lcd.backlight();
  openmv->setup();
  mySer.begin(19200);

  bool started = bno.begin();
  if (!started) {
    print_lcd("could not connect to bno", 1);
  }
  bno.setExtCrystalUse(true);

  for(uint8_t i=0; i<3; i++){
    pinMode(color_pin[i], INPUT);
  }
}

void loop() {
  compass();
  if (!judge_bno) {
    stay();
    if (!judge_stay) {
      Line();
    }
  }
}
