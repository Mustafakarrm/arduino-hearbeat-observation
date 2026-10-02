#include "FakePulseSensor.h"

// One heartbeat waveform, 32 samples (0..255), stored in flash to save RAM
static const uint8_t PULSE_WAVE[32] PROGMEM = {
    1,   7,  37, 115, 218, 252, 179,  79,
   22,   8,  14,  29,  51,  74,  88,  86,
   70,  46,  25,  11,   4,   1,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0
};

FakePulseSensor::FakePulseSensor(uint8_t jitterPct, uint8_t noise,
                                 uint8_t missPct, int8_t outPin)
    : _jitter(jitterPct), _noise(noise), _missPct(missPct), _pin(outPin),
      _minBpm(55), _maxBpm(110), _targetBpm(75), _bpmAvg(0),
      _measureMs(60000UL), _fingerOffMs(15000UL),
      _state(IDLE), _beatFlag(false),
      _stateStart(0), _lastBeat(0), _interval(800), _nextDrift(0),
      _pinOffAt(0), _count(0), _beatCb(0), _fingerCb(0) {}

void FakePulseSensor::begin() {
    if (_pin >= 0) pinMode(_pin, OUTPUT);
    _state = IDLE;
}

void FakePulseSensor::setBpmRange(uint16_t minBpm, uint16_t maxBpm) {
    if (minBpm < 20) minBpm = 20;
    if (maxBpm < minBpm) maxBpm = minBpm;
    _minBpm = minBpm;
    _maxBpm = maxBpm;
}

void FakePulseSensor::setTimings(uint32_t measureMs, uint32_t fingerOffMs) {
    _measureMs = measureMs;
    _fingerOffMs = fingerOffMs;
}

void FakePulseSensor::startMeasuring() {
    if (_state == MEASURING) return;
    enterMeasuring();
}

void FakePulseSensor::stopMeasuring() {
    bool wasOn = (_state == MEASURING);
    _state = IDLE;
    _bpmAvg = 0;
    if (_pin >= 0) digitalWrite(_pin, LOW);
    if (wasOn && _fingerCb) _fingerCb(false);
}

void FakePulseSensor::enterMeasuring() {
    uint32_t now = millis();
    _state = MEASURING;
    _stateStart = now;
    _targetBpm = random(_minBpm, _maxBpm + 1);   // new random heart rate
    _bpmAvg = _targetBpm;
    _lastBeat = now;
    _interval = newInterval();
    _nextDrift = now + 5000UL;
    if (_fingerCb) _fingerCb(true);
}

bool FakePulseSensor::update() {
    uint32_t now = millis();
    bool detected = false;

    if (_state == IDLE) return false;

    if (_state == FINGER_OFF) {
        if (now - _stateStart >= _fingerOffMs) enterMeasuring();
        return false;
    }

    // MEASURING
    if (now - _stateStart >= _measureMs) {
        _state = FINGER_OFF;
        _stateStart = now;
        _bpmAvg = 0;
        if (_pin >= 0) digitalWrite(_pin, LOW);
        if (_fingerCb) _fingerCb(false);
        return false;
    }

    // Heart rate slowly wanders every ~5 s
    if (now >= _nextDrift) {
        int t = (int)_targetBpm + (int)random(-3, 4);
        _targetBpm = constrain(t, (int)_minBpm, (int)_maxBpm);
        _nextDrift = now + 5000UL;
    }

    while (now - _lastBeat >= _interval) {
        _lastBeat += _interval;
        uint16_t inst = 60000UL / _interval;
        _interval = newInterval();
        _bpmAvg = (_bpmAvg * 3 + inst) / 4;      // smoothed reading
        _count++;

        // Randomly "miss" some beats, like a real optical sensor
        if (random(100) >= _missPct) {
            detected = true;
            _beatFlag = true;
            if (_pin >= 0) { digitalWrite(_pin, HIGH); _pinOffAt = now + 30; }
            if (_beatCb) _beatCb(_bpmAvg);
        }
    }

    if (_pin >= 0 && _pinOffAt && now >= _pinOffAt) {
        digitalWrite(_pin, LOW);
        _pinOffAt = 0;
    }
    return detected;
}

bool FakePulseSensor::beatDetected() {
    update();
    bool f = _beatFlag;
    _beatFlag = false;
    return f;
}

int FakePulseSensor::readAnalog(int baseline, int amplitude) {
    update();

    if (_state != MEASURING) {                   // no finger: low, noisy signal
        int v = 40;
        if (_noise) v += random(-(int)_noise * 2, (int)_noise * 2 + 1);
        return constrain(v, 0, 1023);
    }

    uint32_t phase = ((millis() - _lastBeat) * 256UL) / _interval;  // 0..255
    if (phase > 255) phase = 255;
    uint8_t idx = phase >> 3, frac = phase & 7;
    int a = pgm_read_byte(&PULSE_WAVE[idx]);
    int b = pgm_read_byte(&PULSE_WAVE[(idx + 1) & 31]);
    int wave = a + ((b - a) * frac) / 8;

    int v = baseline + (int)(((long)wave * amplitude) / 255);
    if (_noise) v += random(-(int)_noise, (int)_noise + 1);
    return constrain(v, 0, 1023);
}

uint32_t FakePulseSensor::newInterval() {
    uint32_t base = 60000UL / _targetBpm;
    long j = (long)base * _jitter / 100;
    return base + (j ? random(-j, j + 1) : 0);
}