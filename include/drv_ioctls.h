/*!
	\file
	\brief the common declarations shared by driver and user applications
	\author Dorokhin Andrey
	\version 1.2
	\date Dec 2020
*/

#ifndef _FMCPEX_API_H_
 #define _FMCPEX_API_H_

#define DEVICE_NAME "fmcpex"
#define _DRVNAME_ "fmcpex"

#define MAX_DEVICE      	8
#define MAX_STRING_LEN		255
#define MAX_BAR				6
#define PCIE_BAR_AXI 		0xA
#define PCIE_BAR_XDMA 		0xB

#define MAX_XDMA_CHANNEL 	4
#define MAX_USER_IRQ     	16

#define XDMA_ID_H2C      	0x1FC0
#define XDMA_ID_C2H      	0x1FC1


#ifdef __linux__
#include <linux/types.h>
#ifndef __KERNEL__
#include <sys/ioctl.h>
#include <cstdint>
#include <cstddef>
#endif

#define DEVICE_TYPE             'f'
#define MAKE_IOCTL(c) _IO(DEVICE_TYPE, (c))

#else

#ifdef WDFAPI
typedef signed	 int int32_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
#else
#include <stdint.h>
#endif

#define PEX_DEVICE_TYPE             0x8000
#define MAKE_IOCTL(c)\
		(ULONG)CTL_CODE(PEX_DEVICE_TYPE, 0x800+(c), METHOD_BUFFERED, FILE_ANY_ACCESS)

#endif

//-----------------------------------------------------------------------------
// Коды команд управления устройством 
#define GET_VERSION					MAKE_IOCTL(0x00)
#define BAR_INFO					MAKE_IOCTL(0x01)
#define SET_BUS_CONFIG				MAKE_IOCTL(0x02)
#define GET_BUS_CONFIG				MAKE_IOCTL(0x03)
#define GET_LOCATION				MAKE_IOCTL(0x05)

#define DMA_BUFFER_LOCK				MAKE_IOCTL(0x10)
#define DMA_BUFFER_START            MAKE_IOCTL(0x11)
#define DMA_BUFFER_STOP             MAKE_IOCTL(0x12)
#define DMA_BUFFER_UNLOCK           MAKE_IOCTL(0x13)
#define DMA_BLOCK_WAIT              MAKE_IOCTL(0x14)
#define DMA_BUFFER_WAIT             MAKE_IOCTL(0x15)
#define DMA_STATE				    MAKE_IOCTL(0x16)

#define USR_IRQ_ENABLE              MAKE_IOCTL(0x20)
#define USR_IRQ_DISABLE             MAKE_IOCTL(0x21)
#define USR_IRQ_WAIT                MAKE_IOCTL(0x22)

//-----------------------------------------------------------------------------
// Идентификаторы поддерживаемых устройств
#define INSYS_VENDOR_ID		0x4953
#define XLINX_VENDOR_ID		0x10ee

#define ALPHA_DEVID			0x8038 // DeviceID for AlphaData
#define FMC126P_DEVID		0xA522 // DeviceID for FMC126P
#define FMC107X_DEVID		0x1075 // DeviceID for FMC107XILINX 

#pragma pack(push,1)

#ifdef __linux__

//! Описывает параметры для команд управления устройством
struct ioctl_param {
    void *srcBuf;		//!< буфер с данными для устройства (через него передаются данные В драйвер нулевого кольца)
    size_t srcSize;		//!< размер буфера с данными для устройства
    void *dstBuf;		//!< буфер с данными от устройства  (через него передаются данные ИЗ драйвера нулевого кольца)
    size_t dstSize;		//!< dstSize - размер буфера с данными от устройства
}__attribute__((packed));

#endif

//! set/get data structure for IOCTL_SET_BUS_CONFIG / IOCTL_GET_BUS_CONFIG
typedef struct {
	void*	 pBuf;		//!< OUT
	uint32_t bufSize;	//!< IN
	uint32_t offset;	//!< IN
} DATA_BUS_CFG;

//! Device location structure for IOCTL_GET_LOCATION
typedef struct {
	uint16_t busnum;	//!< OUT PCI bus number
	uint16_t devnum;	//!< OUT PCI device number 
} DATA_LOCATION;


//-----------------------------------------------------------------------------
/*!
	 \brief Структура ресурсов PCI/PCIe устройства.

	  Структура описывает все базовые адреса устройства на шине:
	  физический адрес BARx и диапазон занимаемый соответветствующим
	  BARx. Если, ресурсы не выделены, то поля элементов равны 0.
 */
struct bar_info_t {
	size_t        pa[MAX_BAR];   //! Физический адрес BARx
	void*		  va[MAX_BAR];	 //! виртуальный адрес BARx
	size_t        sz[MAX_BAR];   //! Размер адресного пространства BARx
	size_t        fl[MAX_BAR];   //! Флаги адресного пространства BARx
};

//! Идентификаторы адресного пространства XDMA
enum xdma_target_id {
	TARGET_H2C_CHAN		= 0x0,
	TARGET_C2H_CHAN		= 0x1,
	TARGET_IRQ_BLOCK	= 0x2,
	TARGET_CONFIG		= 0x3,
	TARGET_H2C_SGDMA	= 0x4,
	TARGET_C2H_SGDMA	= 0x5,
	TARGET_SGDMA_COMMON = 0x6,
	TARGET_MSIX			= 0x8,
};

//! Идентификаторы направления передачи данных каналов DMA.
enum chan_dir {
	DIR_H2C = 0,    //! Direction Host to Card
	DIR_C2H,        //! Direction Card to Host
};

//-----------------------------------------------------------------------------
/*!
	 \brief Идентификаторы номеров каналов DMA.
 */
enum chan_id { CHAN0 = 0, CHAN1 = 1, CHAN2 = 2, CHAN3 = 3, };

//-----------------------------------------------------------------------------
/*!
	\brief Тип памяти используемой каналом DMA устройства.

	KERNEL_MEMORY_DMA - выделяется драйвером. Физически непрервына. Малое число дескрипторов
	для получения буферов. Большая скорость передачи. Ограниченный размер.
	USER_MEMORY_DMA - выделяется пользователем. Физически разрывна на блоки 4К (PAGE_SIZE).
	Большое число дескрипторов для получения буфера. Меньшая скорость передачи.
	Почти неограниченный размер.
 */
enum dma_memory_type {
	KERNEL_MEMORY_DMA = 1,  //! Каналом используется память выделенная драйвером.
	USER_MEMORY_DMA = 2,    //! Каналом используется память выделенная пользователем.
	BOOT_MEMORY_DMA = 3,    //! boot time memory allocation
	//PHYS_MEMORY_TYPE = 8
};

//-----------------------------------------------------------------------------
/*!
	 \brief Структура для размещения/закрепления памяти для канала DMA.

	  Указывается направление передачи данных, номер канала,
	  число блоков и размер одного блока, для формирования составного буфера.
 */
struct dma_alloc_t {
	uint32_t direction;      //!< DMA channel direction H2C or C2H
	uint32_t channel;        //!< DMA channel number 0, 1, 2, 3
	uint32_t mem_type;		//!< memory type of DMA buffer
	uint32_t block_count;    //!< Number of blocks into DMA buffer
	size_t	block_size;     //!< Size of block (in bytes)
	void	*pStub;			//!< Address of memory area with information of DMA channel state
//	void*	pWb_result;		//!< Address of memory area with DMA write-back information (for C2H)
#ifdef _WIN32
	HANDLE	hBlockEndEvent;	//!< event of the end of each block of DMA buffer
	HANDLE	hTransEndEvent;	//!< event of the end of all DMA buffer
#endif
	void*  pBlocks[1];		//! Addresses of blocks for DMA
};

//-----------------------------------------------------------------------------
/*!
	 \brief Структура для запуска канала DMA.

	  Указывается направление передачи данных, номер канала и режим работы.
 */
struct dma_start_t {
	uint16_t direction;       //!< DMA channel direction H2C or C2H
	uint16_t channel;         //!< DMA channel number 0, 1, 2, 3
	uint16_t cycle;           //!< DMA channel work mode cycle = 1 - continues mode
};

//-----------------------------------------------------------------------------
/*!
	 \brief Структура для остановка канала DMA.

	  Указывается направление передачи данных и номер канала.
 */
struct dma_stop_t {
	uint16_t direction;       //!< DMA channel direction H2C or C2H
	uint16_t channel;         //!< DMA channel number 0, 1, 2, 3
};

//-----------------------------------------------------------------------------
/*!
	 \brief Структура для освобождения канала DMA.

	  Указывается направление передачи данных и номер канала.
 */
struct dma_free_t {
	uint16_t direction;       //!< DMA channel direction H2C or C2H
	uint16_t channel;         //!< DMA channel number 0, 1, 2, 3
};

//-----------------------------------------------------------------------------
/*!
	 \brief Структура для ожидания заполнения блока данных каналом DMA.

	  После старта, можно ожидать заполнения блока данных, соответствующим
	  каналом. При этом поток ожидания не будет потреблять ресурсы процессора.
 */
struct dma_wait_block_t {
	uint16_t	direction;				//! IN DMA channel direction H2C or C2H
	uint16_t	channel;				//! IN DMA channel number 0, 1, 2, 3
	int32_t		timeout;				//! IN Timeout of operation
	int32_t		last_block_number;		//! OUT	Number of last completed block
	uint32_t	completed_block_counter;//! OUT	Total Counter of all completed blocks
	uint32_t	completed_desc_counter; //! OUT	DMA completed descriptors counter
	uint32_t	status;					//! OUT	DMA channel state
};

//-----------------------------------------------------------------------------
/*!
	 \brief Структура для получения состояния канала DMA.

	  В этой структуре возвращается состояние указанного канала DMA.
 */
struct dma_state_t {
	uint16_t	direction;					//! IN DMA channel direction H2C or C2H
	uint16_t	channel;					//! IN DMA channel number 0, 1, 2, 3
	int32_t		last_block_number;			//! OUT last completed block number
	uint32_t	completed_block_counter;	//! OUT total blocks counter
	uint32_t	completed_desc_counter;		//! OUT	completed descriptors counter
	uint32_t	status;						//! OUT	state of DMA channel	
};

//-----------------------------------------------------------------------------
/*!
	 \brief Ожидание пользовательского прерывания с заданным номером.
 */
struct usr_irq_wait_t {
	size_t irq_id;              //! USER IRQ ID
	size_t channel;             //! DMA channel number 0, 1, 2, 3 form USER_IRQ_ID
	size_t timeout;             //! Timeout of waiting operation
	size_t total_irq_counter;   //! Counter of ISR calling for DDR channel
};

//-----------------------------------------------------------------------------
/*!
	 \brief Разрешение пользовательского прерывания с заданным номером.
 */
struct usr_irq_enable_t {
	size_t irq_id;              //! USER IRQ ID
};

//-----------------------------------------------------------------------------
/*!
	 \brief Запрещение пользовательского прерывания с заданным номером.
 */
struct usr_irq_disable_t {
	size_t irq_id;              //! USER IRQ ID
};

//-----------------------------------------------------------------------------
/*! 
	 \brief Структура блочка для получения состояния канала DMA.

	 Адрес блочка возвращает команда размещения памяти для канала DMA.
*/
struct dma_stub_t {
	uint32_t completed_desc_counter;	// Completed Descriptors Counter
	uint32_t completed_block_counter;	// Total Counter of all completed Block
	int32_t	 last_block_number;		// Number of last completed block
	uint32_t status;				// DMA channel state
	uint32_t reserved[4];
};

#pragma pack(pop)

#endif // _FMCPEX_API_H_
