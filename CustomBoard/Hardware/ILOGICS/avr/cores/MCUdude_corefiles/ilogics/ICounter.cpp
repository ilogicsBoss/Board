/**
 * @file ICounter.cpp
 * @brief ILOGICS counter built-in commands.
 */

#include "Arduino.h"

struct IlogicsCounterState {
    uint8_t id;
    int32_t value;
    bool prevCu;
    bool prevCd;
    bool QU;
    bool QD;
    IlogicsCounterState* next;
};

static IlogicsCounterState* counterStates = nullptr;

static IlogicsCounterState* getCounterState(uint8_t counter_id, bool create) {
    for (IlogicsCounterState* state = counterStates; state != nullptr; state = state->next) {
        if (state->id == counter_id) {
            return state;
        }
    }

    if (!create) {
        return nullptr;
    }

    IlogicsCounterState* state = (IlogicsCounterState*)malloc(sizeof(IlogicsCounterState));
    if (state == nullptr) {
        return nullptr;
    }

    state->id = counter_id;
    state->value = 0;
    state->prevCu = false;
    state->prevCd = false;
    state->QU = false;
    state->QD = false;
    state->next = counterStates;
    counterStates = state;
    return state;
}

bool Ictu(uint8_t counter_id, bool CU, bool RESET, int32_t PV) {
    IlogicsCounterState* counter = getCounterState(counter_id, true);
    if (counter == nullptr) return false;

    if (RESET) {
        counter->value = 0;
        counter->QU = false;
    } else {
        if (CU == true && counter->prevCu == false) {
            counter->value++;
        }

        counter->QU = (counter->value >= PV);
    }

    counter->prevCu = CU;
    return counter->QU;
}

bool Ictd(uint8_t counter_id, bool CD, bool LOAD, int32_t PV) {
    IlogicsCounterState* counter = getCounterState(counter_id, true);
    if (counter == nullptr) return false;

    if (LOAD) {
        counter->value = PV;
        counter->QD = false;
    } else {
        if (CD == true && counter->prevCd == false) {
            counter->value--;
        }

        counter->QD = (counter->value <= 0);
    }

    counter->prevCd = CD;
    return counter->QD;
}

bool Ictud(uint8_t counter_id, bool CU, bool CD, bool RESET, bool LOAD, int32_t PV) {
    IlogicsCounterState* counter = getCounterState(counter_id, true);
    if (counter == nullptr) return false;

    if (RESET) {
        counter->value = 0;
    } else if (LOAD) {
        counter->value = PV;
    } else {
        if (CU == true && counter->prevCu == false) {
            counter->value++;
        }

        if (CD == true && counter->prevCd == false) {
            counter->value--;
        }
    }

    counter->QU = (counter->value >= PV);
    counter->QD = (counter->value <= 0);
    counter->prevCu = CU;
    counter->prevCd = CD;

    return counter->QU;
}

int32_t IgetCount(uint8_t counter_id) {
    IlogicsCounterState* counter = getCounterState(counter_id, false);
    if (counter == nullptr) return 0;
    return counter->value;
}

void IsetCount(uint8_t counter_id, int32_t value) {
    IlogicsCounterState* counter = getCounterState(counter_id, true);
    if (counter == nullptr) return;
    counter->value = value;
}

bool IctudDownState(uint8_t counter_id) {
    IlogicsCounterState* counter = getCounterState(counter_id, false);
    if (counter == nullptr) return false;
    return counter->QD;
}
