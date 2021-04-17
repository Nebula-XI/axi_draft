#include "test_units_info.h"
#include "units_dev.h"

using namespace InSys;

static const std::string_view g_config{"TODO: add configuration"};

int main(int argc, char *argv[]) try {
  unit_info_i2c_parser info_i2c_parser{g_config};
  auto info_axi_i2c_list =
      info_i2c_parser.get_info<unit_info_i2c_parser::axi_parser>();
  auto info_i2c_mux_list =
      info_i2c_parser.get_info<unit_info_i2c_parser::mux_parser>();
  auto info_i2c_dev_list =
      info_i2c_parser.get_info<unit_info_i2c_parser::dev_parser>();

  auto info_i2c_dev_opt =
      info_i2c_parser.get_by_label<unit_info_i2c_parser::dev_parser>("DD8"); // "DD3"
  if (!info_i2c_dev_opt) {
    print_info_not_found();
    return EXIT_FAILURE;
  }
  auto info_i2c_dev = *info_i2c_dev_opt;
  print_line();
  print_info(info_i2c_dev);
  unit_info_i2c_mux info_i2c_mux{};
  uint32_t segment{};
  bool is_info_i2c_mux{};
  for (const auto &info : info_i2c_mux_list) {
    auto it = info.segments().find(info_i2c_dev.parent_uid());
    if (it != info.segments().cend()) {
      info_i2c_mux = info;
      segment = it->second;
      is_info_i2c_mux = true;
      break;
    }
  }
  unit_info_axi_i2c info_axi_i2c{};
  if (is_info_i2c_mux) {
    print_line();
    print_info(info_i2c_mux);
    info_axi_i2c =
        *info_i2c_parser.get_by_uid<unit_info_i2c_parser::axi_parser>(
            info_i2c_mux.parent_uid());
  } else {
    info_axi_i2c =
        *info_i2c_parser.get_by_uid<unit_info_i2c_parser::axi_parser>(
            info_i2c_dev.parent_uid());
  }
  print_line();
  print_info(info_axi_i2c);
  auto dev_axi_i2c = unit_dev_axi_i2c{info_axi_i2c};
  auto dev_i2c_mux = unit_dev_i2c_mux{info_i2c_mux, segment, dev_axi_i2c};
  auto dev_i2c = unit_dev_i2c{info_i2c_dev, dev_i2c_mux};
  print_line();
  dev_i2c.read();
  dev_i2c.write();

  return EXIT_SUCCESS;
} catch (const std::exception &e) {
  std::cerr << e.what() << std::endl;
  return EXIT_FAILURE;
} catch (...) {
  std::cerr << "unexcepted exception" << std::endl;
  return EXIT_FAILURE;
}
