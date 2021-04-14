#pragma once

#include "units_info_base.h"

namespace InSys {

/// параметры узла DDR
struct unit_info_ddr : unit_info_base {
  unit_info_ddr() = delete;  //< контруктор по умолчанию
  unit_info_ddr(const std::string &_name, std::size_t _axi_offset)
      : unit_info_base{_name, _axi_offset} {}  //< размещающий
                                               //конструктор
  inline static const std::string unit{"ddr"};
};

/// список параметров узлов DDR
using units_info_ddr_list = units_info_list<unit_info_ddr>;

/// класс формирователь информации об узлах DDR
class units_info_ddr_parser
    : public units_info_base<units_info_list, unit_info_ddr> {
  void parser(const std::string config) override {
    // TODO: add configuration parser
  }

 public:
  /// конструктор
  units_info_ddr_parser(const std::string &config) : units_info_base{config} {
    parser(config);
  }
};

}  // namespace InSys
