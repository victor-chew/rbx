#ifndef RBX_H
#define RBX_H

#include <NewPing.h>
#include <U8g2lib.h>

enum Port { PORT_2A, PORT_2B, PORT_2C, PORT_3A, PORT_3B, PORT_3C, PORT_3D, PORT_4A, PORT_4B, PORT_4C, PORT_4D };
enum Font { FONT_SMALL, FONT_NORMAL, FONT_NORMAL_BOLD, FONT_LARGE, FONT_SYMBOL };
enum Color { COLOR_BLACK, COLOR_WHITE, COLOR_INVERT };
enum Direction { DIR_FWD, DIR_REV };
// Commented enum replicates the one defined in ESP32 IDK to help parse_rbx.py generate the corresponding Javascript constants
// enum Note { NOTE_C, NOTE_Cs, NOTE_D, NOTE_Eb, NOTE_E, NOTE_F, NOTE_Fs, NOTE_G, NOTE_Gs, NOTE_A, NOTE_Bb, NOTE_B };

////////////////////////////////////////////////////////////////
// OTA
////////////////////////////////////////////////////////////////

void OTA_Init(const char* name = "RBX");

////////////////////////////////////////////////////////////////
// LED
////////////////////////////////////////////////////////////////

class Led {
	public:
		Led(int port);
		void setState(bool state);
	private:
		int _pinOut;
};

////////////////////////////////////////////////////////////////
// OLED display (128 x 64)
////////////////////////////////////////////////////////////////

class OledDisplay {
	public:
		OledDisplay();
		~OledDisplay() { delete _u8g2; }
		void startBuffer();
		void flushBuffer();
		void clear();
		void setColor(int color);
		void setFont(int font);
		int width() { return 128; }
		int height() { return 64; }
		int stringWidth(const char* str);
		int stringHeight(const char* str);
		void drawPixel(int x, int y);
		void drawLine(int x1, int y1, int x2, int y2);
		void drawRect(int x1, int y1, int x2, int y2, bool filled = false);
		void drawEllipse(int x, int y, int rx, int ry, bool filled = false);
		void drawString(int x, int y, const char* str);
	private:
		U8G2_SSD1306_128X64_NONAME_F_SW_I2C* _u8g2;
		int _sda, _scl;
		bool _buffer;
};

////////////////////////////////////////////////////////////////
// PUSHBUTTON
////////////////////////////////////////////////////////////////

class Pushbutton {
	public:
		Pushbutton(int port);
    virtual ~Pushbutton() {}
		bool on();
		bool off() { return !on(); }
    bool clicked();
	private:
		int _pinIn;
		bool _lastState, _clickState;
		unsigned long _lastTime;
		virtual bool getState();
};

////////////////////////////////////////////////////////////////
// Rotary Switch
////////////////////////////////////////////////////////////////

class RotarySwitch : public Pushbutton {
	public:
		RotarySwitch(int port2, int port4, int stepsPerNotch = 2);
		virtual ~RotarySwitch();
		int getDelta();
		bool on() { return Pushbutton::on(); }
    bool off() { return Pushbutton::off(); }
    bool clicked() { return Pushbutton::clicked(); }
	private:
		volatile uint8_t _phase;
		volatile int _dt, _clk, _sw, _steps;
		volatile int _rotVal, _swVal;
		portMUX_TYPE _mux;
		int _lastValue;
		void onPhaseChange();
		virtual bool getState();
		// ISR for each instance requires separate static functions
		static RotarySwitch* _instances[5];
		static void (*_phaseChangeISRs[5])();
		static void ISR_PhaseChange4A() {	_instances[0]->onPhaseChange(); }
		static void ISR_PhaseChange4B() {	_instances[1]->onPhaseChange(); }
		static void ISR_PhaseChange4C() {	_instances[2]->onPhaseChange(); }
		static void ISR_PhaseChange4D() {	_instances[3]->onPhaseChange(); }
		static void ISR_PhaseChange4E() {	_instances[4]->onPhaseChange(); }
};

////////////////////////////////////////////////////////////////
// BUZZER
////////////////////////////////////////////////////////////////

class Buzzer {
	public:
		Buzzer(int port);
		// if duration = 0 or unspecified, tone will play until stop() is called
		void playTone(int freq, int duration = 0);
		void playNote(int note, int octave, int duration = 0);
		void stop();
	private:
		int _pinOut, _channel;
};

////////////////////////////////////////////////////////////////
// PHOTORESISTOR
////////////////////////////////////////////////////////////////

class Photoresistor {
	public:
		Photoresistor(int port);
		float getValue();
	private:
		int _pinIn;
};

////////////////////////////////////////////////////////////////
// MOTOR
////////////////////////////////////////////////////////////////

class Motor {
	public:
		Motor(int port);
		void setSpeed(float speed);
		void setDir(int dir);
		void start();
		void stop();
	private:
		int _pinOut1, _pinOut2, _channel1, _channel2, _dutyCycle;
		float _speed;
		bool _running, _reverse;
};

////////////////////////////////////////////////////////////////
// SERVO
////////////////////////////////////////////////////////////////

class Servo {
	public:
		Servo(int port, int degree = 0);
		void setPos(int degree);
		int getPos() { return _degree; }
	private:
		int _pinOut, _channel, _degree;
};

////////////////////////////////////////////////////////////////
// ULTRASONIC SENSOR
////////////////////////////////////////////////////////////////

class Sonar {
	public:
		Sonar(int port);
		~Sonar() { delete _sonar; }
		int getDistance();
	private:
		int _pinTrig, _pinEcho;
		NewPing* _sonar;
};

////////////////////////////////////////////////////////////////
// COLOR SENSOR
////////////////////////////////////////////////////////////////

#define COLOR_TABLE_SIZE 16

class ColorSensor {
	public:
		ColorSensor(int port2, int port4);
    int getColor();
    void setColor(int index);
    void loadFromKeystore(const char* prefix = "");
    void saveToKeystore(const char* prefix = "");
	private:
		int _instNum, _pinS2, _pinS3, _pinOut;
    float red[COLOR_TABLE_SIZE], green[COLOR_TABLE_SIZE], blue[COLOR_TABLE_SIZE];
    volatile unsigned long _pulseWidth;
    void getResponse(float& r, float& g, float &b);
		void onPhaseChange();
		// ISR for each instance requires separate static functions
		static ColorSensor* _instances[3];
		static void (*_phaseChangeISRs[3])();
		static void ISR_PhaseChange2A() {	_instances[0]->onPhaseChange(); }
		static void ISR_PhaseChange2B() {	_instances[1]->onPhaseChange(); }
		static void ISR_PhaseChange2C() {	_instances[2]->onPhaseChange(); }
};

////////////////////////////////////////////////////////////////
// MOTION SENSOR
////////////////////////////////////////////////////////////////

#define ACEL_DEADZONE 8
#define GYRO_DEADZONE 1

class MPU6050;
class Quaternion;
class VectorInt16;
class VectorFloat;

class MotionSensor {
	public:
		MotionSensor();
		~MotionSensor();
		void calibrateInit();
		bool calibrate();
		void refresh();
		float getRoll();
		float getPitch();
		float getYaw();
		float getAccelX();
		float getAccelY();
		float getAccelZ();
		void loadFromKeystore(const char* prefix = "");
		void saveToKeystore(const char* prefix = "");
	private:
		MPU6050* _mpu;
		uint16_t _packetSize, _fifoCount;
		uint8_t _fifoBuffer[64];
		float _ypr[3];
		Quaternion* _q;
		VectorInt16 *_aa, *_aaReal, *_aaWorld;
		VectorFloat* _gravity;
		int16_t _reading[6]; // accX, accY, accZ, gyroX, gyroY, gyroZ
		int16_t _mean[6]; // accX, accY, accZ, gyroX, gyroY, gyroZ
		int16_t _offset[6]; // accX, accY, accZ, gyroX, gyroY, gyroZ
		void read();
    void readMean();
    void writeOffset();
};

////////////////////////////////////////////////////////////////
// KEYSTORE
////////////////////////////////////////////////////////////////

class Dictionary;

class Keystore {
  public:
    static Keystore* getInstance();
    void load();
    void save();
    int count();
    bool exists(const char* key);
    String getValue(const char* key);
    void setValue(const char* key, const char* value);
    String getKeyByIndex(int index);
    String getValueByIndex(int index);
    void remove(const char* key);
    void clear();
  private:
    static Keystore* _instance;
    Dictionary* _dict;
    Keystore();
};

#endif // RBX_H
