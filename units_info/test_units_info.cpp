
#include <iostream>

#include "units_info.h"
#include "units_info_ddr.h"
#include "units_info_gpio.h"
#include "units_info_i2c.h"
#include "units_info_spi.h"

using namespace InSys;
using namespace std::string_literals;

void print_line(char ch = '-', int size = 40) {
  std::cout << std::string(size, ch) << '\n';
}

void print_info_not_found() { std::cout << "unit info is not found" << '\n'; }

void print_info_list_empty() { std::cout << "unit info list is empty" << '\n'; }

void print_info(const unit_info_base &info) {
  std::cout << "name: " << info.name << '\n';
  std::cout << "label: " << info.label << '\n';
  std::cout << "unit: " << info.unit << '\n';
  std::cout << "uid: 0x" << std::hex << info.uid << '\n';
  std::cout << "parent uid: 0x" << std::hex << info.parent_uid << '\n';
}

void print_info(const unit_info_axi_i2c &info) {
  print_info(static_cast<unit_info_base>(info));
  std::cout << "offset: 0x" << std::hex << info.axi_offset << '\n';
}

void print_info(const unit_info_i2c_dev &info) {
  print_info(static_cast<unit_info_base>(info));
  std::cout << "address: 0x" << std::hex << info.address << '\n';
  std::cout << "frequency: " << std::dec << info.frequency << '\n';
}

void print_info(const unit_info_i2c_mux &info) {
  print_info(static_cast<unit_info_i2c_dev_base>(info));
  std::map<uint32_t, unit_info_uid> segments{};
  for (const auto &[uid, port] : info.segments) {
    segments.emplace(port, uid);
  }
  for (const auto &[port, uid] : segments) {
    std::cout << "segment uid[" << std::dec << port << "]: 0x" << std::hex
              << uid << '\n';
  }
}

template <typename unit_info_list_type>
void print_info_list(std::string_view &&desc,
                     const unit_info_list_type &unit_info_list) {
  print_line();
  std::cout << desc << "\n";
  if (unit_info_list.empty()) {
    print_line();
    print_info_list_empty();
  } else
    for (const auto &info : unit_info_list) {
      print_line();
      print_info(info);
    }
}

template <typename unit_info_type>
void print_info(std::string_view &&desc,
                const std::optional<unit_info_type> &unit_info) {
  print_line();
  std::cout << desc << "\n";
  print_line();
  if (!unit_info.has_value()) {
    print_info_not_found();
  } else
    print_info(unit_info.value());
}

static const std::string_view g_config{"TODO: add configuration"};

int main(int argc, char *argv[]) try {
  unit_info_axi_i2c_parser unit_info_axi_i2c_parser{g_config};
  auto unit_info_axi_i2c_list = unit_info_axi_i2c_parser.get_info();
  print_info_list<unit_info_axi_i2c_parser::list_type>(
      "EXAMPLE [1] - get info  axi-i2c units from custom parser",
      unit_info_axi_i2c_list);
  unit_info_i2c_mux_parser unit_info_i2c_mux_parser{g_config};
  auto unit_info_i2c_mux_list = unit_info_i2c_mux_parser.get_info();
  print_info_list<unit_info_i2c_mux_parser::list_type>(
      "EXAMPLE [2] - get info i2c-mux units from custom parser",
      unit_info_i2c_mux_list);
  unit_info_i2c_dev_parser unit_info_i2c_dev_parser{g_config};
  auto unit_info_i2c_dev_list = unit_info_i2c_dev_parser.get_info();
  print_info_list<unit_info_i2c_dev_parser::list_type>(
      "EXAMPLE [3] - get info i2c-dev units from custom parser",
      unit_info_i2c_dev_list);
  unit_info_i2c_parser unit_info_i2c_parser{g_config};
  unit_info_axi_i2c_list =
      unit_info_i2c_parser.get_info<unit_info_i2c_parser::axi_i2c_parser>();
  print_info_list<unit_info_axi_i2c_parser::list_type>(
      "EXAMPLE [4] - get info axi-i2c units from common parser",
      unit_info_axi_i2c_list);
  unit_info_i2c_mux_list =
      unit_info_i2c_parser.get_info<unit_info_i2c_parser::i2c_mux_parser>();
  print_info_list<unit_info_i2c_mux_parser::list_type>(
      "EXAMPLE [5] - get info i2c-mux units from common parser",
      unit_info_i2c_mux_list);
  unit_info_i2c_dev_list =
      unit_info_i2c_parser.get_info<unit_info_i2c_parser::i2c_dev_parser>();
  print_info_list<unit_info_i2c_dev_parser::list_type>(
      "EXAMPLE [6] - get info i2c-dev units from common parser",
      unit_info_i2c_dev_list);
  unit_info_axi_i2c_list = unit_info_axi_i2c_parser.find_by_name("I2C");
  print_info_list<unit_info_axi_i2c_parser::list_type>(
      "EXAMPLE [7] - find info by name for axi-i2c units from custom parser",
      unit_info_axi_i2c_list);
  unit_info_i2c_mux_list = unit_info_i2c_mux_parser.find_by_name("LTC4306");
  print_info_list<unit_info_i2c_mux_parser::list_type>(
      "EXAMPLE [8] - get info by name for i2c-mux units from custom parser",
      unit_info_i2c_mux_list);
  unit_info_i2c_dev_list = unit_info_i2c_dev_parser.find_by_name("INA218");
  print_info_list<unit_info_i2c_dev_parser::list_type>(
      "EXAMPLE [9] - get info by name for i2c-dev units from custom parser",
      unit_info_i2c_dev_list);
  unit_info_axi_i2c_list =
      unit_info_i2c_parser.find_by_name<unit_info_i2c_parser::axi_i2c_parser>(
          "I2C");
  print_info_list<unit_info_axi_i2c_parser::list_type>(
      "EXAMPLE [10] - get info by name for axi-i2c units from common parser",
      unit_info_axi_i2c_list);
  unit_info_i2c_mux_list =
      unit_info_i2c_parser.find_by_name<unit_info_i2c_parser::i2c_mux_parser>(
          "LTC4306");
  print_info_list<unit_info_i2c_mux_parser::list_type>(
      "EXAMPLE [11] - get info by name for i2c-mux units from common parser",
      unit_info_i2c_mux_list);
  unit_info_i2c_dev_list =
      unit_info_i2c_parser.find_by_name<unit_info_i2c_parser::i2c_dev_parser>(
          "INA218");
  print_info_list<unit_info_i2c_dev_parser::list_type>(
      "EXAMPLE [12] - get info by name for i2c-dev units from common parser",
      unit_info_i2c_dev_list);
  auto unit_info_axi_i2c =
      unit_info_i2c_parser.get_by_uid<unit_info_i2c_parser::axi_i2c_parser>(
          0x804c7ca3708aa026);
  print_info(
      "EXAMPLE [13] - get info by uid for axi-i2c units from common parser",
      unit_info_axi_i2c);
  auto unit_info_i2c_mux =
      unit_info_i2c_parser.get_by_uid<unit_info_i2c_parser::i2c_mux_parser>(
          0x6b7d93421e091916);
  print_info(
      "EXAMPLE [14] - get info by uid for i2c-mux units from common parser",
      unit_info_i2c_mux);
  auto unit_info_i2c_dev =
      unit_info_i2c_parser.get_by_uid<unit_info_i2c_parser::i2c_dev_parser>(
          0xa7084de60742fa7d);
  print_info(
      "EXAMPLE [15] - get info by uid for i2c-dev units from common parser",
      unit_info_i2c_dev);
  return EXIT_SUCCESS;
} catch (const std::exception &e) {
  std::cerr << e.what() << std::endl;
  return EXIT_FAILURE;
} catch (...) {
  std::cerr << "unexcepted exception" << std::endl;
  return EXIT_FAILURE;
}
