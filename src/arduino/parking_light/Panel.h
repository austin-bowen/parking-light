#include <Button.h>


class Panel {
  public:
    Panel(const int close_enough_button_pin, const int too_close_button_pin) {
      close_enough_button_ = new Button(close_enough_button_pin);
      too_close_button_ = new Button(too_close_button_pin);
    }

    ~Panel() {
      delete close_enough_button_;
      delete too_close_button_;
    }

    void begin() {
      close_enough_button_->begin();
      too_close_button_->begin();
    }

    bool close_enough_pressed() {
      return close_enough_button_->pressed();
    }

    bool too_close_pressed() {
      return too_close_button_->pressed();
    }

  private:
    Button *close_enough_button_;
    Button *too_close_button_;
};
