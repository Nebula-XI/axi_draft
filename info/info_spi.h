#pragma once

#include "info_base.h"

namespace InSys {

class info_axi_spi final : public info_axi_base {
    inline static constexpr auto k_unit { "axi-spi" };

public:
    using list_type = info_list<info_axi_spi>;
    info_axi_spi()
        : info_axi_base { k_unit }
    {
    }
    info_axi_spi(const std::string_view& name, const std::string_view& label,
        uint64_t axi_offset)
        : info_axi_base { name, label, axi_offset, k_unit }
    {
    }
};

class info_spi_dev_base : public info_base {
    uint32_t m_chip_select {};
    double m_frequency {};

public:
    info_spi_dev_base(const std::string_view& unit)
        : info_base { unit }
        , m_chip_select {}
        , m_frequency {}
    {
    }
    info_spi_dev_base(const std::string_view& name, const std::string_view& label,
        uint32_t chip_select, double frequency, info_uid parent_uid,
        const std::string_view& unit)
        : info_base { name, label, parent_uid, unit }
        , m_chip_select { chip_select }
        , m_frequency { frequency }
    {
    }
    auto chip_select() const noexcept { return m_chip_select; }
    auto frequency() const noexcept { return m_frequency; }
};

class info_spi_dev final : public info_spi_dev_base {
    inline static constexpr auto k_unit { "spi-dev" };

public:
    using list_type = info_list<info_spi_dev>;
    info_spi_dev()
        : info_spi_dev_base { k_unit }
    {
    }
    info_spi_dev(const std::string_view& name, const std::string_view& label,
        uint32_t chip_select, double frequency, info_uid parent_uid)
        : info_spi_dev_base { name, label, chip_select,
            frequency, parent_uid, k_unit }
    {
    }
};

} // namespace InSys
