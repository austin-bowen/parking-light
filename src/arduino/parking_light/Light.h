class Light {
  public:
    Light(const int pin, const unsigned long blink_period):
      pin_(pin), blink_period_(blink_period) {}

    ~Light() {
      turnOff();
    }

    void turnOn() {
      set(true);
    }

    void turnOff() {
      set(false);
    }

    void blink() {
      if (!is_blinking_) {
        is_blinking_ = true;
        blink_offset_ = millis();
      }

      const unsigned long t = millis() - blink_offset_;
      bool on = (t % blink_period_) > (blink_period_ / 2);

      set(on);
    }

    void set(bool on) {
      digitalWrite(pin_, on);
    }

  private:
    const int pin_;
    const unsigned long blink_period_;

    bool is_blinking_ = false;
    unsigned long blink_offset_;
};
