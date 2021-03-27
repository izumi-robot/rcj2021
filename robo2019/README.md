<!-- 必ずVSCodeで開き、Ctrl+Shift+Vでプレビューを開くこと -->

# robo2019

ロボカップ用に作ったArduinoライブラリです。前提条件として、機体のモーターの位置、番号、向き、そしてロボットの中心を原点とした相対座標系について、極形式の角度は次の写真のようになっているものとします。

![machine-info.jpeg](./img/machine-info.jpeg)

`#include <robo2019.h>`でインクルードしてください。以下で紹介するものはすべて`robo`ネームスペースに格納されます。

## util.h

ライブラリ全体で共有するユーティリティを集めました。

`namespace string`

文字列関連の機能群です。Pythonの`str`クラスに似せて作りました。

`String string::rjust(const String &target, uint8_t width, const char &padding)`

`target`に与えられた文字列を長さ`width`に右寄せした文字列を返します。余白には`padding`に与えられた文字が使われます。`padding`が`char`型であることに注意してください。
例:

```C++
String s = "Hello"
s = robo::string::rjust(s, 10, '+');
// s == "+++++Hello"
```

## vec2d.h

* `Vector2D<T>`

自作の2次元ベクトル型です。テンプレート引数の`T`は算術型(`int`, `float`など)である必要があります。モータ制御、pixyのボール情報などでこの型を使用しています。

**型エイリアス**

以下のエイリアスが定義されています。

* `using V2_float = Vector2D<float>;`
* `using V2_int = Vector2D<int>;`

**プロパティ**

* `T x, y`

ベクトルのx座標、y座標です。

**コンストラクター**

* `Vector2D<T>::Vector2D()`

プロパティの`x`と`y`を`0`で初期化します。

* `Vector2D<T>::Vector2D(const T &x, const T &y)`

プロパティの`x`と`y`を、引数の`x`と`y`でそれぞれ初期化します。
例:

```C++
robo::V2_int v(2, 4);
// v.x == 2, v.y == 4
```

* `Vector2D<T>::Vector2D(const Vector2D<T> &p)`

引数のベクトル`p`をコピーして初期化します。

* `Vector2D<T>::Vector2D(const T (&p)[2])`

プロパティの`x`と`y`を、`p[0]`と`p[1]`でそれぞれ初期化します。
例:

```C++
int ls[] = {2, 4};
robo::V2_int v(ls);
// v.x == 2, v.y == 4
```

* `Vector2D<T>::Vector2D(std::initializer_list<T> init)`

初期化子リストです。

```C++
robo::V2_int v{2, 4};
// v.x == 2, v.y == 4
```

**演算子の実装**

`Vector2D`が実装している演算子を紹介します。

* `Vector2D<T>& Vector2D<T>::operator=(const Vector2D<T> &tmp)`

コピー代入です。
例:

```C++
robo::V2_int v1(2, 4), v2(3, 5);
v1 = v2;
// v1.x == 3, v1.y == 5
```

* `Vector2D<T>& Vector2D<T>::operator=(const T (&tmp)[2])`

`x`に`tmp[0]`を、`y`に`tmp[1]`を代入します。
例:

```C++
robo::V2_int v;
int ls[] = {2, 4};
v = ls;
// v.x = 2, v.y = 4
```

* `const T& Vector2D<T>::operator[](size_t index) const`
* `T& Vector2D<T>::operator[](size_t index)`

添字の実装です。`0`であれば`x`、`1`であれば`y`の値を返します。
例:


```C++
robo::V2_int v{2, 4};
v[0] = 3;
v[1] = 5;
// v.x == 2, v.y == 4
```

* `Vector2D<T> operator+(const Vector2D<T> &lh, const Vector2D<T> &rh)`
* `Vector2D<T> operator+(const Vector2D<T> &lh, const T (&rh)[2])`
* `Vector2D<T>& operator+=(Vector2D<T> &lh, const Vector2D<T> &rh)`
* `Vector2D<T>& operator+=(Vector2D<T> &lh, const T (&rh)[2])`

加算の実装です。
例:

```C++
robo::V2_int v1{2, 4}, v2{3, 5};
int ls[] = {6, 1};
robo::V2_int v3 = v1 + ls;
// v3.x == 8, v3.y == 5
v2 += v1;
// v2.x == 5, v2.y == 9
```

* `Vector2D<T> operator-(const Vector2D<T> &lh, const Vector2D<T> &rh)`
* `Vector2D<T> operator-(const Vector2D<T> &lh, const T (&rh)[2])`
* `Vector2D<T>& operator-=(Vector2D<T> &lh, const Vector2D<T> &rh)`
* `Vector2D<T>& operator-=(Vector2D<T> &lh, const T (&rh)[2])`

減算の実装です。使い方は加算と同じです。

* `Vector2D<T> operator*(const Vector2D<T> &lh, const Vector2D<T> &rh)`
* `Vector2D<T> operator*(const Vector2D<T> &lh, const T (&rh)[2])`
* `Vector2D<T> operator*(const Vector2D<T> &lh, const T &rh)`
* `Vector2D<T>& operator*=(Vector2D<T> &lh, const Vector2D<T> &rh)`
* `Vector2D<T>& operator*=(Vector2D<T> &lh, const T (&rh)[2])`
* `Vector2D<T>& operator*=(Vector2D<T> &lh, const T &rh)`

乗算の実装です。内積ではないので注意してください。ベクトル`(a, b)`とベクトル`(x, y)`の乗算はベクトル`(a*x, b*y)`を返します。
例:

```C++
robo::V2_int v1{2, 4}, v2{3, 5};
int ls[] = {6, 1};
robo::V2_int v3 = v1 * ls;
// v3.x == 12, v3.y == 4
v2 *= 5;
// v2.x == 30, v2.y == 5
```

* `Vector2D<T> operator/(const Vector2D<T> &lh, const Vector2D<T> &rh)`
* `Vector2D<T> operator/(const Vector2D<T> &lh, const T (&rh)[2])`
* `Vector2D<T> operator/(const Vector2D<T> &lh, const T &rh)`
* `Vector2D<T>& operator/=(Vector2D<T> &lh, const Vector2D<T> &rh)`
* `Vector2D<T>& operator/=(Vector2D<T> &lh, const T (&rh)[2])`
* `Vector2D<T>& operator/=(Vector2D<T> &lh, const T &rh)`

除算の実装です。ベクトル`(a, b)`とベクトル`(x, y)`の除算はベクトル`(a/x, b/y)`を返します。
例: (小数の桁はテキトーです。)

```C++
robo::V2_float v1{2.0, 4.0}, v2{3.0, 5.0};
int ls[] = {6.0, 1.0};
robo::V2_float v3 = v1 / ls;
// v3.x == 0.3333333, v3.y == 4.0
v2 /= 5;
// v2.x == 0.6, v2.y == 1.0
```

**メンバー関数**

* `String Vector2D<T>::to_string() const`

ベクトルの簡易的な文字列表現を返します。
例:

```C++
robo::V2_int v{2, 4};
String s = v.to_string();
// s == "(2, 4)"
```

* `T Vector2D<T>::dot(const Vector2D<T> &v) const`
* `T Vector2D<T>::dot(const T (&tmp)[2]) const`
* `T Vector2D<T>::dot(const T &x, const T &y) const`

ベクトルの内積を計算します。
例:

```C++
robo::V2_int v{2, 4};
int ls[] = {5, 3};
int d1 = v.dot(ls), d2 = v.dot(3, 5);
// d1 == 22, d2 == 26
```

* `float Vector2D<T>::angle() const`

ベクトルの角度を計算します。原点`(0, 0)`から`(1, 0)`の向きが`0`、`(0, 1)`の向きが`PI / 2`、`(0, -1)`の向きが`-PI / 2`です。
例:

```C++
robo::V2_int v{1, 1};
float ang = v.angle();
// ang == PI / 4
```

* `float Vector2D<T>::mag() const`

ベクトルの大きさを計算します。
例: `robo::V2_int{1, 1} == sqrt(2)`

```C++
robo::V2_int v{1, 1};
float mag = v.mag();
// mag == sqrt(2)
```

**静的関数**

* `static Vector2D<T> Vector2D<T>::from_polar_coord(const float &angle, const T &magnitude)`

極座標のパラメーターからベクトルを作成します。第一引数に角度(ラジアン)、第二引数に大きさを渡してください。
例:

```C++
robo::V2_float v = robo::V2_float::from_polar_coord(PI / 3, 10);
// v.x == 5.0, v.y == 5.0 * sqrt(3.0)
```

## motor.h

モーター制御用の機能群です。モーターはダイセンのMCB対応です。Serial1を通して通信します。モーターのピン番号と位置は次の図のようになります。

![motor-info.jpeg](./img/motor-info.jpeg) <!-- TODO -->

このヘッダーに含まれているのは`Motor`クラスとそのインスタンス`motor`だけです。`Motor`クラスはシングルトンとして提供しています。インスタンスは`robo::Motor::instance()`か`robo::motor`でアクセスしてください。

**関数群**

* `void Motor::stop()`

停止します。

* `void Motor::setup()`

モーターを初期化します。モーターを使用する場合はスケッチ本体の`setup()`関数内で必ず呼んでください。

**`set`オブジェクト**

モーターのパワー設定用の関数群です。呼び出す際には`robo::motor.set.one_motor(1, 100)`などと書く必要があります。

* `void one_motor(uint8_t pin, int8_t power)`

ピン番号`pin`のモーターのパワーを`power`に設定します。

* `void Motor::all_motors(int8_t a, int8_t b, int8_t c, int8_t d)`

モーター4つのパワーを設定します。

* `void velocity(const float &vx, const float &vy)`
* `void velocity(const robo::V2_float &vel)`

機体が、与えられた速度ベクトルで平行移動するようにパワーを設定します。

* `void left_right(int8_t left, int8_t right)`

左側についている車輪、右側についている車輪でパワーを設定します。`all_motors(left, right, left, right)`のショートカットです。

* `void direction_and_speed(const float &direction, int8_t speed)`

与えられた方向`direction`と速さ`speed`で平行移動するようにパワーを設定します。

* `void rotate(bool clockwise, int8_t vel=100)`

`clockwise`が`true`であれば時計回りに、`false`であれば反時計回りに速さ`speed`で回転するようにパワーを設定します。

**`get`オブジェクト**

パワー情報を取得するための関数群です。呼び出す際には`robo::motor.get.one_motor(1, 100)`などと書く必要があります。

* `int8_t one_motor(uint8_t pin) const`

ピン番号`pin`のモーターのパワーを取得します。

* `String power_str(uint8_t pin) const`

ピン番号`pin`のモーターのパワーを取得し、シリアル通信でMCBに渡す文字列を返します。

* `String info() const`

モーターのパワー情報をまとめて文字列で返します。デバッグ用です。

**静的関数**

* `static Motor& Motor::instance()`

シングルトンのインスタンスを返します。

* `static String Motor::power_str(int pin, int8_t power)`

ピン番号`pin`のモーターのパワーを`power`に設定する際の文字列を返します。例: `robo::Motor::power_str(1, 50)`は`"1R050"`を返します。

## interrupt.h

割り込み用のクラス定義です。

こちらもシングルトンですが、テンプレートパラメーターとして割り込み監視用の

<!--
    TODO
    bno055
    dsrlcd
    echo
    interrupt
    pixy
    openmv
-->
