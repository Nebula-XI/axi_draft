#pragma once

#include "units_info_base.h"

namespace InSys {

/// параметры узла SPI
struct unit_info_spi : unit_info_base {
  unit_info_spi() = delete;  //< контруктор по умолчанию
  unit_info_spi(const std::string &_name, const std::string &_label,
                std::size_t _axi_offset, int _chip_select, double _clock)
      : unit_info_base{_name, _label, _axi_offset},
        chip_select{_chip_select},
        clock{_clock} {}    //< размещающий конструктор
  const int chip_select{};  //< выбор кристала
  const double clock{};     //< частота
  inline static const std::string unit{"spi"};
};

/// список параметров узлов SPI
using units_info_spi_list = units_info_list<unit_info_spi>;

/// класс формирователь информации об узлах SPI
class units_info_spi_parser
    : public units_info_base<units_info_list, unit_info_spi> {
  void parser(const std::string config) override {
    // TODO: add configuration parser
    _info_list.emplace_back("LMX2594", "DD1", 0x00000600, 1,
                            10_MHz);  // FIXME: пример
    _info_list.emplace_back("LTC6953", "DD2", 0x00000800, 2,
                            10_MHz);  // FIXME: пример
    _info_list.emplace_back("LMX2594", "DD3", 0x00001200, 3,
                            20_MHz);  // FIXME: пример
  }

 public:
  /// конструктор
  units_info_spi_parser(const std::string &config) : units_info_base{config} {
    parser(config);
  }
};

}  // namespace InSys
