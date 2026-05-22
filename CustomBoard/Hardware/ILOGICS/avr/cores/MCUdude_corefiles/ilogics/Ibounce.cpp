/**
 * @file Ibounce.cpp
 * @brief 산업용 아두이노(MPINO) 스위치 채터링(디바운스) 방지 라이브러리
 * @details Ibounce, IbounceOn, IbounceOff 기능 포함 (노이즈 필터링)
 */

#include "Arduino.h"

struct IbounceState {
    uint8_t pin;
    uint8_t lastReading;
    uint8_t buttonState;
    unsigned long lastDebounceTime;
    IbounceState* next;
};

struct IbounceDelayState {
    uint8_t pin;
    bool isTiming;
    bool outputState;
    unsigned long startTime;
    IbounceDelayState* next;
};

static IbounceState* ibounceStates = nullptr;
static IbounceDelayState* ibounceOnStates = nullptr;
static IbounceDelayState* ibounceOffStates = nullptr;

static IbounceState* getIbounceState(uint8_t pin) {
    for (IbounceState* state = ibounceStates; state != nullptr; state = state->next) {
        if (state->pin == pin) {
            return state;
        }
    }

    IbounceState* state = (IbounceState*)malloc(sizeof(IbounceState));
    if (state == nullptr) {
        return nullptr;
    }

    state->pin = pin;
    state->lastReading = LOW;
    state->buttonState = LOW;
    state->lastDebounceTime = 0;
    state->next = ibounceStates;
    ibounceStates = state;
    return state;
}

static IbounceDelayState* getIbounceDelayState(IbounceDelayState** head, uint8_t pin) {
    for (IbounceDelayState* state = *head; state != nullptr; state = state->next) {
        if (state->pin == pin) {
            return state;
        }
    }

    IbounceDelayState* state = (IbounceDelayState*)malloc(sizeof(IbounceDelayState));
    if (state == nullptr) {
        return nullptr;
    }

    state->pin = pin;
    state->isTiming = false;
    state->outputState = false;
    state->startTime = 0;
    state->next = *head;
    *head = state;
    return state;
}

// ==============================================================================
// 1. Ibounce (양방향 노이즈 필터링)
// - 설명: 입력 신호가 ON 또는 OFF 상태로 변경되었을 때, 
//         설정된 시간 동안 해당 상태가 안정적으로 유지될 때만 최종 상태로 인식합니다.
// ==============================================================================
bool Ibounce(uint8_t pin, uint32_t debounceTime) {
    if (pin >= NUM_DIGITAL_PINS) return false;

    uint8_t reading = digitalRead(pin);
    IbounceState* state = getIbounceState(pin);
    if (state == nullptr) {
        return (reading == HIGH);
    }

    if (reading != state->lastReading) {
        state->lastDebounceTime = millis();
    }

    if ((millis() - state->lastDebounceTime) > debounceTime) {
        if (reading != state->buttonState) {
            state->buttonState = reading;
        }
    }

    state->lastReading = reading;
    return (state->buttonState == HIGH);
}

// ==============================================================================
// 2. IbounceOn (On-지연 필터링)
// - 설명: 신호가 OFF에서 ON으로 변할 때, 설정된 시간 동안 ON 상태가 
//         지속될 때만 실제 ON으로 인식합니다. (OFF 시에는 즉각 반영)
// ==============================================================================
bool IbounceOn(uint8_t pin, unsigned long onDelayTime) {
    if (pin >= NUM_DIGITAL_PINS) return false;

    uint8_t reading = digitalRead(pin);
    IbounceDelayState* state = getIbounceDelayState(&ibounceOnStates, pin);
    if (state == nullptr) {
        return (reading == HIGH);
    }

    if (reading == HIGH) {
        if (state->isTiming == false) {
            state->startTime = millis();
            state->isTiming = true;
        } else {
            if ((millis() - state->startTime) >= onDelayTime) {
                state->outputState = true; 
            }
        }
    } else {
        state->isTiming = false;
        state->outputState = false;
    }

    return state->outputState;
}

// ==============================================================================
// 3. IbounceOff (Off-지연 필터링)
// - 설명: 신호가 ON에서 OFF로 변할 때, 설정된 시간 동안 OFF 상태가 
//         지속될 때만 실제 OFF로 인식합니다. (ON 시에는 즉각 반영)
// - 참고: 기존 IbounceOnOff 함수명을 릴리즈 노트에 맞춰 IbounceOff로 수정 반영.
// ==============================================================================
bool IbounceOff(uint8_t pin, unsigned long offDelayTime) {
    if (pin >= NUM_DIGITAL_PINS) return false;

    uint8_t reading = digitalRead(pin);
    IbounceDelayState* state = getIbounceDelayState(&ibounceOffStates, pin);
    if (state == nullptr) {
        return (reading == HIGH);
    }

    if (reading == HIGH) {
        state->outputState = true;
        state->isTiming = false; 
    } else {
        if (state->outputState == true) { 
            if (state->isTiming == false) {
                state->startTime = millis();
                state->isTiming = true;
            } else {
                if ((millis() - state->startTime) >= offDelayTime) {
                    state->outputState = false; 
                    state->isTiming = false;    
                }
            }
        }
    }

    return state->outputState;
}
