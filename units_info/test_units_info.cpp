
#include <iostream>

#include "units_info.h"
#include "units_info_ddr.h"
#include "units_info_gpio.h"
#include "units_info_i2c.h"
#include "units_info_spi.h"

using namespace InSys;
using namespace std::string_literals;

void print_info_list_empty(const std::string &unit_name) {
  std::cout << unit_name << " unit list is empty" << '\n';
  std::cout << std::string(32, '-') << '\n';
}

void print_info_not_found(const std::string &unit_name) {
  std::cout << unit_name << " unit not found" << '\n';
  std::cout << std::string(32, '-') << '\n';
}

void print_info(const unit_info_i2c &info) {
  std::cout << "name: " << info.name << '\n';
  std::cout << "label: " << info.label << '\n';
  std::cout << "type: " << info.unit << '\n';
  std::cout << "uid: 0x" << std::hex << info.uid << '\n';
  std::cout << "addr: 0x" << info.address << '\n';
  std::cout << "freq: " << std::dec << info.frequency << '\n';
  std::cout << "offset: 0x" << std::hex << info.axi_offset << '\n';
  if (info.parent_switch.is_present) {
    std::cout << "switch uid: 0x" << std::hex << info.parent_switch.uid << '\n';
    std::cout << "switch port: " << std::dec << info.parent_switch.port << '\n';
  }
  std::cout << std::string(32, '-') << '\n';
}

void print_info(const unit_info_spi &info) {
  std::cout << "name: " << info.name << '\n';
  std::cout << "label: " << info.label << '\n';
  std::cout << "type: " << info.unit << '\n';
  std::cout << "uid: 0x" << std::hex << info.uid << '\n';
  std::cout << "cs: 0x" << info.chip_select << '\n';
  std::cout << "clk: " << std::dec << info.clock << '\n';
  std::cout << "offset: 0x" << std::hex << info.axi_offset << '\n';
  std::cout << std::string(32, '-') << '\n';
}

void print_info(const unit_info_gpio &info) {
  std::cout << "name: " << info.name << '\n';
  std::cout << "label: " << info.label << '\n';
  std::cout << "type: " << info.unit << '\n';
  std::cout << "uid: 0x" << std::hex << info.uid << '\n';
  std::cout << "offset: 0x" << info.axi_offset << '\n';
  std::cout << std::string(32, '-') << '\n';
}

// описание конфигурации
static const std::string g_config{"TODO: add configuration"};

int main(int argc, char *argv[]) try {
  std::cout << std::string(32, '-') << '\n';
  // делаем разбор конфигурации для узлов I2C, SPI, GPIO
  units_info_i2c_parser units_info_i2c{g_config};
  units_info_spi_parser units_info_spi{g_config};
  units_info_gpio_parser units_info_gpio{g_config};
  units_info_ddr_parser units_info_ddr{g_config};

  std::cout << "EXAMPLE [1] - get all I2C units:\n";
  std::cout << std::string(32, '-') << '\n';
  // получаем описание всех узлов I2C
  auto i2c_info_list = units_info_i2c.get_info();
  if (i2c_info_list.empty()) {
    // список узлов пуст
    print_info_list_empty(unit_info_i2c::unit);
  } else
    // обходим все узлы I2C
    for (const auto &info : i2c_info_list) {
      // выводим описание каждого узла I2C в консоль
      print_info(info);
    }

  std::cout << "EXAMPLE [2] - get all SPI units:\n";
  std::cout << std::string(32, '-') << '\n';
  // получаем описание всех узлов SPI
  auto spi_info_list = units_info_spi.get_info();
  if (spi_info_list.empty()) {
    // список узлов пуст
    print_info_list_empty(unit_info_spi::unit);
  } else
    // обходим все узлы SPI
    for (const auto &info : spi_info_list) {
      // выводим описание каждого узла SPI в консоль
      print_info(info);
    }

  std::cout << "EXAMPLE [3] - get all GPIO units:\n";
  std::cout << std::string(32, '-') << '\n';
  // получаем описание всех узлов GPIO
  auto gpio_info_list = units_info_gpio.get_info();
  if (gpio_info_list.empty()) {
    // список узлов пуст
    print_info_list_empty(unit_info_gpio::unit);
  } else
    // обходим все узлы GPIO
    for (const auto &info : gpio_info_list) {
      // выводим описание каждого узла GPIO в консоль
      print_info(info);
    }

  std::cout << "EXAMPLE [4] - get I2C unit by UID:\n";
  std::cout << std::string(32, '-') << '\n';
  // формируем ID для INA218
  auto ina218_uid = units_info_make_uid("INA218", "DD6");
  // поиск всех узлов с соответствующим ID
  auto ina218_info = units_info_i2c.get_info_by_uid(ina218_uid);
  if (ina218_info.has_value()) {
    // выводим описание узла в консоль
    print_info(ina218_info.value());
  } else {
    // узел не найден
    print_info_not_found(unit_info_i2c::unit);
  }

  std::cout << "EXAMPLE [5] - get SPI units by name:\n";
  std::cout << std::string(32, '-') << '\n';
  // поиск всех узлов с соответствующим именем
  auto lmx2594_info_list = units_info_spi.find_info_by_name("LMX2594");
  if (lmx2594_info_list.empty()) {
    // список узлов пуст
    print_info_list_empty(unit_info_spi::unit);
  } else
    // обходим все найденные узлы
    for (const auto &info : lmx2594_info_list) {
      // выводим описание узла в консоль
      print_info(info);
    }

  std::cout << "EXAMPLE [6] - get I2C units by offset:\n";
  std::cout << std::string(32, '-') << '\n';
  // получаем описание I2C узла с заданным оффсетом
  auto i2c_info = units_info_i2c.get_info_by_offset(0x00000100);
  if (i2c_info.has_value()) {
    // выводим описание узла в консоль
    print_info(i2c_info.value());
  } else {
    // узел не найден
    print_info_not_found(unit_info_i2c::unit);
  }

  std::cout << "EXAMPLE [7] - get I2C units behind switch and I2C switch:\n";
  std::cout << std::string(32, '-') << '\n';
  // получаем описание всех узлов I2C которые находятся за switch'ами
  auto i2c_info_behind_switch_list = units_info_i2c.find_info_behind_switch();
  if (i2c_info_behind_switch_list.empty()) {
    // список узлов пуст
    print_info_list_empty(unit_info_i2c::unit);
  } else
    // обходим все узлы I2C находящиеся за switch'ом
    for (const auto &info : i2c_info_behind_switch_list) {
      // выводим описание каждого узла I2C в консоль
      print_info(info);
      // получаем описание узла I2C switch
      auto switch_info = units_info_i2c.get_info_by_uid(info.parent_switch.uid);
      if (switch_info.has_value()) {
        // выводим описание узла I2C switch в консоль
        print_info(switch_info.value());
      } else {
        // узел I2C switch не найден
        print_info_not_found(unit_info_i2c::unit);
      }
    }

  std::cout << "EXAMPLE [8] - any units list:\n";
  std::cout << std::string(32, '-') << '\n';
  // обобщенный список параметров узлов
  units_info_any_list units_info_any_list{};
  // заполняем список параметрами узлов I2C
  for (const auto &info : i2c_info_list) {
    units_info_any_list.emplace_back(info);
  }
  // заполняем список параметрами узлов SPI
  for (const auto &info : spi_info_list) {
    units_info_any_list.emplace_back(info);
  }
  // заполняем список параметрами узлов GPIO
  for (const auto &info : gpio_info_list) {
    units_info_any_list.emplace_back(info);
  }
  if (units_info_any_list.empty()) {
    // обобщенный список узлов пуст
    print_info_list_empty("any");
  } else
    // обходим все узлы в обобщенном списке
    for (const auto &info : units_info_any_list) {
      // выводим описание узла в консоль
      if (info.type() == typeid(unit_info_i2c)) {
        print_info(std::any_cast<unit_info_i2c>(info));
      } else if (info.type() == typeid(unit_info_spi)) {
        print_info(std::any_cast<unit_info_spi>(info));
      }
      if (info.type() == typeid(unit_info_gpio)) {
        print_info(std::any_cast<unit_info_gpio>(info));
      }
    }

  return EXIT_SUCCESS;
} catch (const std::exception &e) {
  std::cerr << e.what() << std::endl;
  return EXIT_FAILURE;
} catch (...) {
  std::cerr << "unexcepted exception" << std::endl;
  return EXIT_FAILURE;
}
