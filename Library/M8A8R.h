uint32_t HCNT0, HCNT1;

void M8A8R_Init()
{
	for (int k = 0; k < 8; k++) pinMode(62 + k, OUTPUT);
	analogReference(EXTERNAL);
	Serial.begin(9600);
}

void HCNT0_ENABLE(){
	TIMSK1 = 0x01; TCCR1A = 0x00; TCCR1B = 0x1F;
}

void HCNT1_ENABLE(){
	TIMSK5 = 0x01; TCCR5A = 0x00; TCCR5B = 0x1F;
}

void HCNT0_32BIT(){
	TIMSK1 = 0x01; TCCR1A = 0x00; TCCR1B = 0x1F;
}

void HCNT1_32BIT(){
	
}

void PWM_SET(uint16_t Value){
	TCCR3A=0xAA; TCCR3B=0x1A; ICR3 = Value;
	TCCR4A=0xAA; TCCR4B=0x1A; ICR4 = Value;
}


