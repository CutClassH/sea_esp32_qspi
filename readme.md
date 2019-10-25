# sea_esp32_qspi
sea_esp32_qspi 是 [Spartan Edge Accelerator Board]()的QSPI库，运行在SEA的ESP32端，用于esp32与fpga的通信。
## 使用说明
安装Arduino IDE的esp32 board和Arduino库可参照https://github.com/sea-s7/spartan-edge-esp32-boot.
## 用例
SeaTrans是sea_esp32_qspi的实例，使用的qspi控制器和qspi的io与SEA板设计一致。\
一个简单的读写FPGA RAM的程序
```c++
  SeaTrans.begin();
  SeaTrans.write(0, data1, 2);
  SeaTrans.read(0, data2, 2);
```
begin()初始化SeaTrans，end()释放QSPI总线。\
读写函数的申明
```c++
bool write(uint16_t addr, uint8_t data[], uint16_t length);
bool read(uint16_t addr, uint8_t data[], uint16_t length=1);
```