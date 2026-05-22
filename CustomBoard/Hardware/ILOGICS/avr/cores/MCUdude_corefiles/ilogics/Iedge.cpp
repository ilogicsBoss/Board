/**
 * @file Iedge.cpp
 * @brief 산업용 아두이노(MPINO) 펄스(에지) 트리거 명령어 라이브러리
 * @details PLC의 PLS(상승 에지), PLF(하강 에지)와 동일하게 동작합니다.
 */

#include "Arduino.h"

#define MAX_Iedge_VARS 32 // 통합 에지 트리거 최대 동시 사용 가능 개수

// ==============================================================================
// [메모리 풀] 에지 상태 관리를 위한 정적 배열
// ==============================================================================
static bool edge_prev_up[MAX_Iedge_VARS] = {false, };   // 상승 에지용 이전 상태
static bool edge_prev_down[MAX_Iedge_VARS] = {false, }; // 하강 에지용 이전 상태

// ==============================================================================
// 1. Iup (Rising Edge Trigger / 상승 에지 펄스 / PLS)
// - 설명: 입력(input)이 OFF에서 ON으로 변하는 찰나에 딱 1스캔 동안만 true를 반환합니다.
// ==============================================================================
bool Iup(uint8_t id, bool input) {
    if (id >= MAX_Iedge_VARS) return false;

    // 상승 에지(OFF -> ON) 감지
    bool pulse = (input == true && edge_prev_up[id] == false);
    edge_prev_up[id] = input; // 현재 상태 기억
    
    return pulse;
}

// ==============================================================================
// 2. Idown (Falling Edge Trigger / 하강 에지 펄스 / PLF)
// - 설명: 입력(input)이 ON에서 OFF로 변하는 찰나에 딱 1스캔 동안만 true를 반환합니다.
// ==============================================================================
bool Idown(uint8_t id, bool input) {
    if (id >= MAX_Iedge_VARS) return false;

    // 하강 에지(ON -> OFF) 감지
    bool pulse = (input == false && edge_prev_down[id] == true);
    edge_prev_down[id] = input; // 현재 상태 기억
    
    return pulse;
}