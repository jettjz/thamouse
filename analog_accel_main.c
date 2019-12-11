// Analog accelerometer app
//
// Reads data from the ADXL327 analog accelerometer

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "nrfx_gpiote.h"
#include "nrfx_saadc.h"

#include "buckler.h"

// ADC channels
#define X_CHANNEL 0
#define Y_CHANNEL 1
#define Z_CHANNEL 2
#define FINGER_0 3
#define FINGER_1 4
#define FINGER_2 5

// LED is pin 17
#define RED_LED NRF_GPIO_PIN_MAP(0,17)

// callback for SAADC events
void saadc_callback (nrfx_saadc_evt_t const * p_event) {
  // don't care about adc callbacks
}

// sample a particular analog channel in blocking mode
nrf_saadc_value_t sample_value (uint8_t channel) {
  nrf_saadc_value_t val;
  ret_code_t error_code = nrfx_saadc_sample_convert(channel, &val);
  APP_ERROR_CHECK(error_code);
  return val;
}

int main (void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  // initialize GPIO driver
  if (!nrfx_gpiote_is_init()) {
    error_code = nrfx_gpiote_init();
  }
  APP_ERROR_CHECK(error_code);
  // initialize analog to digital converter
  nrfx_saadc_config_t saadc_config = NRFX_SAADC_DEFAULT_CONFIG;
  saadc_config.resolution = NRF_SAADC_RESOLUTION_12BIT;
  error_code = nrfx_saadc_init(&saadc_config, saadc_callback);
  APP_ERROR_CHECK(error_code);

  // initialize analog inputs
  // configure with 0 as input pin for now
  nrf_saadc_channel_config_t channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(0);
  channel_config.gain = NRF_SAADC_GAIN1_6; // input gain of 1/6 Volts/Volt, multiply incoming signal by (1/6)
  channel_config.reference = NRF_SAADC_REFERENCE_INTERNAL; // 0.6 Volt reference, input after gain can be 0 to 0.6 Volts

  // specify input pin and initialize that ADC channel
  channel_config.pin_p = NRF_SAADC_INPUT_AIN3;//BUCKLER_ANALOG_ACCEL_X;
  error_code = nrfx_saadc_channel_init(X_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  // specify input pin and initialize that ADC channel
  channel_config.pin_p = NRF_SAADC_INPUT_AIN4; //BUCKLER_ANALOG_ACCEL_Y;
  error_code = nrfx_saadc_channel_init(Y_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  // specify input pin and initialize that ADC channel
  channel_config.pin_p = NRF_SAADC_INPUT_AIN6; //BUCKLER_ANALOG_ACCEL_Z;
  error_code = nrfx_saadc_channel_init(Z_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  // TODO: Shuould eventually have to make this AIN8 but need to manually configure pin
  channel_config.pin_p = NRF_SAADC_INPUT_AIN5;
  error_code = nrfx_saadc_channel_init(FINGER_0, &channel_config);
  APP_ERROR_CHECK(error_code);

  channel_config.pin_p = NRF_SAADC_INPUT_AIN1; 
  error_code = nrfx_saadc_channel_init(FINGER_1, &channel_config);
  APP_ERROR_CHECK(error_code);

  channel_config.pin_p = NRF_SAADC_INPUT_AIN2; 
  error_code = nrfx_saadc_channel_init(FINGER_2, &channel_config);
  APP_ERROR_CHECK(error_code);

  // initialization complete
  printf("Buckler initialized!\n");
  nrfx_gpiote_out_config_t out_config = NRFX_GPIOTE_CONFIG_OUT_SIMPLE(true);
  error_code = nrfx_gpiote_out_init(RED_LED, &out_config);
  APP_ERROR_CHECK(error_code);
  // loop forever
  while (1) {
    // sample analog inputs
    nrf_saadc_value_t x_val = sample_value(X_CHANNEL);
    nrf_saadc_value_t y_val = sample_value(Y_CHANNEL);
    nrf_saadc_value_t z_val = sample_value(Z_CHANNEL);

    float fsr = 3.6; // = 1*reference / gain; where m = 1 because our range is 0 to Vref
    float lsb = fsr/(pow(2,12)); // 2^N; N = 12 from above
    float x_volt = lsb*x_val;
    float y_volt = lsb*y_val;
    float z_volt = lsb*z_val;

    //TODO Recheck supply voltage
    float supply = 2.8;
    float expected_sensitivity = 300.0*supply/3;
    float expected_bias = 1.5*supply/3;
    float x_g = 1000*(x_volt - expected_bias)/(expected_sensitivity);
    float y_g = 1000*(y_volt - expected_bias)/(expected_sensitivity);
    float z_g = 1000*(z_volt - expected_bias)/(expected_sensitivity);

    float x_tilt = (180/M_PI)*atan(x_g/sqrt(pow(y_g,2) + pow(z_g,2)));
    float y_tilt = (180/M_PI)*atan(y_g/sqrt(pow(x_g,2) + pow(z_g,2)));
    float z_tilt = (180/M_PI)*atan(sqrt(pow(x_g,2) + pow(y_g,2))/z_g);

    // display results
    printf("g-forces\tx: %9.6f\ty: %9.6f\tz:%9.6f\n", x_g, y_g, z_g);
    printf("Tilt\tx: %9.6f\ty: %9.6f\tz:%9.6f\n", x_tilt, y_tilt, z_tilt);
    nrf_saadc_value_t f0 = sample_value(FINGER_0);
    nrf_saadc_value_t f1 = sample_value(FINGER_1);
    nrf_saadc_value_t f2 = sample_value(FINGER_2);


    // Clear LED if f1 (middle finger) pressed
    // Hold LED if f0 (index) is pressed
    // else toggle
    
    if ((uint) f1 > 1500) {
      nrf_gpio_pin_clear(RED_LED);
    } else if ((uint) f0 > 1500) {
      nrf_gpio_pin_set(RED_LED);
    } else {
      nrf_gpio_pin_toggle(RED_LED);
    }

    printf("Finger0:\t%u\tFinger1:\t%uFinger2:\t%u\n", f0, f1, f2);
    nrf_delay_ms(100);

  }
}

