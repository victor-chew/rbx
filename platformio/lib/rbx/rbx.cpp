/*
 * rbx.cpp
 *
 * Copyright 2020 Victor Chew
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ArduinoOTA.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <Dictionary.h> 
#include "rbx.h"
#include <MPU6050_6Axis_MotionApps20.h>

#define SG90_MIN_PULSE_WIDTH 500		// 0.4ms
#define SG90_MAX_PULSE_WIDTH 2300		// 2.4ms
#define SG90_CHANNEL_FREQ 50 				// channel frequency is 50Hz 
#define SG90_CHANNEL_WIDTH 16 			// channel resolution is 16-bit 

static int Pins[11][2] = {
	{ 26,  0 },	// PORT_2A
	{ 27,  0 }, // PORT_2B
	{ 32,  0 }, // PORT_2C
	{ 33,  0 },	// PORT_3A
	{ 34,  0 }, // PORT_3B
	{ 36,  0 }, // PORT_3C
	{ 39,  0 }, // PORT_3D
	{ 13, 14 }, // PORT_4A; PWM channel1 = 7; PWM channel2 = 12
	{ 16, 17 }, // PORT_4B; PWM channel1 = 8; PWM channel2 = 13
	{ 18, 19 }, // PORT_4C; PWM channel1 = 9; PWM channel2 = 14
	{ 23, 25 }, // PORT_4D; PWM channel1 = 10; PWM channel2 = 15
};

////////////////////////////////////////////////////////////////
// OTA
////////////////////////////////////////////////////////////////

static TaskHandle_t hUpdateTask;
static AsyncWebServer server(80);
static DNSServer dns;
AsyncWiFiManager wifimgr(&server, &dns);

static void OTA_UpdateTask(void* param) {
	while(true) {
		ArduinoOTA.handle();
		delay(1000);
	}
}

// Source: https://github.com/espressif/arduino-esp32/issues/400
// Use to reset wifi credentials
static void wifiResetSettings(bool reset) {
  if (reset) {
    WiFi.disconnect(true);
    WiFi.begin("0","0");
    ESP.restart();
    delay(1000);
  }
}

void OTA_Init(const char* name) {
  wifiResetSettings(false); // Change to true to reset wifi credentials
  wifimgr.autoConnect(name);
	ArduinoOTA.begin();
	xTaskCreatePinnedToCore(OTA_UpdateTask, "otaTask", 2048, NULL, 1, &hUpdateTask, 0);
}

////////////////////////////////////////////////////////////////
// LED
////////////////////////////////////////////////////////////////

Led::Led(int port) {
	if (port < PORT_2A || port > PORT_2C) port = PORT_2A;
	_pinOut = Pins[port][0];
	pinMode(_pinOut, OUTPUT);
}

void Led::setState(bool state) {
	digitalWrite(_pinOut, state ? HIGH : LOW);
}

////////////////////////////////////////////////////////////////
// PUSHBUTTON
////////////////////////////////////////////////////////////////

Pushbutton::Pushbutton(int port) {
	if (port < PORT_3A || port > PORT_3D) port = PORT_3A;
	_pinIn = Pins[port][0];
	_clickState = false;
	_lastState = false;
	_lastTime = 0;
	pinMode(_pinIn, INPUT_PULLUP);
}

bool Pushbutton::on() {
	// Performs debouncing by returning the same state within 10ms of function call
	bool state = getState();
	unsigned long now = millis();
	if (_lastTime == 0) {
		_lastState = state;
		_lastTime = now;
		return state;
	}
	else if (now - _lastTime < 10) return _lastState;
	_lastTime = 0;
	return state;
}

bool Pushbutton::clicked() {
	if (_clickState == false) {
		if (on()) _clickState = true;
	} else {
		if (off()) {
			_clickState = false;
			return true;
		}
	}
	return false;
}

bool Pushbutton::getState() {
	return (digitalRead(_pinIn) == LOW);
}

////////////////////////////////////////////////////////////////
// Rotary Switch
////////////////////////////////////////////////////////////////

RotarySwitch* RotarySwitch::_instances[5] = { 0 };
void (*RotarySwitch::_phaseChangeISRs[5])() = { ISR_PhaseChange4A, ISR_PhaseChange4B, ISR_PhaseChange4C, ISR_PhaseChange4D, ISR_PhaseChange4E};

// Adapted from: 
// https://github.com/pcbreflux/espressif/blob/master/esp32/arduino/sketchbook/ESP32_RotaryEncoderOne/ESP32_RotaryEncoderOne.ino
void RotarySwitch::onPhaseChange() {
	portENTER_CRITICAL_ISR(&_mux);
	uint8_t p = _phase & 3;
	if (digitalRead(_dt)) p |= 4;
	if (digitalRead(_clk)) p |= 8;
	switch(p) {
		case 0: case 5: case 10: case 15:
			break;
		case 1: case 7: case 8: case 14:
			_rotVal++; break;
		case 2: case 4: case 11: case 13:
			_rotVal--; break;
		case 3: case 12:
			_rotVal += 2; break;
		default:
			_rotVal -= 2; break;
	}
	_phase = (p >> 2);
	portEXIT_CRITICAL_ISR(&_mux);
}

RotarySwitch::RotarySwitch(int port2, int port4, int stepsPerNotch) : Pushbutton(0) {
	if (port2 < PORT_2A || port2 > PORT_2C) port2 = PORT_2A;
	if (port4 < PORT_4A || port4 > PORT_4D) port4 = PORT_4A;
	_phase = 0;
	_rotVal = _lastValue = _swVal = 0;
	_sw = Pins[port2][0];
	_clk = Pins[port4][0];
	_dt = Pins[port4][1];
	_steps = stepsPerNotch;
	_mux = portMUX_INITIALIZER_UNLOCKED;
	pinMode(_sw, INPUT_PULLUP);
	pinMode(_clk, INPUT_PULLUP);
	pinMode(_dt, INPUT_PULLUP);
	int instNum = port4 - PORT_4A;
	_instances[instNum] = this;
	attachInterrupt(_clk, _phaseChangeISRs[instNum], CHANGE);
	attachInterrupt(_dt, _phaseChangeISRs[instNum], CHANGE);
}

RotarySwitch::~RotarySwitch() {
	detachInterrupt(_clk);
	detachInterrupt(_dt);
}

int RotarySwitch::getDelta() {
	long value = _rotVal / _steps;
	long delta = value - _lastValue;
	_lastValue = value;
	return delta;
}

bool RotarySwitch::getState() {
	return digitalRead(_sw) == LOW;
}

////////////////////////////////////////////////////////////////
// OLED display (128 x 64)
////////////////////////////////////////////////////////////////

OledDisplay::OledDisplay() {
	_buffer = false;
	_sda = 21;
	_scl = 22;
	_u8g2 = new U8G2_SSD1306_128X64_NONAME_F_SW_I2C(U8G2_R0, _scl, _sda, U8X8_PIN_NONE);
	_u8g2->begin();
	setColor(COLOR_WHITE);
	setFont(FONT_NORMAL);
}

void OledDisplay::startBuffer() {
	_buffer = true;
}

void OledDisplay::flushBuffer() {
	_u8g2->sendBuffer();
	_buffer = false;
}

void OledDisplay::clear() {
	_u8g2->clearBuffer();
	if (!_buffer) _u8g2->sendBuffer();
}

void OledDisplay::setColor(int color) {
	_u8g2->setDrawColor(color);
}

void OledDisplay::setFont(int font) {
	switch(font) {
		case FONT_SMALL: _u8g2->setFont(u8g2_font_5x7_tf); break;
		case FONT_NORMAL: _u8g2->setFont(u8g2_font_pxplusibmcgathin_8f); break;
		case FONT_NORMAL_BOLD: _u8g2->setFont(u8g2_font_pxplusibmcga_8f); break;
		case FONT_LARGE: _u8g2->setFont(u8g2_font_pxplusibmvga8_tf); break;
		case FONT_SYMBOL: _u8g2->setFont(u8g2_font_unifont_t_symbols); break;
	}
}

void OledDisplay::drawPixel(int x, int y) {
	_u8g2->drawPixel(x, y);
	if (!_buffer) _u8g2->sendBuffer();
}

void OledDisplay::drawLine(int x1, int y1, int x2, int y2) {
	_u8g2->drawLine(x1, y1, x2, y2);
	if (!_buffer) _u8g2->sendBuffer();
}

void OledDisplay::drawRect(int x1, int y1, int x2, int y2, bool filled) {
	if (!filled) 
		_u8g2->drawFrame(x1, y1, x2, y2); 
	else 
		_u8g2->drawBox(x1, y1, x2, y2);
	if (!_buffer) _u8g2->sendBuffer();
}

void OledDisplay::drawEllipse(int x, int y, int rx, int ry, bool filled) {
	if (!filled) 
		_u8g2->drawEllipse(x, y, rx, ry, U8G2_DRAW_ALL);
	else 
		_u8g2->drawFilledEllipse(x, y, rx, ry, U8G2_DRAW_ALL);
	if (!_buffer) _u8g2->sendBuffer();
}

int OledDisplay::stringWidth(const char* text) {
	return _u8g2->getUTF8Width(text);
}

int OledDisplay::stringHeight(const char* text) {
	return _u8g2->getMaxCharHeight();
}

void OledDisplay::drawString(int x, int y, const char* str) {
	_u8g2->drawUTF8(x, y, str);
	if (!_buffer) _u8g2->sendBuffer();
}

////////////////////////////////////////////////////////////////
// BUZZER
////////////////////////////////////////////////////////////////

Buzzer::Buzzer(int port) {
	if (port < PORT_2A || port > PORT_2C) port = PORT_2A;
	_pinOut = Pins[port][0];
	_channel = port;
	ledcSetup(_channel, 2000, 8);  
	ledcAttachPin(_pinOut, _channel);
}

void Buzzer::playTone(int freq, int duration) {
	ledcWriteTone(_channel, freq);
	if (duration > 0) {
		delay(duration);
		ledcWrite(_channel, 0);
	}
}

void Buzzer::playNote(int note, int octave, int duration) {
	ledcWriteNote(_channel, (note_t)note, octave);
	if (duration > 0) {
		delay(duration);
		ledcWrite(_channel, 0);
	}
}

void Buzzer::stop() {
	ledcWrite(_channel, 0);
}

////////////////////////////////////////////////////////////////
// PHOTORESISTOR
////////////////////////////////////////////////////////////////

Photoresistor::Photoresistor(int port) {
	if (port < PORT_3A || port > PORT_3D) port = PORT_3A;
	_pinIn = Pins[port][0];
}

float Photoresistor::getValue() {
	return analogRead(_pinIn) / 4095.0;
}

////////////////////////////////////////////////////////////////
// MOTOR
////////////////////////////////////////////////////////////////

Motor::Motor(int port) {
	if (port < PORT_4A || port > PORT_4D) port = PORT_4A;
	_running = false;
	_reverse = false;
	_speed = 1.0;
	_pinOut1 = Pins[port][0];
	_pinOut2 = Pins[port][1];
	_channel1 = port;
	_channel2 = port + 5;
	ledcSetup(_channel1, 2000, 8);
	ledcSetup(_channel2, 2000, 8);
	ledcWrite(_channel1, 0);
	ledcWrite(_channel2, 0);
	ledcAttachPin(_pinOut1, _channel1);
	ledcAttachPin(_pinOut2, _channel2);
}

void Motor::setSpeed(float speed) {
	_speed = speed;
	if (_speed < 0.0) _speed = 0.0;
	if (_speed > 1.0) _speed = 1.0;
	if (speed == 0.0) _dutyCycle = 0; else _dutyCycle = 160 + (_speed * (255-160));
	if (_running) start();
}

void Motor::setDir(int dir) {
	_reverse = dir == DIR_REV;
	if (_running) start();
}

void Motor::start() {
	_running = true;
	if (!_reverse) {
		ledcWrite(_channel2, 0);
		ledcWrite(_channel1, _dutyCycle);
	} else {
		ledcWrite(_channel1, 0);
		ledcWrite(_channel2, _dutyCycle);
	}
}

void Motor::stop() {
	_running = false;
	ledcWrite(_channel1, 0);
	ledcWrite(_channel2, 0);
}

////////////////////////////////////////////////////////////////
// SERVO
////////////////////////////////////////////////////////////////

Servo::Servo(int port, int degree) {
	if (port < PORT_2A || port > PORT_2C) port = PORT_2A;
	_pinOut = Pins[port][0];
	_channel = port;
	ledcSetup(_channel, SG90_CHANNEL_FREQ, SG90_CHANNEL_WIDTH);
	ledcAttachPin(_pinOut, _channel);
	setPos(degree);
}

void Servo::setPos(int degree) {
	degree = max(0, min(degree, 180));
	int usec = map(degree, 180, 0, SG90_MIN_PULSE_WIDTH, SG90_MAX_PULSE_WIDTH);
	int ticks = (int)(usec / (1000000.0/SG90_CHANNEL_FREQ) * pow(2,SG90_CHANNEL_WIDTH));
	ledcWrite(_channel, ticks);
	_degree = degree;
}

////////////////////////////////////////////////////////////////
// ULTRASONIC SENSOR
////////////////////////////////////////////////////////////////

Sonar::Sonar(int port) {
	if (port < PORT_4A || port > PORT_4D) port = PORT_4A;
	_pinTrig = Pins[port][0];
	_pinEcho = Pins[port][1];
	_sonar = new NewPing(_pinTrig, _pinEcho);
}

int Sonar::getDistance() {
	return _sonar->convert_cm(_sonar->ping_median());
}

////////////////////////////////////////////////////////////////
// COLOR SENSOR
////////////////////////////////////////////////////////////////

ColorSensor* ColorSensor::_instances[3] = { 0 };
void (*ColorSensor::_phaseChangeISRs[3])() = { ISR_PhaseChange2A, ISR_PhaseChange2B, ISR_PhaseChange2C };

void ColorSensor::onPhaseChange() {
  static unsigned long time0 = 0;
  noInterrupts();
  if (digitalRead(_pinOut) == HIGH) time0 = micros();
  else _pulseWidth = micros() - time0;
  interrupts();
}

ColorSensor::ColorSensor(int port2, int port4) {
	if (port2 < PORT_2A || port2 > PORT_2C) port2 = PORT_2A;
	if (port4 < PORT_4A || port4 > PORT_4D) port4 = PORT_4A;
  _pinS2 = Pins[port4][0];
  _pinS3 = Pins[port4][1];
  _pinOut = Pins[port2][0];
  pinMode(_pinS2, OUTPUT);
  pinMode(_pinS3, OUTPUT);
  pinMode(_pinOut, INPUT_PULLUP);
  for (int i=0; i<COLOR_TABLE_SIZE; i++) red[i] = green[i] = blue[i] = 0.0;
	_instNum = port2 - PORT_2A;
	_instances[_instNum] = this;
}

void ColorSensor::getResponse(float& r, float& g, float &b) {
  attachInterrupt(_pinOut, _phaseChangeISRs[_instNum], CHANGE);
  digitalWrite(_pinS2, LOW);
  digitalWrite(_pinS3, LOW);
  delay(5);
  r = (500000.0 / _pulseWidth) / 19000;
  digitalWrite(_pinS2, HIGH);
  digitalWrite(_pinS3, HIGH);
  delay(5);
  g = (500000.0 / _pulseWidth) / 13600;
  digitalWrite(_pinS2, LOW);
  digitalWrite(_pinS3, HIGH);
  delay(5);
  b = (500000.0 / _pulseWidth) / 16400;
  detachInterrupt(_pinOut);
}

void ColorSensor::setColor(int index) {
  index = min(index, COLOR_TABLE_SIZE - 1);
  getResponse(red[index], green[index], blue[index]);
}

int ColorSensor::getColor() {
  float r, g, b;
  getResponse(r, g, b);
  int result = 0;
  float min_dist = 999999.0;
  for (int i=0; i<COLOR_TABLE_SIZE; i++) {
    float dist = sqrt((red[i]-r)*(red[i]-r) + (green[i]-g)*(green[i]-g) + (blue[i]-b)*(blue[i]-b));
    if (dist < min_dist) {
      min_dist = dist;
      result = i;
    }
  }
  return result;
}

void ColorSensor::loadFromKeystore(const char* prefix) {
  Keystore* ks = Keystore::getInstance();
  for (int i=0; i<COLOR_TABLE_SIZE; i++) {
    String ri = prefix; ri.concat("r"); ri.concat(i); if (ks->exists(ri.c_str())) red[i] = ks->getValue(ri.c_str()).toFloat();
    String gi = prefix; gi.concat("g"); gi.concat(i); if (ks->exists(gi.c_str())) green[i] = ks->getValue(gi.c_str()).toFloat();
    String bi = prefix; gi.concat("b"); bi.concat(i); if (ks->exists(bi.c_str())) blue[i] = ks->getValue(bi.c_str()).toFloat();
  }
}

void ColorSensor::saveToKeystore(const char* prefix) {
  Keystore* ks = Keystore::getInstance();
  for (int i=0; i<COLOR_TABLE_SIZE; i++) {
    String ri = prefix; ri.concat("r"); ri.concat(i); ks->setValue(ri.c_str(), (new String(red[i]))->c_str());
    String gi = prefix; gi.concat("g"); gi.concat(i); ks->setValue(gi.c_str(), (new String(green[i]))->c_str());
    String bi = prefix; bi.concat("b"); bi.concat(i); ks->setValue(bi.c_str(), (new String(blue[i]))->c_str());
  }
}

////////////////////////////////////////////////////////////////
// MOTION SENSOR
////////////////////////////////////////////////////////////////

MotionSensor::MotionSensor() {
  _mpu = new MPU6050();
  _mpu->initialize();
  if (_mpu->dmpInitialize() == 0) {
    _mpu->setDMPEnabled(true);
    _packetSize = _mpu->dmpGetFIFOPacketSize();
  }
  memset(_reading, 0, sizeof(_reading));
  memset(_mean, 0, sizeof(_mean));
  memset(_offset, 0, sizeof(_offset));
  memset(_ypr, 0, sizeof(_ypr));
	_q = new Quaternion();
	_aa = new VectorInt16();
  _aaReal = new VectorInt16();
  _aaWorld = new VectorInt16();
	_gravity = new VectorFloat();
}

MotionSensor::~MotionSensor() {
  delete _mpu;
  delete _q;
  delete _aa;
  delete _aaReal;
  delete _aaWorld;
  delete _gravity;
}

void MotionSensor::refresh() {
  _mpu->resetFIFO();
  while ((_fifoCount = _mpu->getFIFOCount()) < _packetSize);
  _mpu->getFIFOBytes(_fifoBuffer, _packetSize);
  _mpu->dmpGetQuaternion(_q, _fifoBuffer);
  _mpu->dmpGetAccel(_aa, _fifoBuffer);
  _mpu->dmpGetGravity(_gravity, _q);
  _mpu->dmpGetYawPitchRoll(_ypr, _q, _gravity);
  _mpu->dmpGetLinearAccel(_aaReal, _aa, _gravity);
  _mpu->dmpGetLinearAccelInWorld(_aaWorld, _aaReal, _q);
}

float MotionSensor::getRoll() {
  return _ypr[2] * 180/M_PI;
}

float MotionSensor::getPitch() {
  return _ypr[1] * 180/M_PI;
}

float MotionSensor::getYaw() {
  return _ypr[0] * 180/M_PI;
}

float MotionSensor::getAccelX() {
  return _aaWorld->x;
}

float MotionSensor::getAccelY() {
  return _aaWorld->y;
}

float MotionSensor::getAccelZ() {
  return _aaWorld->z;
}

void MotionSensor::calibrateInit() {
	readMean(); 
	_offset[0] = -_mean[0] / 8;
	_offset[1] = -_mean[1] / 8;
  _offset[2] = (16384 - _mean[2]) / 8;
	for (int x=3; x<6; x++) _offset[x] = -_mean[x] / 4;
}

// Ref: https://mjwhite8119.github.io/Robots/mpu6050
bool MotionSensor::calibrate() {
	int ready = 0;
  writeOffset();
	readMean();
	if (abs(_mean[0]) <= ACEL_DEADZONE) ready++; else _offset[0] -= _mean[0] / ACEL_DEADZONE;
	if (abs(_mean[1]) <= ACEL_DEADZONE) ready++; else _offset[1] -= _mean[1] / ACEL_DEADZONE;
	if (abs(16384 - _mean[2]) <= ACEL_DEADZONE) ready++; else _offset[2] += (16384 - _mean[2]) / ACEL_DEADZONE;
  for (int x=3; x<6; x++) {
		if (abs(_mean[x]) <= GYRO_DEADZONE) ready++; else _offset[x] -= _mean[x] / (GYRO_DEADZONE + 1);
	}
	return ready == 6;
}

void MotionSensor::loadFromKeystore(const char* prefix) {
  Keystore* ks = Keystore::getInstance();
  for (int x=0; x<6; x++) {
    String key = prefix; key.concat("off"); key.concat(x);
    if (ks->exists(key.c_str())) _offset[x] = ks->getValue(key.c_str()).toFloat();
  }
  writeOffset();
}

void MotionSensor::saveToKeystore(const char* prefix) {
  Keystore* ks = Keystore::getInstance();
  for (int x=0; x<6; x++) {
    String key = prefix; key.concat("off"); key.concat(x);
    ks->setValue(key.c_str(), (new String(_offset[x]))->c_str());
  }
}

void MotionSensor::read() {
	_reading[0] = _mpu->getAccelerationX();
	_reading[1] = _mpu->getAccelerationY();
	_reading[2] = _mpu->getAccelerationZ();
	_reading[3] = _mpu->getRotationX();
	_reading[4] = _mpu->getRotationY();
	_reading[5] = _mpu->getRotationZ();
}

void MotionSensor::readMean() {
	long sum[6] = { 0 };
	// Discard first 100 readings
	for (int i=0; i<100; i++) {
		read();
		delay(2);
	}
  for (int i=0; i<1000; i++) {
		read();
		for (int x=0; x<6; x++) sum[x] += _reading[x];
		delay(2);
	}
	for (int x=0; x<6; x++) _mean[x] = sum[x] / 1000;
}

void MotionSensor::writeOffset() {
	_mpu->setXAccelOffset(_offset[0]);
	_mpu->setYAccelOffset(_offset[1]);
	_mpu->setZAccelOffset(_offset[2]);
	_mpu->setXGyroOffset(_offset[3]);
	_mpu->setYGyroOffset(_offset[4]);
	_mpu->setZGyroOffset(_offset[5]);
}

////////////////////////////////////////////////////////////////
// KEYSTORE
////////////////////////////////////////////////////////////////

#define KEYFILE "/rbx/.keystore"

Keystore* Keystore::_instance = NULL;

Keystore* Keystore::getInstance() {
  if (_instance == NULL) _instance = new Keystore();
  return _instance;
}

Keystore::Keystore() {
  _dict = new Dictionary();
}

void Keystore::load() {
  if (!SPIFFS.exists(KEYFILE)) return;
  File file = SPIFFS.open(KEYFILE, FILE_READ);
  String content; while(file.available()) content += (char)file.read();
  file.close();
  _dict->jload(content);
}

void Keystore::save() {
  File file = SPIFFS.open(KEYFILE, FILE_WRITE);
  file.write((const unsigned char*)_dict->json().c_str(), _dict->jsize());
  file.close();
}

int Keystore::count() {
  return _dict->count();
}

bool Keystore::exists(const char* key) {
  return (*_dict)(key);
}

String Keystore::getValue(const char* key) {
  return (*_dict)[key];
}

String Keystore::getKeyByIndex(int index) {
  return _dict->key(index);
}

String Keystore::getValueByIndex(int index) {
  return _dict->value(index);
}

void Keystore::setValue(const char* key, const char* value) {
  (*_dict)(key, value);
}

void Keystore::remove(const char* key) {
  _dict->remove(key);
}

void Keystore::clear() {
  delete _dict;
  _dict = new Dictionary();
}