uint32_t HCNT0, HCNT1, ENCO;
bool HCNT0_EN;

void M8A8RS_Init()
{
	for (int k = 0; k < 8; k++) pinMode(32 + k, OUTPUT);
	analogReference(EXTERNAL);
	Serial.begin(9600);
}

void HCNT0_ENABLE(){
	attachInterrupt(6, __HCNT0, RISING);
	HCNT0_EN = true;
}

void HCNT1_ENABLE(){
	attachInterrupt(7, __HCNT1, RISING);
}

void ENCO_ENABLE(){
	attachInterrupt(6, __ENCO, RISING);
}

void PWM_SET(uint16_t Value){
	TCCR3A=0xAA; TCCR3B=0x19; ICR3 = Value;
}


void __HCNT0() { HCNT0++; }
void __HCNT1() { HCNT1++; }
void __ENCO() { 
	if (digitalRead(25) == 0) { ENCO++; }
	else { ENCO--; }
}
