#pragma once

#include "units_info_base.h"

namespace InSys {

struct unit_info_axi_spi final : unit_info_axi_base {
  using list_type = units_info_list<unit_info_axi_i2c>;
  unit_info_axi_spi(const std::string_view &_name,
                    const std::string_view &_label, unit_info_uid _axi_offset)
      : unit_info_axi_base{_name, _label, _axi_offset, "axi-spi"} {}
};

struct unit_info_spi_dev_base : unit_info_base {
  unit_info_spi_dev_base(const std::string_view &_name,
                         const std::string_view &_label, uint32_t _chip_select,
                         double _frequency, unit_info_uid _parent_uid,
                         const std::string_view &_unit)
      : unit_info_base{_name, _label, _parent_uid, _unit},
        chip_select{_chip_select},
        frequency{_frequency} {}
  const uint32_t chip_select{};
  const double frequency{};
};

struct unit_info_spi_dev final : unit_info_spi_dev_base {
  using list_type = units_info_list<unit_info_spi_dev>;
  unit_info_spi_dev(const std::string_view &_name,
                    const std::string_view &_label, uint32_t _chip_select,
                    double _frequency, unit_info_uid _parent_uid)
      : unit_info_spi_dev_base{_name,      _label,      _chip_select,
                               _frequency, _parent_uid, "spi-dev"} {}
};

class unit_info_axi_spi_parser
    : public units_info_base_parser<units_info_list, unit_info_axi_spi> {
  void parser(const std::string_view &config) override {
    // TODO: add configuration parser
    _info_list.emplace_back("SPI", "PORT0", 0x00010000);
    _info_list.emplace_back("SPI", "PORT1", 0x00020000);
    _info_list.emplace_back("SPI", "PORT2", 0x00030000);
  }

 public:
  unit_info_axi_spi_parser() = default;
  unit_info_axi_spi_parser(const std::string_view &config) { parser(config); }
};

class unit_info_spi_dev_parser
    : public units_info_base_parser<units_info_list, unit_info_spi_dev> {
  void parser(const std::string_view &config) override {
    // TODO: add configuration parser
    _info_list.emplace_back("LMX2594", "DD1", 0, 10_MHz,
                            make_unit_info_uid{}("SPI", "PORT0"));
    _info_list.emplace_back("LTC6953", "DD2", 1, 30_MHz,
                            make_unit_info_uid{}("SPI", "PORT1"));
    _info_list.emplace_back("LMX2594", "DD3", 2, 10_MHz,
                            make_unit_info_uid{}("SPI", "PORT2"));
  }

 public:
  unit_info_spi_dev_parser() = default;
  unit_info_spi_dev_parser(const std::string_view &config) { parser(config); }
};

class unit_info_spi_parser final : public unit_info_axi_spi_parser,
                                   public unit_info_spi_dev_parser {
 public:
  using axi_parser = unit_info_axi_spi_parser;
  using dev_parser = unit_info_spi_dev_parser;

  unit_info_spi_parser(const std::string_view &config) { parser(config); }
  template <typename parser>
  typename parser::list_type get_info() const {
    return parser::get_info();
  }
  template <typename parser>
  typename parser::list_type find_by_name(const std::string_view &name) const {
    return parser::find_by_name(name);
  }
  template <typename parser>
  std::optional<typename parser::value_type> get_by_label(
      const std::string_view &label) const {
    return parser::get_by_label(label);
  }
  template <typename parser>
  std::optional<typename parser::value_type> get_by_uid(
      unit_info_uid uid) const {
    return parser::get_by_uid(uid);
  }

 private:
  void parser(const std::string_view &config) final {
    // TODO: add configuration parser
    axi_parser::_info_list.emplace_back("SPI", "PORT0", 0x00010000);
    axi_parser::_info_list.emplace_back("SPI", "PORT1", 0x00020000);
    axi_parser::_info_list.emplace_back("SPI", "PORT2", 0x00030000);
    dev_parser::_info_list.emplace_back("LMX2594", "DD1", 0, 10_MHz,
                                        make_unit_info_uid{}("SPI", "PORT0"));
    dev_parser::_info_list.emplace_back("LTC6953", "DD2", 1, 30_MHz,
                                        make_unit_info_uid{}("SPI", "PORT1"));
    dev_parser::_info_list.emplace_back("LMX2594", "DD3", 1, 10_MHz,
                                        make_unit_info_uid{}("SPI", "PORT2"));
  }
};

}  // namespace InSys
