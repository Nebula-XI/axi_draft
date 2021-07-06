#pragma once

#include <cstdlib>
#include <iostream>

#include "info_gpio.h"
#include "info_gpio_parser.h"
#include "info_i2c.h"
#include "info_i2c_parser.h"
#include "info_sdram.h"
#include "info_sdram_parser.h"
#include "info_spi.h"
#include "info_spi_parser.h"

namespace InSys {

void print_line(char ch = '-', int size = 40)
{
    std::cout << std::string(size, ch) << '\n';
}

void print_info_not_found() { std::cout << "unit info is not found" << '\n'; }

void print_info_list_empty() { std::cout << "unit info list is empty" << '\n'; }

void print_info(const info_base& info)
{
    std::cout << "name: " << info.name() << '\n';
    std::cout << "label: " << info.label() << '\n';
    std::cout << "unit: " << info.unit() << '\n';
    std::cout << "uid: 0x" << std::hex << info.uid() << '\n';
    std::cout << "parent uid: 0x" << std::hex << info.parent_uid() << '\n';
}

void print_info(const info_axi_base& info)
{
    print_info(static_cast<info_base>(info));
    std::cout << "offset: 0x" << std::hex << info.axi_offset() << '\n';
}

void print_info(const info_i2c_dev& info)
{
    print_info(static_cast<info_base>(info));
    std::cout << "address: 0x" << std::hex << info.address() << '\n';
    std::cout << "frequency: " << std::dec << info.frequency() << '\n';
}

void print_info(const info_i2c_mux& info)
{
    print_info(static_cast<info_i2c_dev_base>(info));
    std::map<uint32_t, info_uid> segments {};
    for (const auto& [uid, port] : info.segments()) {
        segments.emplace(port, uid);
    }
    for (const auto& [port, uid] : segments) {
        std::cout << "segment uid[" << std::dec << port << "]: 0x" << std::hex
                  << uid << '\n';
    }
}

void print_info(const info_spi_dev& info)
{
    print_info(static_cast<info_base>(info));
    std::cout << "chip_select: " << std::dec << info.chip_select() << '\n';
    std::cout << "frequency: " << std::dec << info.frequency() << '\n';
}

template <typename info_list_type>
void print_info_list(std::string_view&& desc, const info_list_type& info_list)
{
    print_line();
    std::cout << desc << "\n";
    if (info_list.empty()) {
        print_line();
        print_info_list_empty();
    } else
        for (const auto& info : info_list) {
            print_line();
            print_info(info);
        }
}

template <typename info_type>
void print_info(std::string_view&& desc, const std::optional<info_type>& info)
{
    print_line();
    std::cout << desc << "\n";
    print_line();
    if (!info) {
        print_info_not_found();
    } else
        print_info(*info);
}

} // namespace InSys
