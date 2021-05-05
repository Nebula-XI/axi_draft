
#include "dev_axi.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <fstream>

using namespace InSys;

//-----------------------------------------------------------------------------

dev_axi::dev_axi(const std::string &name) : _name(name), _valid(true)
{
    // TODO: 
    // Открыть устройство
    // Получить конфигурацию
    // Отобразить BAR из драйвера или через mmap(...)
    // Получчить информацию об устройстве (DMA, etc...)
}

//-----------------------------------------------------------------------------

dev_axi::~dev_axi()
{
    // TODO: 
    // Освободить занимаемые ресурсы
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

bool axi_config::write_configuration(const std::string &config_file)
{
    // Устройство не сконфигурировано
    // Запишем файл конфигурации в пространство блока is_main
    std::string config_data;
    if(!process_config_file(config_file, config_data)) {
        return false;
    }

    size_t config_size = config_data.length() + 1;

    // буфер для конфигурационного пространства выровненный на 4 байта
    size_t space_size = ALIGN_UP(config_size, 4);
    std::vector<uint8_t> space_buffer(space_size, 0);
    // скопируем содержимое файла в буфер
    size_t offset = 0;
    for(const auto& v : config_data) {
        space_buffer[offset++] = v;
    }
    space_buffer[offset++] = '\0';

    // перепишем все конфигурационные данные из буфера в BRAM
    auto buffer32 = (uint32_t*)space_buffer.data();
    for(int idx{}; idx < space_buffer.size()/sizeof(uint32_t); ++idx) {
        auto val = buffer32[idx];
        _hw->write_axi(0, idx, val);
    }

    return true;
}

//-----------------------------------------------------------------------------

bool axi_config::save_configuration(const std::string& config_file)
{
    std::string bram_config;
    if(!hw_configuration(bram_config)) {
        fprintf(stderr, "%s, %d: %s():\n Can't find any data in configuration space.\n", __FILE__, __LINE__, __func__);
        return false;
    }

    if(!config_file.empty()) {
        std::ofstream ofs;
        ofs.open(config_file.c_str(), std::ios::out);
        if (!ofs.is_open()) {
            fprintf(stderr, "%s, %d: %s():\n Can't open file for save configuration: %s\n", __FILE__, __LINE__, __func__, config_file.c_str());
            return false;
        }
        ofs << bram_config;
        ofs.close();
    }

    return !bram_config.empty();
}

//-----------------------------------------------------------------------------

bool axi_config::hw_configuration(std::string& bram_config)
{
    char* insys_tag = _hw->axi_region_address<char>();
    if(insys_tag) {
        std::string bram = insys_tag;
        size_t pos = bram.find_first_of('\n');
        std::string tag = bram.substr(0, pos);
        if(tag == "[insys]") {
            fprintf(stderr, "%s(): Found AXI Subsystem on device. TAG: %s\n", __func__, tag.c_str());
            bram_config = bram;
            return true;
        }
    }
    return false;
}

//-----------------------------------------------------------------------------
