/**
 * @file iTimer.cpp
 * @brief 산업용 아두이노(MPINO) PLC 타이머 명령어 라이브러리
 * @details TON, TOF, TPL, TMON, TMR 기능 및 상태 확인 함수 포함
 */

#include "Arduino.h"

// ==============================================================================
// [상태 저장] 사용하는 timer_id만 동적으로 확보
// ==============================================================================
struct IlogicsTimerState {
    uint8_t id;
    unsigned long startTime;
    unsigned long accumulatedTime;
    bool isTiming;
    bool outputState;
    bool prevIn;
    IlogicsTimerState* next;
};

static IlogicsTimerState* timerStates = nullptr;

static IlogicsTimerState* getTimerState(uint8_t timer_id, bool create) {
    for (IlogicsTimerState* state = timerStates; state != nullptr; state = state->next) {
        if (state->id == timer_id) {
            return state;
        }
    }

    if (!create) {
        return nullptr;
    }

    IlogicsTimerState* state = (IlogicsTimerState*)malloc(sizeof(IlogicsTimerState));
    if (state == nullptr) {
        return nullptr;
    }

    state->id = timer_id;
    state->startTime = 0;
    state->accumulatedTime = 0;
    state->isTiming = false;
    state->outputState = false;
    state->prevIn = false;
    state->next = timerStates;
    timerStates = state;
    return state;
}

// ==============================================================================
// 1. TON (On-Delay Timer)
// - 설명: 입력 신호가 ON으로 유지되는 동안 설정 시간이 경과하면 출력이 ON 됩니다.
// - 입력이 OFF되면 타이머와 출력은 즉시 초기화됩니다.
// ==============================================================================
bool Iton(uint8_t timer_id, bool state, unsigned long delayTime) {
    IlogicsTimerState* timer = getTimerState(timer_id, true);
    if (timer == nullptr) return false;

    if (state == true) {
        if (timer->isTiming == false) {
            timer->startTime = millis();
            timer->isTiming = true;
            timer->outputState = false;
        } else {
            if ((millis() - timer->startTime) >= delayTime) {
                timer->outputState = true;
            }
        }
    } else {
        timer->isTiming = false;
        timer->outputState = false;
    }

    return timer->outputState;
}

// ==============================================================================
// 2. TOF (Off-Delay Timer)
// - 설명: 입력 신호가 ON일 때 출력도 즉시 ON 되며, 입력이 OFF된 시점부터 
//         설정 시간이 경과한 후 출력이 OFF 됩니다.
// ==============================================================================
bool Itof(uint8_t timer_id, bool state, unsigned long delayTime) {
    IlogicsTimerState* timer = getTimerState(timer_id, true);
    if (timer == nullptr) return false;

    if (state == true) {
        timer->isTiming = false;
        timer->outputState = true;
    } else {
        if (timer->outputState == true) { 
            if (timer->isTiming == false) {
                timer->startTime = millis();
                timer->isTiming = true;
            } else {
                if ((millis() - timer->startTime) >= delayTime) {
                    timer->outputState = false;
                    timer->isTiming = false;
                }
            }
        }
    }

    return timer->outputState;
}

// ==============================================================================
// 3. TPL (Pulse Timer)
// - 설명: 입력 신호의 상승 에지(OFF->ON) 감지 시 출력이 ON 되며, 
//         설정 시간이 경과하면 출력이 OFF 됩니다.
// - 동작 중 재입력 시 타이머가 연장(Retrigger) 됩니다.
// ==============================================================================
bool Itpl(uint8_t timer_id, bool state, unsigned long delayTime) {
    IlogicsTimerState* timer = getTimerState(timer_id, true);
    if (timer == nullptr) return false;

    if (state == true && timer->prevIn == false) {
        timer->startTime = millis();
        timer->isTiming = true;
        timer->outputState = true;
    }
    timer->prevIn = state;

    if (timer->isTiming) {
        if ((millis() - timer->startTime) >= delayTime) {
            timer->outputState = false;
            timer->isTiming = false;
        }
    } else {
        timer->outputState = false;
    }

    return timer->outputState;
}

// ==============================================================================
// 4. TMON (Monostable Timer)
// - 설명: 입력 신호의 상승 에지 감지 시 설정 시간 동안 출력을 유지합니다.
// - TPL과 달리 동작 중 발생하는 추가 입력 신호는 무시(Non-Retrigger) 됩니다.
// ==============================================================================
bool Itmon(uint8_t timer_id, bool state, unsigned long delayTime) {
    IlogicsTimerState* timer = getTimerState(timer_id, true);
    if (timer == nullptr) return false;

    if (state == true && timer->prevIn == false && timer->isTiming == false) {
        timer->startTime = millis();
        timer->isTiming = true;
        timer->outputState = true;
    }
    timer->prevIn = state;

    if (timer->isTiming) {
        if ((millis() - timer->startTime) >= delayTime) {
            timer->outputState = false;
            timer->isTiming = false;
        }
    } else {
        timer->outputState = false;
    }

    return timer->outputState;
}

// ==============================================================================
// 5. TMR (Retentive Timer)
// - 설명: 입력 신호가 ON인 시간만을 누적하여 측정합니다.
// - 입력이 OFF 되어도 누적된 시간은 유지되며, 별도의 reset 신호로 초기화합니다.
// ==============================================================================
bool Itmr(uint8_t timer_id, bool state, unsigned long delayTime, bool reset) {
    IlogicsTimerState* timer = getTimerState(timer_id, !reset);
    if (timer == nullptr) return false;

    if (reset) {
        timer->accumulatedTime = 0;
        timer->outputState = false;
        timer->isTiming = false;
        timer->prevIn = state;
        return false;
    }

    if (state == true) {
        if (timer->prevIn == false) {
            timer->startTime = millis();
            timer->isTiming = true;
        }
        
        unsigned long now = millis();
        timer->accumulatedTime += (now - timer->startTime);
        timer->startTime = now; 

        if (timer->accumulatedTime >= delayTime) {
            timer->accumulatedTime = delayTime;
            timer->outputState = true;
        }
    } else {
        timer->isTiming = false;
    }
    
    timer->prevIn = state;
    return timer->outputState;
}

// ==============================================================================
// 6. IgetTime (진행 시간 반환)
// - 반환값: 현재 측정 중인 타이머의 경과 시간 (단위: ms)
// - 변경사항: TMR(적산 타이머) 연동을 위해 누적된 시간을 반환하는 로직을 추가했습니다.
// ==============================================================================
unsigned long IgetTime(uint8_t timer_id) {
    IlogicsTimerState* timer = getTimerState(timer_id, false);
    if (timer == nullptr) return 0;

    // TMR 적산 타이머의 누적 시간이 존재하는 경우 누적 시간을 우선 반환
    if (timer->accumulatedTime > 0) {
        return timer->accumulatedTime;
    }

    // 일반 타이머가 동작 중일 경우 경과 시간 반환
    if (timer->isTiming == true) {
        return millis() - timer->startTime;
    }
    
    return 0; 
}

// ==============================================================================
// 7. IgetState (출력 상태 반환)
// - 반환값: 해당 타이머 ID의 현재 출력 상태값 (true/false)
// ==============================================================================
bool IgetState(uint8_t timer_id) {
    IlogicsTimerState* timer = getTimerState(timer_id, false);
    if (timer == nullptr) return false; 
    return timer->outputState;
}
