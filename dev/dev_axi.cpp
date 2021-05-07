
#include "dev_axi.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <fstream>
#include <fcntl.h>
#ifdef __linux__
#include <unistd.h>
#endif

using namespace InSys;

//-----------------------------------------------------------------------------

dev_axi::dev_axi(const std::string &name) : _name(name), _valid(true)
{
    try {
#ifdef __linux__
		_fd = open(_name.c_str(), O_RDWR, 0666);
		if (_fd < 0) {
			throw std::string("Error open device!\n");
		}
        _mm.init(_fd);
#else
		_fd = CreateFile(devname.c_str(), GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (_fd == INVALID_HANDLE_VALUE) {
			throw std::string("Error open device!\n");
		}
#endif
        struct bar_info_t bi;
	    if (!dev_ioctl(BAR_INFO, nullptr, 0, &bi, sizeof(struct bar_info_t))) {
			throw std::string("Error in dev_ioctl()!\n");
        }

        size_t *_bar{nullptr};
        size_t _sz{0ULL};

		for(int ii=0; ii<MAX_BAR; ii++) {
            if(!bi.pa[ii]) continue;
#ifdef __linux__
			_bar = (size_t*)(_mm.map(bi.pa[ii], bi.sz[ii]));
#else
			_bar = (size_t*)(bi.va[ii]);
#endif
			if(!_bar) {
                throw std::string("Can't map BAR into application.\n");
            }
            _sz = bi.sz[ii];

            fprintf(stderr, "%s(): 0x%lx --> %p : [0x%lx]\n", __func__, bi.pa[ii], _bar, _sz);

            if(is_insys_tag(_bar)) {
                _axi_bar = _bar;
                _axi_bar_size = _sz;
                continue;
            }

            uint32_t reg_val = _bar[0];
            if(((reg_val >> 20) & 0xfff) == 0x1fC) {
                fprintf("%s(): Found DMA Subsystem for PCIe on BAR%d. ID: 0x%x\n", __func__, ii, reg_val>>20 );
                _xdma_bar = _bar;
                _xdma_bar_size = _sz;
            }
        }

        if(_axi_bar_size && _xdma_bar) {
            _valid = true;
        }

    } catch(...) {
        //TODO: Free rsources and throw exception to upper layers
    }
}

//-----------------------------------------------------------------------------

dev_axi::~dev_axi()
{
    if(_fd > 0) {
#ifdef __linux__
		close(_fd);
#else
		CloseHandle(_fd);
#endif
    }
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
