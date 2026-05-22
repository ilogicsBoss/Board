/**
 * @file Ialt.cpp
 * @brief 산업용 아두이노 원버튼 토글(Alternate/Flip-Flop) 명령어 라이브러리
 * @details 사용자 ID 입력이나 별도의 이전 상태 변수 선언 없이, 제어 대상 변수(상태 변수)의 
 * 메모리 주소를 내부적으로 추적하여 자동으로 독립 동작하는 스마트 토글 함수입니다.
 */

#include "Arduino.h"

#define MAX_IALT_VARS 32 // 통합 토글(Ialt) 최대 동시 사용 가능 개수 제한

// ==============================================================================
// [메모리 풀] 토글 상태 자동 관리를 위한 정적 배열
// - 설명: 사용자가 ID를 입력하지 않아도 되도록, 변수의 주소값을 추적하여 상태를 기억합니다.
// ==============================================================================
static bool* alt_addresses[MAX_IALT_VARS] = {nullptr, }; // 제어 대상 변수의 메모리 주소 저장
static bool alt_prev_inputs[MAX_IALT_VARS] = {false, };  // 각 변수별 입력 신호의 이전 상태 (에지 감지용)

// ==============================================================================
// 1. Ialt (Alternate / 토글 / 플립플롭)
// - 설명: 입력(input) 신호의 상승 에지(OFF -> ON)마다 대상 변수(state_var)의 상태를 반전시킵니다.
//         푸시 버튼 하나로 기동/정지를 교대로 제어할 때 사용합니다.
// - 매개변수:
//   * input: 트리거가 되는 입력 신호 (예: digitalRead(0) 또는 Ibounce 거친 신호)
//   * state_var: 제어하고자 하는 타겟 변수 (예: pumpState, 램프 상태 등)
// ==============================================================================
void Ialt(bool input, bool &state_var) {
    int index = -1;

    // 1. 등록 여부 확인: 현재 들어온 상태 변수(state_var)의 주소가 배열에 이미 있는지 검색
    for (int i = 0; i < MAX_IALT_VARS; i++) {
        if (alt_addresses[i] == &state_var) {
            index = i; // 이미 등록된 변수면 해당 인덱스 번호 부여
            break;
        }
    }

    // 2. 신규 등록: 배열에 없다면 (처음 호출되는 변수라면), 비어있는(nullptr) 자리에 새로 할당
    if (index == -1) {
        for (int i = 0; i < MAX_IALT_VARS; i++) {
            if (alt_addresses[i] == nullptr) {
                alt_addresses[i] = &state_var; // 현재 변수의 메모리 주소값 저장
                index = i;
                break;
            }
        }
    }

    // 3. 토글 로직 수행: 인덱스를 찾았거나 새로 등록을 완료한 경우 동작
    if (index != -1) {
        // 입력 신호의 상승 에지(OFF -> ON) 감지
        if (input == true && alt_prev_inputs[index] == false) { 
            state_var = !state_var; // 타겟 변수의 현재 상태를 반전 (ON->OFF, OFF->ON)
        }
        
        // 다음 스캔 사이클의 에지 감지를 위해 현재 입력 상태를 메모리에 기억
        alt_prev_inputs[index] = input; 
    }
}