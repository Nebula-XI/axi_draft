#include "dev_gpio.h"
#include "dev_i2c.h"
#include "dev_sdram.h"
#include "dev_spi.h"
#include "ina21x.h"
#include "test_info.h"

using namespace InSys;

static const std::string_view g_config{"TODO: add configuration"};

int main(int argc, char *argv[]) try {
  info_i2c_parser i2c_info_parser{g_config};
  auto i2c_mux_info_list =
      i2c_info_parser.get_info<info_i2c_parser::mux_parser>();
  auto create_i2c_dev = [&](info_i2c_dev &&i2c_dev_info) {
    // FIXME: этот пример не ищет вложенность мультиплексоров
    print_line();
    print_info(i2c_dev_info);
    info_i2c_mux i2c_mux_info{};
    i2c_segment i2c_mux_segment{};
    bool i2c_mux_is_present{};
    for (const auto &info : i2c_mux_info_list) {
      auto it = info.segments().find(i2c_dev_info.parent_uid());
      if (it != info.segments().cend()) {
        i2c_mux_info = info;
        i2c_mux_segment = it->second;
        i2c_mux_is_present = true;
        break;
      }
    }
    auto i2c_axi_info_opt =
        i2c_info_parser.get_by_uid<info_i2c_parser::axi_parser>(
            (i2c_mux_is_present) ? i2c_mux_info.parent_uid()
                                 : i2c_dev_info.parent_uid());
    if (!i2c_axi_info_opt) {
      print_info_not_found();
      return make_dev<dev_i2c>();
    }
    auto i2c_axi = dev_axi_i2c::create(i2c_axi_info_opt->axi_offset());
    if (i2c_mux_is_present) {
      auto i2c_mux =
          dev_i2c_mux::create(i2c_mux_segment, i2c_axi, i2c_mux_info.address(),
                              i2c_mux_info.frequency());
      return dev_i2c::create(i2c_axi, i2c_mux, i2c_dev_info.address(),
                             i2c_dev_info.frequency());
    } else {
      return dev_i2c::create(i2c_axi, i2c_dev_info.address(),
                             i2c_dev_info.frequency());
    }
  };
  auto i2c_dev_info_list =
      i2c_info_parser.get_info<info_i2c_parser::dev_parser>();
  for (auto &i2c_dev_info : i2c_dev_info_list) {
    auto i2c_dev = create_i2c_dev(std::move(i2c_dev_info));
    if (!i2c_dev) {
      continue;
    }
    auto data = i2c_dev->read();
    auto writed = i2c_dev->write({});
  }
  auto ic_ina21x{chips::ina21x::create(dev_axi_i2c::create(0), 0x49, 100._kHz)};
  auto data = ic_ina21x->read();
  auto writed = ic_ina21x->write({});
  return EXIT_SUCCESS;
} catch (const std::exception &e) {
  std::cerr << e.what() << std::endl;
  return EXIT_FAILURE;
} catch (...) {
  std::cerr << "unexcepted exception" << std::endl;
  return EXIT_FAILURE;
}
