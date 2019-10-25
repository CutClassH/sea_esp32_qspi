#include "esp32-hal-qspi.h"

static const spi_device_interface_config_t QSPI_DEV_DEFAULT_CFG = {
  .command_bits = 8,
  .dummy_bits = 4,
  .address_bits = 32,
  .clock_speed_hz = SPI_MASTER_FREQ_40M,
  .mode = 0,      //SPI MODE0
  .spics_io_num = -1,  //CS GPIO pin for this device, or -1 if not used
  .queue_size = 7, //We want to be able to queue 7 transactions at a time
  .flags = SPI_DEVICE_HALFDUPLEX
  };

static const spi_bus_config_t VSPI_BUS_DEFAULT_CFG = {
	.mosi_io_num = 23,
	.miso_io_num = 19,
	.sclk_io_num = 18,
	.quadwp_io_num = 22,
	.quadhd_io_num = 21,
	.max_transfer_sz = 512,
	.flags = SPICOMMON_BUSFLAG_MASTER | SPICOMMON_BUSFLAG_NATIVE_PINS};

static const spi_bus_config_t HSPI_BUS_DEFAULT_CFG = {
	.mosi_io_num = 13,
	.miso_io_num = 12,
	.sclk_io_num = 14,
	.quadwp_io_num = 2,
	.quadhd_io_num = 4,
	.max_transfer_sz = 512,
	.flags = SPICOMMON_BUSFLAG_MASTER | SPICOMMON_BUSFLAG_NATIVE_PINS};

bool qspiBusInit(qspi_t *qspi,uint32_t freq)
{
	esp_err_t error;
	spi_device_interface_config_t spi_dev_cfg = QSPI_DEV_DEFAULT_CFG;
	spi_device_handle_t dev;
	spi_dev_cfg.spics_io_num = qspi->cs;
	spi_dev_cfg.clock_speed_hz = APB_CLK_FREQ / (10 - freq);

	gpio_set_direction((gpio_num_t)qspi->cs, GPIO_MODE_OUTPUT);//initialize cs pin
	gpio_set_level((gpio_num_t)qspi->cs, 1);
	if(qspi->host==VSPI_HOST){
		spi_bus_free(qspi->host);
		error = spi_bus_initialize(qspi->host, &VSPI_BUS_DEFAULT_CFG, 0);
	}
	else if(qspi->host==HSPI_HOST){
		spi_bus_free(qspi->host);
		error = spi_bus_initialize(qspi->host, &HSPI_BUS_DEFAULT_CFG, 0);
	}
	else{
		printf("SPI HOST(%d) not suport QSPI\r\n", qspi->host);
		return false;
	}
	if(error==ESP_OK){
		printf("SPI HOST(%d) initialize success\r\n",qspi->host);
	}
	else{
		//ESP_ERROR_CHECK(error);
		printf("SPI HOST(%d) initialize failure,error code %X\r\n",qspi->host,error);
		return false;
	}
	error = spi_bus_add_device(VSPI_HOST, &spi_dev_cfg, &dev);
	if(error==ESP_OK){
		qspi->dev = dev;
	}
	else{
		//ESP_ERROR_CHECK(error);
		return false;
	}
	printf("QSPI Freq is %fMHz\r\n", (float)spi_dev_cfg.clock_speed_hz / 1000000);
	return true;
}

bool qspiBusDeinit(qspi_t * qspi)
{
	if(qspi->dev != NULL){
		spi_bus_remove_device(qspi->dev);
		spi_bus_free(qspi->host);
		qspi->dev != NULL;
	}
	return true;
}

bool qspiWritePolling(qspi_t * qspi,uint32_t addr,uint32_t cmd,uint8_t data[],uint32_t length)
{
	esp_err_t error;
	spi_transaction_t trans = { .flags = SPI_TRANS_MODE_QIO | SPI_TRANS_MODE_DIOQIO_ADDR,
			.length = length * 8 ,.tx_buffer = data,.addr = addr ,.cmd = cmd 
		};
    
	spi_device_queue_trans(qspi->dev, &trans, portMAX_DELAY);
	error = spi_device_get_trans_result(qspi->dev, &trans, portMAX_DELAY);
	if (error == ESP_OK){
		//printf("tx already\n");
		return true;
	}
	else
	{
		printf("write error %d\r\n",error);
		//ESP_ERROR_CHECK(error);
		return false;
	}
}

bool qspiReadPolling(qspi_t * qspi,uint32_t addr,uint32_t cmd,uint8_t data[],uint32_t length)
{
	esp_err_t error;
	spi_transaction_t trans;
	memset(&trans, 0, sizeof(trans));
	trans.flags = SPI_TRANS_MODE_QIO | SPI_TRANS_MODE_DIOQIO_ADDR;
	//trans.length = data_length * 8 ;
	trans.rxlength = length * 8;
	trans.rx_buffer = data;
	trans.addr = addr;
	trans.cmd = cmd;
	
	spi_device_queue_trans(qspi->dev, &trans, portMAX_DELAY);
	error = spi_device_get_trans_result(qspi->dev, &trans, portMAX_DELAY);
	if (error == ESP_OK){
		return true;
	}
	else{
		//ESP_ERROR_CHECK(error);
		return false;
	}
}
