#ifndef ESP_HAL_QSPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include "driver/spi_master.h"

struct qspi_struct_t{
  uint8_t host;
  int   cs;
  spi_device_handle_t dev;
};
typedef struct qspi_struct_t qspi_t;

bool qspiBusInit(qspi_t * qspi,uint32_t freq);
bool qspiBusDeinit(qspi_t * qspi);
bool qspiWritePolling(qspi_t * qspi,uint32_t addr,uint32_t cmd,uint8_t data[],uint32_t length);
bool qspiReadPolling(qspi_t * qspi,uint32_t addr,uint32_t cmd,uint8_t data[],uint32_t length);

#ifdef __cplusplus
}
#endif
#endif //ESP_HAL_QSPI_H
