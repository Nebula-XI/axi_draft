
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
  std::cout << "unit name: " << info.name << '\n';
  std::cout << "unit label: " << info.label << '\n';
  std::cout << "unit type: " << info.unit << '\n';
  std::cout << "unit id: 0x" << std::hex << info.id << '\n';
  std::cout << "unit address: 0x" << info.address << '\n';
  std::cout << "unit frequency: " << std::dec << info.frequency << '\n';
  std::cout << "unit axi offset: 0x" << std::hex << info.axi_offset << '\n';
  std::cout << std::string(32, '-') << '\n';
}

void print_info(const unit_info_spi &info) {
  std::cout << "unit name: " << info.name << '\n';
  std::cout << "unit label: " << info.label << '\n';
  std::cout << "unit type: " << info.unit << '\n';
  std::cout << "unit id: 0x" << std::hex << info.id << '\n';
  std::cout << "unit chip select: 0x" << info.chip_select << '\n';
  std::cout << "unit clock: " << std::dec << info.clock << '\n';
  std::cout << "unit axi offset: 0x" << std::hex << info.axi_offset << '\n';
  std::cout << std::string(32, '-') << '\n';
}

void print_info(const unit_info_gpio &info) {
  std::cout << "unit name: " << info.name << '\n';
  std::cout << "unit label: " << info.label << '\n';
  std::cout << "unit type: " << info.unit << '\n';
  std::cout << "unit id: 0x" << std::hex << info.id << '\n';
  std::cout << "unit axi offset: 0x" << info.axi_offset << '\n';
  std::cout << std::string(32, '-') << '\n';
}

// описание конфигурации
static const std::string g_config{
    "[I2C]\nunits = INA218_0, INA218_1, LTC2991_0\n\n[SPI]\nunits = LMX2594_0, "
    "LTC6953_0, LMX2594_1\n\n[GPIO]\nunits = GPIO_0, "
    "GPIO_1\n\n[INA218_0]\nname = INA218\nid = 0xe0d116e870d116c1\naxi_offset "
    "= 0x100\nunit = i2c\naddr = 0x32\nfreq = 200000\n\n[INA218_1]\nname = "
    "INA218\nid = 0xe0d116e870d116c1\naxi_offset = 0x200\nunit = i2c\naddr = "
    "0x48\nfreq = 200000\n\n[LTC2991_0]\nname = LTC2991\nid = "
    "0x62ef2330732ddc24\naxi_offset = 0x400\nunit = i2c\naddr = 0x53\nfreq = "
    "200000\n\n[LMX2594_0]\nname = LMX2594\nid = "
    "0xffa287061f44f906\naxi_offset = 0x600\nunit = spi\ncs = 0x1\nclk = "
    "1e+07\n\n[LTC6953_0]\nname = LTC6953\nid = 0x624d49ef1349a8b0\naxi_offset "
    "= 0x800\nunit = spi\ncs = 0x2\nclk = 1e+07\n\n[LMX2594_1]\nname = "
    "LMX2594\nid = 0xffa287061f44f906\naxi_offset = 0x1200\nunit = spi\ncs = "
    "0x3\nclk = 2e+07\n\n[GPIO_0]\nname = GPIO0\nid = "
    "0x14b48f04c3c3c704\naxi_offset = 0x800\nunit = gpio\n\n[GPIO_1]\nname = "
    "GPIO1\nid = 0x619ca3c738df1080\naxi_offset = 0x1000\nunit = gpio\n"};

int main(int argc, char *argv[]) try {
  std::cout << std::string(32, '-') << '\n';
  // делаем разбор конфигурации для узлов I2C, SPI, GPIO
  units_info_i2c_parser units_info_i2c{g_config};
  units_info_spi_parser units_info_spi{g_config};
  units_info_gpio_parser units_info_gpio{g_config};
  units_info_ddr_parser units_info_ddr{g_config};

  std::cout << "EXAMPLE [1] - get all I2C units:\n";
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

  std::cout << "EXAMPLE [4] - get I2C units by ID:\n";
  // формируем ID для INA218
  auto ina218_id = units_info_make_id("INA218", "DD6");
  // поиск всех узлов с соответствующим ID
  auto ina218_info_list = units_info_i2c.find_info(ina218_id);
  if (ina218_info_list.empty()) {
    // список узлов пуст
    print_info_list_empty(unit_info_i2c::unit);
  } else
    // обходим все найденные узлы
    for (const auto &info : ina218_info_list) {
      // выводим описание узла в консоль
      print_info(info);
    }

  std::cout << "EXAMPLE [5] - get SPI units by name:\n";
  // поиск всех узлов с соответствующим именем
  auto lmx2594_info_list = units_info_spi.find_info("LMX2594");
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
  // получаем описание I2C узла с заданным оффсетом
  auto i2c_info = units_info_i2c.get_info(0x00000100);
  if (i2c_info.has_value()) {
    // выводим описание узла в консоль
    print_info(i2c_info.value());
  } else {
    // узел не найден
    print_info_not_found(unit_info_i2c::unit);
  }

  std::cout << "EXAMPLE [7] - any units list:\n";
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
