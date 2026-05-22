#include "NTEMP.h"

const static int16_t ADC_Table[] PROGMEM = { 15813,  15785,  15754,  15720,  15683,  15644,  15602,  15556,  15509,  15458,  15404,  15347,  15287,  15224,  15158,  15089,  15018,  14943,  14865,  14785,  14702,  14618,  14531,  14441,  14349,  14254,  14156,  14054,  13950,  13843,  13733,  13620,  13504,  13384,  13262,  13136,  13008,  12875,  12740,  12602,  12461,  12313,  12163,  12009,  11853,  11694,  11532,  11369,  11202,  11035,  10863,  10691,  10518,  10342,  10166,  9989, 9812, 9630, 9450, 9273, 9092, 8909, 8731, 8548, 8372, 8192, 8014, 7837, 7661, 7487, 7314, 7143, 6973, 6806, 6640, 6477, 6316, 6157, 6000, 5846, 5694, 5546, 5399, 5256, 5115, 4977, 4842, 4709, 4580, 4453, 4330, 4209, 4091, 3976, 3864, 3754, 3647, 3543, 3441, 3343, 3247, 3154, 3062, 2974, 2889, 2805, 2724, 2646, 2569, 2494, 2422, 2353, 2285, 2219, 2156, 2093, 2033, 1975, 1918, 1864, 1811, 1759, 1710, 1661, 1614, 1569, 1526, 1483, 1442, 1403, 1364, 1327, 1290, 1255, 1221, 1188, 1155, 1124, 1094, 1064, 1035, 1008, 982,  956,  931,  907,  883,  859,  836,  814,  792,  771,  751,  731,  711,  692,  674,  656,  638,  622,  605 };
const static int16_t ADC_Table1023[] PROGMEM = { 987, 986, 984, 982, 979, 977, 974, 971, 968, 965, 962, 958, 955, 951, 946, 942, 938, 933, 928, 923, 918, 913, 907, 902, 896, 890, 884, 878, 871, 864, 858, 850, 843, 836, 828, 820, 812, 804, 796, 787, 778, 769, 759, 750, 740, 730, 720, 710, 699, 689, 678, 668, 657, 646, 635, 624, 613, 601, 590, 579, 568, 556, 545, 534, 523, 512, 500, 489, 478, 468, 457, 446, 435, 425, 415, 404, 394, 384, 375, 365, 356, 346, 337, 328, 319, 311, 302, 294, 286, 278, 270, 263, 255, 248, 241, 234, 228, 221, 215, 209, 203, 197, 191, 186, 180, 175, 170, 165, 160, 156, 151, 147, 143, 139, 135, 131, 127, 123, 120, 116, 113, 110, 107, 104, 101, 98, 95, 93, 90, 88, 85, 83, 81, 78, 76, 74, 72, 70, 68, 66, 65, 63, 61, 60, 58, 57, 55, 54, 52, 51, 49, 48, 47, 46, 44, 43, 42, 41, 40, 39, 38 };
const static int16_t TH_Table[] PROGMEM = { -40,  -39,  -38,  -37,  -36,  -35,  -34,  -33,  -32,  -31,  -30,  -29,  -28,  -27,  -26,  -25,  -24,  -23,  -22,  -21,  -20,  -19,  -18,  -17,  -16,  -15,  -14,  -13,  -12,  -11,  -10,  -9, -8, -7, -6, -5, -4, -3, -2, -1, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100,  101,  102,  103,  104,  105,  106,  107,  108,  109,  110,  111,  112,  113,  114,  115,  116,  117,  118,  119,  120 };


//데이터표방식
int INTC(unsigned int RawADC)
{
	int analogInput = 0;
	bool tway = 0;
	if (RawADC < 60) {
		analogInput = analogRead(RawADC);
		tway = 1;

	}
	else { tway = 0; }
	uint8_t Cnt = 0;
	uint8_t Cnt1 = 0;
	bool foundCnt = false;    // [FIX] for 루프가 break 했는지 추적 (안 했으면 경계값)
	bool foundCnt1 = false;

	uint8_t k = 0;

	if (tway == 0) {			//spi입력
		for (k = 0; k < 161; k++) {
			if (RawADC > pgm_read_word_near(&(ADC_Table[k]))) {
				Cnt = k;
				foundCnt = true;
				break;
			}
		}
		// [FIX] 경계값 처리: 테이블 범위를 벗어나면 끝값으로 클램프
		if (!foundCnt) {
			// RawADC가 테이블 마지막 값(가장 작은 값)보다 작음 → 최고온도로 클램프
			return (int)(pgm_read_word_near(&(TH_Table[160])) * 10);
		}
		if (Cnt == 0) {
			// RawADC가 테이블 첫 값(가장 큰 값)보다 큼 → 최저온도로 클램프
			return (int)(pgm_read_word_near(&(TH_Table[0])) * 10);
		}
		float rPoint = ((float)(RawADC - pgm_read_word_near(&(ADC_Table[Cnt]))) / (float)(pgm_read_word_near(&(ADC_Table[Cnt - 1])) - pgm_read_word_near(&(ADC_Table[Cnt]))));
		float rTemp = pgm_read_word_near(&(TH_Table[Cnt])) - rPoint;
		return (int)(rTemp * 10.0);  // [FIX] unsigned int 캐스트 제거 (음수 온도 지원)
	}
	else /* tway == 1 */ {		//analogRead입력
		for (uint8_t k1 = 0; k1 < 161; k1++) {
			if (analogInput > (int)pgm_read_word_near(&(ADC_Table1023[k1]))) {
				Cnt1 = k1;
				foundCnt1 = true;
				break;
			}
		}
		// [FIX] 경계값 처리
		if (!foundCnt1) {
			return (int)(pgm_read_word_near(&(TH_Table[160])) * 10);
		}
		if (Cnt1 == 0) {
			return (int)(pgm_read_word_near(&(TH_Table[0])) * 10);
		}
		float rPoint1 = ((float)(analogInput - pgm_read_word_near(&(ADC_Table1023[Cnt1]))) / (float)(pgm_read_word_near(&(ADC_Table1023[Cnt1 - 1])) - pgm_read_word_near(&(ADC_Table1023[Cnt1]))));
		float rTemp1 = pgm_read_word_near(&(TH_Table[Cnt1])) - rPoint1;
		return (int)(rTemp1 * 10.0);  // [FIX] unsigned int 캐스트 제거
	}
}


//로그방식
float INTC1(int32_t _ch, float _bValue)
{
	uint8_t i;
	float average;
	uint16_t samples[5];
	//float _bValue;
	int rValue;
	uint8_t SpiCheck = 0;

	for (i = 0; i < 5; i++) {
		// take N samples in a row, with a slight delay
		if (_ch > 60) {
			samples[i] = _ch;
			SpiCheck = 1;
		}
		else {
			samples[i] = analogRead(_ch);
			SpiCheck = 0;
		}
	}
	if (SpiCheck == 0) {
		// average all the samples out
		average = 0;
		for (i = 0; i < 5; i++) {
			average += samples[i];
		}
		average /= 5;

		// convert the value to resistance
		average = 1023.0 / average - 1.0;
		average = 10000.0 / average;

		float steinhart;
		steinhart = average / 10000;     // (R/Ro)
		steinhart = log(steinhart);                  // ln(R/Ro)
		steinhart /= _bValue;                   // 1/B * ln(R/Ro)
		steinhart += 1.0 / (25 + 273.15); // + (1/To)
		steinhart = 1.0 / steinhart;                 // Invert
		steinhart -= 273.15;                         // convert to C                                           

		return steinhart;
	}
	else if (SpiCheck == 1) {
		// average all the samples out
		average = 0;
		for (i = 0; i < 5; i++) {
			average += samples[i];
		}
		average /= 5;

		// convert the value to resistance
		average = 15999.5 / average - 1.0;
		average = 10000.0 / average;

		float steinhart;
		steinhart = average / 10000;     // (R/Ro)
		steinhart = log(steinhart);                  // ln(R/Ro)
		steinhart /= _bValue;                   // 1/B * ln(R/Ro)
		steinhart += 1.0 / (25 + 273.15); // + (1/To)
		steinhart = 1.0 / steinhart;                 // Invert
		steinhart -= 273.15;                         // convert to C                                           

		return steinhart;
	}
}
