#pragma once

#include "units_info_base.h"

namespace InSys {

/// параметры узла GPIO
struct unit_info_gpio : unit_info_base {
  unit_info_gpio() = delete;  //< контруктор по умолчанию
  unit_info_gpio(const std::string &_name, std::size_t _axi_offset)
      : unit_info_base{_name, _axi_offset} {}  //< размещающий
                                               //конструктор
  inline static const std::string unit{"gpio"};
  std::string to_config() const {
    std::string config{};
    config += unit_info_base::to_config();
    config += "unit = " + unit + "\n";
    return config;
  }
};

/// список параметров узлов GPIO
using units_info_gpio_list = units_info_list<unit_info_gpio>;

/// класс формирователь информации об узлах GPIO
class units_info_gpio
    : public units_info_base<units_info_list, unit_info_gpio> {
  void parser(const std::string config) override {
    // TODO: add configuration parser
    _info_list.emplace_back("GPIO0", 0x00000800);  // FIXME: пример
    _info_list.emplace_back("GPIO1", 0x00001000);  // FIXME: пример
  }

 public:
  /// конструктор
  units_info_gpio(const std::string &config) : units_info_base{config} {
    parser(config);
  }
};

}  // namespace InSys
