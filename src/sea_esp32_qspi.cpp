#include "sea_esp32_qspi.h"

#if 1
sea_esp32_qspi SeaTrans(VSPI_HOST, 5);
#endif

sea_esp32_qspi::sea_esp32_qspi(int spi_host,int cs){
	qspi.host = spi_host;
  qspi.cs = cs;
  qspi.dev = NULL;
}

sea_esp32_qspi::~sea_esp32_qspi()
{
	this->end();
}

bool sea_esp32_qspi::begin(freq_e baud)
{
	return qspiBusInit(&this->qspi,baud);
}
bool sea_esp32_qspi::end()
{
	if(qspi.dev!=NULL){
		return qspiBusDeinit(&this->qspi);
	}
	return true;
}

bool sea_esp32_qspi::write(uint16_t addr, uint8_t data)
{
	qspiWritePolling(&this->qspi, addr, WR_CMD,&data, 1);
}

bool sea_esp32_qspi::write(uint16_t addr, uint8_t data[], uint16_t length)
{
	qspiWritePolling(&this->qspi, addr, WR_CMD,data, length);
}

bool sea_esp32_qspi::write(uint16_t addr, char data[], uint16_t length)
{
	qspiWritePolling(&this->qspi, addr, WR_CMD,(uint8_t *)data, length);
}

bool sea_esp32_qspi::read(uint16_t addr, uint8_t data[], uint16_t length)
{
	qspiReadPolling(&this->qspi, addr, RD_CMD, data, length);
}

bool sea_esp32_qspi::read(uint16_t addr, char data[], uint16_t length=1)
{
	qspiReadPolling(&this->qspi, addr, RD_CMD, (uint8_t *)data, length);
}
