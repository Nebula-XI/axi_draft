#pragma once

#include "info_spi.h"

namespace InSys {

class info_axi_spi_parser : public info_base_parser<info_list, info_axi_spi> {
  void parser(const std::string_view &config) override {
    // TODO: add configuration parser
    m_info_list.emplace_back("SPI", "PORT0", 0x00010000);
    m_info_list.emplace_back("SPI", "PORT1", 0x00020000);
    m_info_list.emplace_back("SPI", "PORT2", 0x00030000);
  }

 public:
  info_axi_spi_parser() = default;
  info_axi_spi_parser(const std::string_view &config) { parser(config); }
};

class info_spi_dev_parser : public info_base_parser<info_list, info_spi_dev> {
  void parser(const std::string_view &config) override {
    // TODO: add configuration parser
    m_info_list.emplace_back("LMX2594", "DD1", 0, 10_MHz,
                             make_info_uid{}("SPI", "PORT0"));
    m_info_list.emplace_back("LTC6953", "DD2", 1, 30_MHz,
                             make_info_uid{}("SPI", "PORT1"));
    m_info_list.emplace_back("LMX2594", "DD3", 2, 10_MHz,
                             make_info_uid{}("SPI", "PORT2"));
  }

 public:
  info_spi_dev_parser() = default;
  info_spi_dev_parser(const std::string_view &config) { parser(config); }
};

class info_spi_parser final : public info_axi_spi_parser,
                              public info_spi_dev_parser {
 public:
  using axi_parser = info_axi_spi_parser;
  using dev_parser = info_spi_dev_parser;

  info_spi_parser(const std::string_view &config) { parser(config); }
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
    // TODO: add configuration parser
    axi_parser::m_info_list.emplace_back("SPI", "PORT0", 0x00010000);
    axi_parser::m_info_list.emplace_back("SPI", "PORT1", 0x00020000);
    axi_parser::m_info_list.emplace_back("SPI", "PORT2", 0x00030000);
    dev_parser::m_info_list.emplace_back("LMX2594", "DD1", 0, 10_MHz,
                                         make_info_uid{}("SPI", "PORT0"));
    dev_parser::m_info_list.emplace_back("LTC6953", "DD2", 1, 30_MHz,
                                         make_info_uid{}("SPI", "PORT1"));
    dev_parser::m_info_list.emplace_back("LMX2594", "DD3", 1, 10_MHz,
                                         make_info_uid{}("SPI", "PORT2"));
  }
};

}  // namespace InSys
