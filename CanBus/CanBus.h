#ifndef CanBus_h
#define CanBus_h

#include "Arduino.h"
#include <map>
#include <vector>
#include <FlexCAN_T4.h>

#define CBTP_CLASS template <CAN_DEV_TABLE selectedBus>
#define CBTP_FNC CanBus<selectedBus>

CBTP_CLASS class CanBus {
protected:
  CanBus();
  int baudRate;
  FlexCAN_T4<selectedBus, RX_SIZE_256, TX_SIZE_16> can;

public:
  std::map<uint32_t, CAN_message_t> mestable;
  std::vector<CAN_message_t> senddata;

  static CBTP_FNC &getInstance();

  void begin(int baudRate);
  void reset();

  void fetch();
  void read(uint32_t canid, uint8_t data[8]);

  void stage(uint32_t canid, uint8_t buf[8]);
  void write();
};

#include "CanBus/CanBus.tpp"
#endif