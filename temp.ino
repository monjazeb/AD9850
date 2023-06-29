/*------------------------------------------------------------------*
Uses 9 short ints of EEPROM to store the Frequency and phase


*------------------------------------------------------------------*/
class AD9850 {
  private:
    short int _wclk, _fqud, _reset, _data;
    double frequency;
    short int phase;
    short int at;
    bool x2;

    void tfr_byte(short int d) {
      for (int i = 0; i < 8; i++, d >>= 1) {
        digitalWrite(this->_data, d & 0x01);
        pulseHigh(this->_wclk);
      }
    }

  public:
    AD9850(short int at, short int wclk, short int fqud, short int rst, short int dta) {
      this->_wclk = wclk;
      this->_fqud = fqud;
      this->_reset = rst;
      this->_data = dta;

      pinMode(fqud, OUTPUT);
      pinMode(wclk, OUTPUT);
      pinMode(dta, OUTPUT);
      pinMode(rst, OUTPUT);

      pulseHigh(this->_reset);
      pulseHigh(this->_wclk);
      pulseHigh(this->_fqud);
    }

    void update() {
      int32_t freq = this->frequency * 4294967295 / 125000000;
      if (this->x2) freq = freq * 2;
      for (int b = 0; b < 4; b++, freq >>= 8) {
        tfr_byte(freq & 0xFF);
      }
      tfr_byte(this->phase & 0xf0);
      pulseHigh(this->_fqud);
    }

    void set(double frequency, int phaze) {
      this->frequency = frequency;
      this->phase = phaze;
      EEPROM.put(this->at + 0, this->frequency);
      EEPROM.put(this->at + 8, this->phase);
      this->update();
    }

    void setPhase(int phaze) {
      this->phase = phaze;
      EEPROM.put(this->at + 8, this->phase);
      this->update();
    }

    double getFrequency() {
      return this->frequency;
    }

    short int getPhase() {
      return this->phase;
    }
};