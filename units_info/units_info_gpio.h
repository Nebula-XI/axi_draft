#pragma once

#include "units_info.h"

namespace InSys {

/// параметры узла GPIO
struct unit_info_gpio : unit_info_base {
  unit_info_gpio() = delete;  //< контруктор по умолчанию
  unit_info_gpio(const std::string &_name, std::size_t _axi_offset)
      : unit_info_base{_name, _axi_offset} {}  //< размещающий
                                               //конструктор
  inline static const char unit[]{"gpio"};
};

/// список параметров узлов GPIO
using units_info_gpio_list = std::vector<unit_info_gpio>;

/// класс формирователь информации об устройствах
class units_info_gpio final {
  units_info_gpio_list _info_list{};

 public:
  /// конструктор
  units_info_gpio(const std::string &config) {
    // TODO: add configuration parser
    _info_list.emplace_back("GPIO0", 0x00000800);  // FIXME: пример
    _info_list.emplace_back("GPIO1", 0x00001000);  // FIXME: пример
  }
  units_info_gpio_list get_info() const { return _info_list; }
  units_info_gpio_list find_info(std::size_t id) const {
    units_info_gpio_list info_list{};
    for (const auto &info : _info_list) {
      if (info.id != id) {
        continue;
      }
      info_list.push_back(info);
    }
    return info_list;
  }
};

}  // namespace InSys
