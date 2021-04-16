
#include "test_units_info.h"

using namespace InSys;
using namespace std::string_literals;

static const std::string_view g_config{"TODO: add configuration"};

int main(int argc, char *argv[]) try {
  unit_info_axi_i2c_parser info_axi_i2c_parser{g_config};
  auto info_axi_i2c_list = info_axi_i2c_parser.get_info();
  print_info_list<unit_info_axi_i2c_parser::list_type>(
      "EXAMPLE [1] - get info  axi-i2c units from custom parser",
      info_axi_i2c_list);
  unit_info_i2c_mux_parser info_i2c_mux_parser{g_config};
  auto info_i2c_mux_list = info_i2c_mux_parser.get_info();
  print_info_list<unit_info_i2c_mux_parser::list_type>(
      "EXAMPLE [2] - get info i2c-mux units from custom parser",
      info_i2c_mux_list);
  unit_info_i2c_dev_parser info_i2c_dev_parser{g_config};
  auto info_i2c_dev_list = info_i2c_dev_parser.get_info();
  print_info_list<unit_info_i2c_dev_parser::list_type>(
      "EXAMPLE [3] - get info i2c-dev units from custom parser",
      info_i2c_dev_list);
  unit_info_i2c_parser info_i2c_parser{g_config};
  info_axi_i2c_list =
      info_i2c_parser.get_info<unit_info_i2c_parser::axi_parser>();
  print_info_list<unit_info_axi_i2c_parser::list_type>(
      "EXAMPLE [4] - get info axi-i2c units from common parser",
      info_axi_i2c_list);
  info_i2c_mux_list =
      info_i2c_parser.get_info<unit_info_i2c_parser::mux_parser>();
  print_info_list<unit_info_i2c_mux_parser::list_type>(
      "EXAMPLE [5] - get info i2c-mux units from common parser",
      info_i2c_mux_list);
  info_i2c_dev_list =
      info_i2c_parser.get_info<unit_info_i2c_parser::dev_parser>();
  print_info_list<unit_info_i2c_dev_parser::list_type>(
      "EXAMPLE [6] - get info i2c-dev units from common parser",
      info_i2c_dev_list);
  info_axi_i2c_list = info_axi_i2c_parser.find_by_name("I2C");
  print_info_list<unit_info_axi_i2c_parser::list_type>(
      "EXAMPLE [7] - find info by name for axi-i2c units from custom parser",
      info_axi_i2c_list);
  info_i2c_mux_list = info_i2c_mux_parser.find_by_name("LTC4306");
  print_info_list<unit_info_i2c_mux_parser::list_type>(
      "EXAMPLE [8] - get info by name for i2c-mux units from custom parser",
      info_i2c_mux_list);
  info_i2c_dev_list = info_i2c_dev_parser.find_by_name("INA218");
  print_info_list<unit_info_i2c_dev_parser::list_type>(
      "EXAMPLE [9] - get info by name for i2c-dev units from custom parser",
      info_i2c_dev_list);
  info_axi_i2c_list =
      info_i2c_parser.find_by_name<unit_info_i2c_parser::axi_parser>("I2C");
  print_info_list<unit_info_axi_i2c_parser::list_type>(
      "EXAMPLE [10] - get info by name for axi-i2c units from common parser",
      info_axi_i2c_list);
  info_i2c_mux_list =
      info_i2c_parser.find_by_name<unit_info_i2c_parser::mux_parser>("LTC4306");
  print_info_list<unit_info_i2c_mux_parser::list_type>(
      "EXAMPLE [11] - get info by name for i2c-mux units from common parser",
      info_i2c_mux_list);
  info_i2c_dev_list =
      info_i2c_parser.find_by_name<unit_info_i2c_parser::dev_parser>("INA218");
  print_info_list<unit_info_i2c_dev_parser::list_type>(
      "EXAMPLE [12] - get info by name for i2c-dev units from common parser",
      info_i2c_dev_list);
  auto info_axi_i2c =
      info_i2c_parser.get_by_uid<unit_info_i2c_parser::axi_parser>(
          0x804c7ca3708aa026);
  print_info(
      "EXAMPLE [13] - get info by uid for axi-i2c units from common parser",
      info_axi_i2c);
  auto info_i2c_mux =
      info_i2c_parser.get_by_uid<unit_info_i2c_parser::mux_parser>(
          0x6b7d93421e091916);
  print_info(
      "EXAMPLE [14] - get info by uid for i2c-mux units from common parser",
      info_i2c_mux);
  auto info_i2c_dev =
      info_i2c_parser.get_by_uid<unit_info_i2c_parser::dev_parser>(
          0xa7084de60742fa7d);
  print_info(
      "EXAMPLE [15] - get info by uid for i2c-dev units from common parser",
      info_i2c_dev);
  auto info_i2c_dev_by_label =
      info_i2c_parser.get_by_label<unit_info_i2c_parser::dev_parser>("DD1");
  print_info(
      "EXAMPLE [16] - get info by label for i2c-dev units from common parser",
      info_i2c_dev_by_label);
  unit_info_axi_spi_parser info_axi_spi_parser{g_config};
  auto info_axi_spi_list = info_axi_spi_parser.get_info();
  print_info_list<unit_info_axi_spi_parser::list_type>(
      "EXAMPLE [17] - get info axi-spi units from custom parser",
      info_axi_spi_list);
  unit_info_spi_dev_parser info_spi_dev_parser{g_config};
  auto info_spi_dev_list = info_spi_dev_parser.get_info();
  print_info_list<unit_info_spi_dev_parser::list_type>(
      "EXAMPLE [18] - get info api-dev units from custom parser",
      info_spi_dev_list);
  unit_info_spi_parser info_spi_parser{g_config};
  info_axi_spi_list =
      info_spi_parser.get_info<unit_info_spi_parser::axi_parser>();
  print_info_list<unit_info_axi_spi_parser::list_type>(
      "EXAMPLE [19] - get info axi-spi units from common parser",
      info_axi_spi_list);
  info_spi_dev_list =
      info_spi_parser.get_info<unit_info_spi_parser::dev_parser>();
  print_info_list<unit_info_spi_dev_parser::list_type>(
      "EXAMPLE [20] - get info spi-dev units from common parser",
      info_spi_dev_list);
  unit_info_axi_gpio_parser info_axi_gpio_parser{g_config};
  auto info_axi_gpio_list = info_axi_gpio_parser.get_info();
  print_info_list<unit_info_axi_gpio_parser::list_type>(
      "EXAMPLE [21] - get info axi-gpio units from custom parser",
      info_axi_gpio_list);
  unit_info_gpio_parser info_gpio_parser{g_config};
  info_axi_gpio_list =
      info_gpio_parser.get_info<unit_info_gpio_parser::axi_parser>();
  print_info_list<unit_info_axi_gpio_parser::list_type>(
      "EXAMPLE [22] - get info axi-gpio units from common parser",
      info_axi_gpio_list);
  unit_info_axi_sdram_parser info_axi_sdram_parser{g_config};
  auto info_axi_sdram_list = info_axi_sdram_parser.get_info();
  print_info_list<unit_info_axi_sdram_parser::list_type>(
      "EXAMPLE [23] - get info axi-sdram units from custom parser",
      info_axi_sdram_list);
  unit_info_sdram_parser info_sdram_parser{g_config};
  info_axi_sdram_list =
      info_sdram_parser.get_info<unit_info_sdram_parser::axi_parser>();
  print_info_list<unit_info_axi_sdram_parser::list_type>(
      "EXAMPLE [24] - get info axi-sdram units from common parser",
      info_axi_sdram_list);
  return EXIT_SUCCESS;
} catch (const std::exception &e) {
  std::cerr << e.what() << std::endl;
  return EXIT_FAILURE;
} catch (...) {
  std::cerr << "unexcepted exception" << std::endl;
  return EXIT_FAILURE;
}
