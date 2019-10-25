#ifndef SEA_TRANS_H

#include <Arduino.h>
#include "esp32-hal-qspi.h"

#ifndef ARDUINO_ARCH_ESP32
#error "the target is not ESP32."
#endif


class sea_esp32_qspi
{

public:
	enum freq_e
    {
        FREQ_8M = 0,
        FREQ_9M,
        FREQ_10M,
        FREQ_11M,
        FREQ_13M,
        FREQ_16M,
        FREQ_20M,
        FREQ_26M,
        FREQ_40M,
    };
	static const uint32_t WR_CMD = 0x32;
	static const uint32_t RD_CMD= 0x6B;

private:
	qspi_t qspi;

public:
	sea_esp32_qspi(int spi_host,int cs);
	~sea_esp32_qspi();

	//bool begin();
    bool begin(freq_e baud=FREQ_40M);
    bool end();

    bool write(uint16_t addr, uint8_t data);
    bool write(uint16_t addr, uint8_t data[], uint16_t length);
    bool write(uint16_t addr, char data[], uint16_t length);

    bool read(uint16_t addr, uint8_t data[], uint16_t length=1);
    bool read(uint16_t addr, char data[], uint16_t length=1);
};

#if 1
extern sea_esp32_qspi SeaTrans;
#endif

#endif //SEA_TRANS_H
