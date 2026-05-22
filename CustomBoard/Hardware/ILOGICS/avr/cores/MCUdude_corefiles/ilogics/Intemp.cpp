/**
 * @file Intemp.cpp
 * @brief ILOGICS NTC temperature built-in commands.
 *
 * Converted from Library/NTEMP.* so sketches can call INTC() and INTC1()
 * directly without including the old ILIB library.
 */

#include "Arduino.h"

const static int16_t ADC_Table[] PROGMEM = {
    15813, 15785, 15754, 15720, 15683, 15644, 15602, 15556, 15509, 15458,
    15404, 15347, 15287, 15224, 15158, 15089, 15018, 14943, 14865, 14785,
    14702, 14618, 14531, 14441, 14349, 14254, 14156, 14054, 13950, 13843,
    13733, 13620, 13504, 13384, 13262, 13136, 13008, 12875, 12740, 12602,
    12461, 12313, 12163, 12009, 11853, 11694, 11532, 11369, 11202, 11035,
    10863, 10691, 10518, 10342, 10166, 9989, 9812, 9630, 9450, 9273,
    9092, 8909, 8731, 8548, 8372, 8192, 8014, 7837, 7661, 7487,
    7314, 7143, 6973, 6806, 6640, 6477, 6316, 6157, 6000, 5846,
    5694, 5546, 5399, 5256, 5115, 4977, 4842, 4709, 4580, 4453,
    4330, 4209, 4091, 3976, 3864, 3754, 3647, 3543, 3441, 3343,
    3247, 3154, 3062, 2974, 2889, 2805, 2724, 2646, 2569, 2494,
    2422, 2353, 2285, 2219, 2156, 2093, 2033, 1975, 1918, 1864,
    1811, 1759, 1710, 1661, 1614, 1569, 1526, 1483, 1442, 1403,
    1364, 1327, 1290, 1255, 1221, 1188, 1155, 1124, 1094, 1064,
    1035, 1008, 982, 956, 931, 907, 883, 859, 836, 814,
    792, 771, 751, 731, 711, 692, 674, 656, 638, 622, 605
};

const static int16_t ADC_Table1023[] PROGMEM = {
    987, 986, 984, 982, 979, 977, 974, 971, 968, 965,
    962, 958, 955, 951, 946, 942, 938, 933, 928, 923,
    918, 913, 907, 902, 896, 890, 884, 878, 871, 864,
    858, 850, 843, 836, 828, 820, 812, 804, 796, 787,
    778, 769, 759, 750, 740, 730, 720, 710, 699, 689,
    678, 668, 657, 646, 635, 624, 613, 601, 590, 579,
    568, 556, 545, 534, 523, 512, 500, 489, 478, 468,
    457, 446, 435, 425, 415, 404, 394, 384, 375, 365,
    356, 346, 337, 328, 319, 311, 302, 294, 286, 278,
    270, 263, 255, 248, 241, 234, 228, 221, 215, 209,
    203, 197, 191, 186, 180, 175, 170, 165, 160, 156,
    151, 147, 143, 139, 135, 131, 127, 123, 120, 116,
    113, 110, 107, 104, 101, 98, 95, 93, 90, 88,
    85, 83, 81, 78, 76, 74, 72, 70, 68, 66,
    65, 63, 61, 60, 58, 57, 55, 54, 52, 51,
    49, 48, 47, 46, 44, 43, 42, 41, 40, 39, 38
};

const static int16_t TH_Table[] PROGMEM = {
    -40, -39, -38, -37, -36, -35, -34, -33, -32, -31,
    -30, -29, -28, -27, -26, -25, -24, -23, -22, -21,
    -20, -19, -18, -17, -16, -15, -14, -13, -12, -11,
    -10, -9, -8, -7, -6, -5, -4, -3, -2, -1,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
    90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
    110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120
};

static int16_t readTempTable(uint8_t index) {
    return (int16_t)pgm_read_word_near(&(TH_Table[index]));
}

int INTC(unsigned int rawADC) {
    int analogInput = 0;
    bool useAnalogRead = false;

    if (rawADC < 60) {
        analogInput = analogRead(rawADC);
        useAnalogRead = true;
    }

    if (!useAnalogRead) {
        uint8_t cnt = 0;
        bool found = false;

        for (uint8_t k = 0; k < 161; k++) {
            if (rawADC > pgm_read_word_near(&(ADC_Table[k]))) {
                cnt = k;
                found = true;
                break;
            }
        }

        if (!found) return (int)(readTempTable(160) * 10);
        if (cnt == 0) return (int)(readTempTable(0) * 10);

        float rPoint = ((float)(rawADC - pgm_read_word_near(&(ADC_Table[cnt]))) /
                        (float)(pgm_read_word_near(&(ADC_Table[cnt - 1])) -
                                pgm_read_word_near(&(ADC_Table[cnt]))));
        float rTemp = readTempTable(cnt) - rPoint;
        return (int)(rTemp * 10.0f);
    }

    uint8_t cnt = 0;
    bool found = false;

    for (uint8_t k = 0; k < 161; k++) {
        if (analogInput > (int)pgm_read_word_near(&(ADC_Table1023[k]))) {
            cnt = k;
            found = true;
            break;
        }
    }

    if (!found) return (int)(readTempTable(160) * 10);
    if (cnt == 0) return (int)(readTempTable(0) * 10);

    float rPoint = ((float)(analogInput - pgm_read_word_near(&(ADC_Table1023[cnt]))) /
                    (float)(pgm_read_word_near(&(ADC_Table1023[cnt - 1])) -
                            pgm_read_word_near(&(ADC_Table1023[cnt]))));
    float rTemp = readTempTable(cnt) - rPoint;
    return (int)(rTemp * 10.0f);
}

float INTC1(int32_t ch, float bValue) {
    float average = 0.0f;
    uint16_t samples[5];
    uint8_t spiCheck = 0;

    for (uint8_t i = 0; i < 5; i++) {
        if (ch > 60) {
            samples[i] = ch;
            spiCheck = 1;
        } else {
            samples[i] = analogRead(ch);
            spiCheck = 0;
        }
    }

    for (uint8_t i = 0; i < 5; i++) {
        average += samples[i];
    }
    average /= 5.0f;

    if (average <= 0.0f) return 0.0f;

    if (spiCheck == 0) {
        average = 1023.0f / average - 1.0f;
    } else {
        average = 15999.5f / average - 1.0f;
    }

    if (average == 0.0f) return 0.0f;
    average = 10000.0f / average;

    float steinhart = average / 10000.0f;
    steinhart = log(steinhart);
    steinhart /= bValue;
    steinhart += 1.0f / (25.0f + 273.15f);
    steinhart = 1.0f / steinhart;
    steinhart -= 273.15f;

    return steinhart;
}
