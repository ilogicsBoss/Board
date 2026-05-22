/**
 * @file Iscale.cpp
 * @brief 산업용 아두이노 아날로그 스케일링 명령어 라이브러리
 * @details 기본 map() 함수의 한계(소수점 불가, 범위 이탈)를 극복하고,
 * 센서의 원시 데이터(Raw)를 실제 물리량(Float)으로 정밀하고 안전하게 변환합니다.
 */

#include "Arduino.h"

// ==============================================================================
// 1. Iscale (Analog Scaling / 아날로그 정밀 변환)
// - 매개변수:
//   * x: 변환할 원시 입력값 (예: analogRead 값 0~1023)
//   * in_min / in_max: 입력값의 최소/최대 범위
//   * out_min / out_max: 실제 물리량으로 변환할 출력값의 최소/최대 범위 (소수점 가능)
// - 반환값: 변환된 실제 물리량 (float)
// ==============================================================================
float Iscale(float x, float in_min, float in_max, float out_min, float out_max) {
    // 1. 예외 처리: 입력 범위가 같아서 발생하는 0으로 나누기(Divide by Zero) 치명적 오류 방지
    if (in_min == in_max) {
        return out_min; 
    }

    // 2. 선형 보간법 (Linear Interpolation) 정밀 실수 연산
    float result = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

    // 3. 산업용 안전장치 (Clamping): 노이즈로 인해 결과값이 범위를 뚫고 나가는 현상 방지
    if (out_min < out_max) { // 정상 스케일 (예: 0~100)
        if (result < out_min) return out_min;
        if (result > out_max) return out_max;
    } else {                 // 반전 스케일 (예: 100~0)
        if (result > out_min) return out_min;
        if (result < out_max) return out_max;
    }

    return result; // 안전하게 정제된 최종 소수점 결과값 반환
}