#include <memory>

#include "driver/gpio.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "lvgl.h"

#include "common/macros.hpp"
#include "common/task.hpp"
#include "device/oled_ssd1309.hpp"
#include "ui/encoder_button_input.hpp"
#include "ui/style.hpp"
#include "ui/view/root.hpp"

namespace {
constexpr char TAG[] = "main";
}

std::unique_ptr<OledSsd1309> g_oled;

esp_err_t SetupSpiBus() {
  spi_bus_config_t bus_config = {
      .mosi_io_num = GPIO_NUM_23,
      .miso_io_num = GPIO_NUM_NC,
      .sclk_io_num = GPIO_NUM_18,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = 0,
      .flags = 0,
      .intr_flags = 0,
  };
  TRY(spi_bus_initialize(VSPI_HOST, &bus_config, /*dma*/ VSPI_HOST));
  return ESP_OK;
}

esp_err_t SetupOled() {
  g_oled = OledSsd1309::Create(OledSsd1309::Option{
      .spi = VSPI_HOST,
      .cs_pin = GPIO_NUM_5,
      .dc_pin = GPIO_NUM_19,
  });
  if (!g_oled) {
    return ESP_FAIL;
  }
  TRY(g_oled->Flush());

  return ESP_OK;
}

class UiTask : public Task {
 public:
  virtual ~UiTask() = default;
  using Task::SpawnSame;

  UiTask() = default;

 protected:
  void Run() override {
    root_ = std::make_unique<ui::view::Root>(lv_scr_act());
    lv_indev_set_group(ui::g_buttons_indev, root_->default_group);
    lv_indev_set_group(ui::g_encoder_indev, root_->default_group);

    TickType_t last_ready = xTaskGetTickCount();
    while (1) {
      lv_task_handler();
      root_->Render();
      vTaskDelayUntil(&last_ready, pdMS_TO_TICKS(50));
    }
  }

 private:
  std::unique_ptr<ui::view::Root> root_{};
};

std::unique_ptr<UiTask> g_ui_task;

esp_err_t SetupUiTask() {
  g_ui_task = std::make_unique<UiTask>();
  return g_ui_task ? ESP_OK : ESP_FAIL;
}

esp_err_t PrepareUi() {
  ESP_LOGI(TAG, "%s", __PRETTY_FUNCTION__);
  lv_init();
  TRY(SetupSpiBus());
  TRY(SetupOled());
  TRY(g_oled->RegisterLvglDriver());
  TRY(g_oled->SetDisplayEnabled(true));
  TRY(ui::RegisterLvglInputDrivers());
  TRY(ui::SetupTheme());
  TRY(ui::SetupStyle());
  TRY(SetupUiTask());
  return ESP_OK;
}

extern "C" void app_main(void) {
  puts("\n\n\n\n\n");
  gpio_pad_select_gpio(GPIO_NUM_17);
  gpio_set_direction(GPIO_NUM_17, GPIO_MODE_OUTPUT);
  gpio_pad_select_gpio(GPIO_NUM_16);
  gpio_set_direction(GPIO_NUM_16, GPIO_MODE_OUTPUT);
  CHECK_OK(PrepareUi());
  CHECK_OK(g_ui_task->SpawnSame("ui", 4096, 1));
}
