#ifndef FAKEHEARTBEATSENSOR_H
#define FAKEHEARTBEATSENSOR_H
#include <Arduino.h>

// Simulates a finger-clip heart rate sensor.
//
//  IDLE       --startMeasuring()-->  MEASURING (finger on, random BPM, beats)
//  MEASURING  --after 60 s------->   FINGER_OFF (no beats, BPM = 0)
//  FINGER_OFF --after 15 s------->   MEASURING (new random BPM)
//
// Call update() (or readAnalog()) every loop(); no threads needed.
class FakePulseSensor {
public:
    enum State : uint8_t { IDLE, MEASURING, FINGER_OFF };

    typedef void (*BeatCallback)(uint16_t bpm);    // fired on each detected beat
    typedef void (*FingerCallback)(bool fingerOn); // fired when finger on/off changes

    // jitterPct: beat-to-beat variation (%) | noise: analog noise (ADC counts)
    // missPct:   % of beats the "sensor" fails to detect | outPin: LED pin or -1
    FakePulseSensor(uint8_t jitterPct = 5, uint8_t noise = 8,
                    uint8_t missPct = 3, int8_t outPin = -1);

    void begin();

    // --- commands ---
    void startMeasuring();   // finger on, begin measuring
    void stopMeasuring();    // back to IDLE

    // --- settings ---
    void setBpmRange(uint16_t minBpm, uint16_t maxBpm);       // default 55..110
    void setTimings(uint32_t measureMs, uint32_t fingerOffMs); // default 60000 / 15000
    void onBeat(BeatCallback cb)      { _beatCb = cb; }
    void onFinger(FingerCallback cb)  { _fingerCb = cb; }

    // --- polling ---
    bool update();                    // true if a beat was detected during this call
    bool beatDetected();              // true once per detected beat (clears itself)
    bool fingerDetected() const { return _state == MEASURING; }
    uint16_t bpm() const        { return _bpmAvg; }   // 0 when no finger
    State state() const         { return _state; }
    uint32_t beatCount() const  { return _count; }

    // PPG-like sample 0..1023 (flat low signal when finger is off)
    int readAnalog(int baseline = 300, int amplitude = 500);

private:
    void enterMeasuring();
    uint32_t newInterval();

    uint8_t _jitter, _noise, _missPct;
    int8_t _pin;
    uint16_t _minBpm, _maxBpm, _targetBpm, _bpmAvg;
    uint32_t _measureMs, _fingerOffMs;

    State _state;
    bool _beatFlag;
    uint32_t _stateStart, _lastBeat, _interval, _nextDrift, _pinOffAt, _count;

    BeatCallback _beatCb;
    FingerCallback _fingerCb;
};
#endif