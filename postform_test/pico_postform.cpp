#include <array>
#include <span>

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "postform/logger.h"
#include "postform/rtt/rtt.h"
#include "postform/rtt/transport.h"
#include "postform/serial_logger.h"

constexpr static size_t RTT_CHANNEL_SIZE = 1024u;
static std::array<uint8_t, RTT_CHANNEL_SIZE> rtt_buffer;

[[gnu::used]] Postform::Rtt::ControlBlock<1u, 0u> _SEGGER_RTT{
    std::array<Postform::Rtt::ChannelDescriptor, 1u>{
        Postform::Rtt::ChannelDescriptor{
            .name = "postform_channel",
            .buffer{rtt_buffer},
        },
    },
    {},
};

static Postform::Rtt::Transport transport{&_SEGGER_RTT.up_channels[0u]};
Postform::SerialLogger<Postform::Rtt::Transport> logger{&transport};

namespace Ditto {

void assert_failed(const char* condition, int line, const char* file) {
  LOG_ERROR(&logger,
            "ASSERT FAILED! "
            "Condition `%s` failed in file `%s`, line %d",
            condition, file, line);
  while (true) {
    // Remember that infinite loops without side-effects are UB! (and normally
    // are plainly removed...)
    volatile int a = 0;
    static_cast<void>(a);
  }
}

}  // namespace Ditto

int main() {
  LOG_INFO(&logger, "Hello world!");
  if (cyw43_arch_init()) {
    LOG_ERROR(&logger, "Wi-Fi init failed");
    return -1;
  }
  while (true) {
    LOG_INFO(&logger, "Led ON!");
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    sleep_ms(250);
    LOG_INFO(&logger, "Led OFF!");
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    sleep_ms(250);
  }

  return 0;
}
