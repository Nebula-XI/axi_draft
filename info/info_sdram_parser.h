#pragma once

#include "info_sdram.h"

namespace InSys {

class sdram_units_tree : public units_tree {
  static constexpr auto k_sdram{"sdram"};

 public:
  using units_tree::units_tree;
  constexpr auto unit_name() const { return k_sdram; }
};
class info_axi_sdram_parser
    : public info_base_parser<info_list, info_axi_sdram> {
 protected:
  void parser(const units_tree &units_tree) override {
    // TODO: add configuration parser
    /*
    m_info_list.emplace_back("SDRAM", "DDR0", 0x00080000);
    m_info_list.emplace_back("SDRAM", "DDR1", 0x00090000);
    */
  }
  void parser(const std::string_view &config) override {
    parser(sdram_units_tree{config}.get_units());
  }

 public:
  info_axi_sdram_parser() = default;
  info_axi_sdram_parser(const std::string_view &config) { parser(config); }
};

class info_sdram_parser final : public info_axi_sdram_parser {
 public:
  using axi_parser = info_axi_sdram_parser;

  info_sdram_parser(const std::string_view &config) { parser(config); }
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
  std::optional<typename parser::value_type> get_by_uid(info_uid uid) const {
    return parser::get_by_uid(uid);
  }

 private:
  void parser(const std::string_view &config) final {
    auto units_tree = gpio_units_tree{config}.get_units();
    axi_parser::parser(units_tree);
    // TODO: add configuration parser
    /*
    axi_parser::m_info_list.emplace_back("SDRAM", "DDR0", 0x00080000);
    axi_parser::m_info_list.emplace_back("SDRAM", "DDR1", 0x00090000);
    */
  }
};

}  // namespace InSys
