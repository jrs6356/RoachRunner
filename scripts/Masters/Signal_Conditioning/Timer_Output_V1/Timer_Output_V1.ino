float freq_des = 4.9152;

void setup() {
  Serial.begin(115200);  
  makeTimer(7,freq_des);
}

void loop(){

}

void makeTimer(int p, float freq){
  int m = 84/freq;
  int m2 = m/2;
  int32_t mask_PWM_pin = digitalPinToBitMask(p);
  REG_PMC_PCER1 = 1<<4; // activate clock for PWM controller
  REG_PIOC_PDR |= mask_PWM_pin; // activate peripheral functions for pin (disables all PIO functionality)
  REG_PIOC_ABSR |= mask_PWM_pin; // choose peripheral option B
  REG_PWM_CLK = 0; // choose clock rate, 0 → full MCLK as reference 84MHz
  REG_PWM_CMR6 = 0<<9; // select clock and polarity for PWM channel (pin7) → (CPOL = 0)
  REG_PWM_CPRD6 = m; // initialize PWM period → T = value/84MHz (value: up to 16bit), value=8 → 10.5MHz
  REG_PWM_CDTY6 = m2; // initialize duty cycle, REG_PWM_CPRD6 / value = duty cycle, for 8/4 = 50%
  REG_PWM_ENA = 1<<6; // enable PWM on PWM channel (pin 7 = PWML6)
}
