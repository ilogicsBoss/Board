/**
 * @file IanalogFilter.cpp
 * @brief 산업용 아두이노(MPINO) 아날로그 이동 평균 필터 및 통합 읽기 명령어
 */

#include "Arduino.h"

inline void* operator new(size_t, void* ptr) {
    return ptr;
}

#if defined(__AVR_ATmega2560__) && \
    (defined(MP8A8R) || defined(MP16A16R) || defined(MP32A16R) || \
     defined(MP16A32R) || defined(MP24A24R) || defined(MP32A32R) || \
     defined(M48A48R) || defined(M64A64R) || defined(M48A48R_OLD) || \
     defined(M64A64R_OLD))
#define ILOGICS_ENABLE_MPAINO_IANALOGREAD 1
#endif

#if defined(ILOGICS_ENABLE_MPAINO_IANALOGREAD)
#define IANALOG_FILTER_ID_LIMIT 20
#else
// 대표님 요청: 메모리 최적화를 위해 MPINO는 최대 필터 개수를 6개(0~5번 채널)로 제한
#define IANALOG_FILTER_ID_LIMIT 6
#endif
#define MAX_FILTER_SAMPLES 20  // 한 채널당 최대 누적 샘플 개수

// ==============================================================================
// [메모리 풀] 사용하는 필터 ID만 동적으로 확보
// ==============================================================================
struct IanalogFilterState {
    uint8_t id;
    uint8_t samples;
    uint8_t index;
    uint8_t count;
    int32_t sum;
    int32_t* buffer;
    IanalogFilterState* next;
};

static IanalogFilterState* filterStates = nullptr;

static IanalogFilterState* findFilterState(uint8_t id) {
    for (IanalogFilterState* state = filterStates; state != nullptr; state = state->next) {
        if (state->id == id) {
            return state;
        }
    }

    IanalogFilterState* state = (IanalogFilterState*)malloc(sizeof(IanalogFilterState));
    if (state == nullptr) {
        return nullptr;
    }

    state->id = id;
    state->samples = 0;
    state->index = 0;
    state->count = 0;
    state->sum = 0;
    state->buffer = nullptr;
    state->next = filterStates;
    filterStates = state;
    return state;
}

static bool prepareFilterBuffer(IanalogFilterState* state, uint8_t samples) {
    if (state->buffer == nullptr) {
        state->buffer = (int32_t*)malloc((size_t)samples * sizeof(int32_t));
        if (state->buffer == nullptr) {
            return false;
        }
    } else if (state->samples != samples) {
        int32_t* resized = (int32_t*)realloc(state->buffer, (size_t)samples * sizeof(int32_t));
        if (resized == nullptr) {
            return false;
        }
        state->buffer = resized;
    } else {
        return true;
    }

    state->samples = samples;
    state->index = 0;
    state->count = 0;
    state->sum = 0;
    for (uint8_t i = 0; i < samples; i++) {
        state->buffer[i] = 0;
    }
    return true;
}

// ==============================================================================
// 1. IanalogFilter (기본 이동 평균 필터)
// ==============================================================================
int32_t IanalogFilter(uint8_t id, int32_t raw_value, uint8_t samples) {
    if (samples == 0) return raw_value;
    if (id >= IANALOG_FILTER_ID_LIMIT) return raw_value;
    
    if (samples > MAX_FILTER_SAMPLES) samples = MAX_FILTER_SAMPLES;

    IanalogFilterState* state = findFilterState(id);
    if (state == nullptr || !prepareFilterBuffer(state, samples)) {
        return raw_value;
    }

    state->sum -= state->buffer[state->index];
    state->buffer[state->index] = raw_value;
    state->sum += raw_value;
    
    state->index++;
    if (state->index >= samples) {
        state->index = 0;
    }

    if (state->count < samples) {
        state->count++;
        return state->sum / state->count;
    }

    return state->sum / samples;
}

// ==============================================================================
// 2. IanalogRead (원시 데이터 읽기 + 필터링 통합 함수)
// - MPINO: analogRead() 값을 필터링합니다.
// - MPAINO: IanalogRead(ch)로 읽은 ADS1118 raw 값을 필터링합니다.
// - 매개변수:
//   * ch: 아날로그 핀 번호 (예: 0, 1, 2 또는 A0, A1, A2)
//   * samples: 평균을 낼 샘플 개수 (숫자가 클수록 부드러워짐)
// ==============================================================================
int32_t IanalogRead(uint8_t ch, uint8_t samples) {
#if defined(ILOGICS_ENABLE_MPAINO_IANALOGREAD)
    int32_t raw_value = IanalogRead(ch);
    return IanalogFilter(ch, raw_value, samples);
#else
    // 사용자가 '0' 대신 'A0'(아두이노 핀 번호)를 입력했을 때를 대비한 안전장치
    uint8_t id = ch;
#if defined(A0)
    if (id >= A0) {
        id = id - A0; // A0가 들어오면 0으로, A1이 들어오면 1로 변환
    }
#endif

    // 1. 센서에서 원시 데이터 읽기
    int32_t raw_value = analogRead(ch);

    // 2. 읽은 데이터와 채널 ID를 바로 필터 함수로 전달하여 결과 반환
    return IanalogFilter(id, raw_value, samples);
#endif
}

// ==============================================================================
// 3. IanalogRead (MPAINO ADS1118 raw read)
// - ch 0~3   : user module index 0, IADC module 1, ADS1118 channel 0~3
// - ch 4~7   : user module index 1, IADC module 2, ADS1118 channel 0~3
// - ch 8~19  : user module index 2~4, IADC module 3~5
// ==============================================================================
uint16_t IanalogRead(uint8_t ch) {
#if defined(ILOGICS_ENABLE_MPAINO_IANALOGREAD)
    const uint8_t moduleIndex = ch / 4;
    const uint8_t adcChannel = ch % 4;

    if (moduleIndex >= 5) {
        return 0;
    }

    static IADC* iadcModules[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};

    if (iadcModules[moduleIndex] == nullptr) {
        void* memory = malloc(sizeof(IADC));
        if (memory == nullptr) {
            return 0;
        }
        iadcModules[moduleIndex] = new (memory) IADC(moduleIndex + 1);
        iadcModules[moduleIndex]->INIT(0, 0, 4);
    }

    long value = iadcModules[moduleIndex]->GET_ADC(adcChannel);
    if (value <= 0) {
        return 0;
    }
    if (value >= 65535L) {
        return 65535U;
    }
    return (uint16_t)value;
#else
    (void)ch;
    return 0;
#endif
}
