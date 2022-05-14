#include <HCSR04.h>


class DistanceSensor {
  public:
    DistanceSensor(
      const int trigger_pin,
      const int echo_pin,
      const unsigned long active_polling_period,
      const unsigned long inactive_polling_period,
      const float active_variance,
      const unsigned long active_timeout
    ): active_polling_period_(active_polling_period),
       inactive_polling_period_(inactive_polling_period),
       active_variance_(active_variance),
       active_timeout_(active_timeout)
    {
      sensor_ = new HCSR04(trigger_pin, echo_pin);
    }

    ~DistanceSensor() {
      delete sensor_;
    }

    // Gets the distance in meters
    float get_dist() {
      if (is_time_to_update()) {
        update_dist();
        check_inactive();
      }

      return dist_;
    }

    bool is_active() {
      return is_active_;
    }

  private:
    HCSR04 *sensor_;
    const unsigned long active_polling_period_;
    const unsigned long inactive_polling_period_;
    const float active_variance_;
    const unsigned long active_timeout_;

    float dist_ = 0;
    bool is_active_ = true;
    float last_active_dist_ = 0;
    unsigned long last_active_t_ = 0;
    unsigned long last_update_t_ = 0;

    bool is_time_to_update() {
      return time_since_last_update() > get_update_period();
    }

    unsigned long time_since_last_update() {
      return millis() - last_update_t_;
    }

    unsigned long get_update_period() {
      return is_active_ ? active_polling_period_ : inactive_polling_period_;
    }

    void update_dist() {
      last_update_t_ = millis();
      dist_ = get_dist_from_sensor();
    }

    float get_dist_from_sensor() {
      return sensor_->dist() / 100.;
    }

    void check_inactive() {
      const unsigned long time_inactive = get_time_inactive();

      if (time_inactive == 0) {
        is_active_ = true;
      } else if (time_inactive >= active_timeout_) {
        is_active_ = false;
      }
    }

    unsigned long get_time_inactive() {
      float dist_diff = abs(dist_ - last_active_dist_);

      bool is_active = dist_diff > active_variance_;

      if (is_active) {
        last_active_dist_ = dist_;
        last_active_t_ = millis();
        return 0;
      }

      return millis() - last_active_t_;
    }
};
