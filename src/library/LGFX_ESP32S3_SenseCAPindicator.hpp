#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
//#include <lgfx/v1/touch/Touch_FT5x06.hpp>
#include <driver/i2c.h>

// ESP32でLovyanGFXを独自設定で利用する場合の設定例

/// 独自の設定を行うクラスを、LGFX_Deviceから派生して作成します。
class LGFX : public lgfx::LGFX_Device
{
  //lgfx::Bus_I2C       _bus_instance; 

public:

  lgfx::Bus_RGB      _bus_instance;
  lgfx::Panel_ST7701 _panel_instance;
  lgfx::Light_PWM _light_instance;
  //lgfx::Touch_GT911  _touch_instance;
  lgfx::Touch_FT5x06  _touch_instance;
  

  // コンストラクタを作成し、ここで各種設定を行います。
  // クラス名を変更した場合はコンストラクタも同じ名前を指定してください。
  LGFX(void)
  {
    {
      auto cfg = _panel_instance.config();

      cfg.memory_width  = 480;
      cfg.memory_height = 480;
      cfg.panel_width  = 480;
      cfg.panel_height = 480;

      cfg.offset_x = 0;
      cfg.offset_y = 0;

      _panel_instance.config(cfg);
    }

    {
      auto cfg = _panel_instance.config_detail();

      cfg.use_psram = 1;
      //cfg.pin_cs = 1; //1
      cfg.pin_sclk = 41; //12
      cfg.pin_mosi = 48; //11

      _panel_instance.config_detail(cfg);
    }

    {
      auto cfg = _bus_instance.config();
     
      cfg.panel = &_panel_instance;
      cfg.pin_d0  = GPIO_NUM_15;  // B0
      cfg.pin_d1  = GPIO_NUM_14;  // B1
      cfg.pin_d2  = GPIO_NUM_13; // B2
      cfg.pin_d3  = GPIO_NUM_12; // B3
      cfg.pin_d4  = GPIO_NUM_11;  // B4
      cfg.pin_d5  = GPIO_NUM_10;  // G0
      cfg.pin_d6  = GPIO_NUM_9;  // G1
      cfg.pin_d7  = GPIO_NUM_8; // G2
      cfg.pin_d8  = GPIO_NUM_7; // G3
      cfg.pin_d9  = GPIO_NUM_6; // G4
      cfg.pin_d10 = GPIO_NUM_5;  // G5
      cfg.pin_d11 = GPIO_NUM_4; // R0
      cfg.pin_d12 = GPIO_NUM_3; // R1
      cfg.pin_d13 = GPIO_NUM_2; // R2
      cfg.pin_d14 = GPIO_NUM_1; // R3
      cfg.pin_d15 = GPIO_NUM_0; // R4

      cfg.pin_henable = GPIO_NUM_18; //45
      cfg.pin_vsync   = GPIO_NUM_17; //4
      cfg.pin_hsync   = GPIO_NUM_16; //5
      cfg.pin_pclk    = GPIO_NUM_21; //21
      cfg.freq_write  = 14000000;
      //cfg.freq_write  = 14000000;

      cfg.hsync_polarity    = 0;
      cfg.hsync_front_porch = 10;
      cfg.hsync_pulse_width = 8;
      cfg.hsync_back_porch  = 50;
      cfg.vsync_polarity    = 0;
      cfg.vsync_front_porch = 10;
      cfg.vsync_pulse_width = 8;
      cfg.vsync_back_porch  = 20;
      cfg.pclk_idle_high    = 0;
      cfg.de_idle_high      = 1;
      
      _bus_instance.config(cfg);
    }
    _panel_instance.setBus(&_bus_instance);

    {
      
      auto cfg = _touch_instance.config();
      cfg.x_min      = 0;
      cfg.x_max      = 480;
      cfg.y_min      = 0;
      cfg.y_max      = 480;
      //cfg.pin_int = -1; //追加
      cfg.bus_shared = false; //false
      cfg.offset_rotation = 0;

      cfg.i2c_port   = I2C_NUM_1; //NUM_1
      cfg.i2c_addr = 0x48; //20,48
      cfg.pin_int    = GPIO_NUM_NC;
      cfg.pin_sda    = GPIO_NUM_39;
      cfg.pin_scl    = GPIO_NUM_40;
      cfg.pin_rst    = GPIO_NUM_NC; //38
      
      cfg.freq       = 200000; //400000
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
      
    }

     {
       auto cfg = _light_instance.config();
       cfg.pin_bl = GPIO_NUM_45;
       _light_instance.config(cfg);
     }
     _panel_instance.light(&_light_instance);

    setPanel(&_panel_instance);
  }
};

