#include "Arduino.h"
#include <CanBus/CanBus.h>

CBTP_FUNC CBTP_OPT::CanBus() {}
CBTP_FUNC CBTP_OPT &CBTP_OPT::getInstance() {
  static CanBus canbus;
  return canbus;
}

CBTP_FUNC void CBTP_OPT::begin(int baudRate) {
  this->baudRate = baudRate;
  can.begin();
  can.setBaudRate(baudRate);

  delay(500);
}

CBTP_FUNC void CBTP_OPT::reset() {
  can.reset();
  rxMap.clear();
  while (!txQueue.empty())
    txQueue.pop();
  begin(baudRate);
}

// canbusからデータを受信する関数
CBTP_FUNC void CBTP_OPT::fetch() {
  CAN_message_t msg;
  while (can.read(msg)) {
    rxMap[msg.id].limitPush(msg);
  }
}

// ID指定でデータ読み出し
CBTP_FUNC int CBTP_OPT::read(uint32_t canid, uint8_t data[8]) {
  if (available(canid) == 0)
    return 0;

  CAN_message_t msg = rxMap[canid].front();
  memcpy(data, msg.buf, 8);
  rxMap[msg.id].pop();
  return 1;
}

CBTP_FUNC void CBTP_OPT::stage(uint32_t canid, uint8_t buf[8]) {
  CAN_message_t msg;
  msg.id = canid;
  for (int i = 0; i < 8; i++) {
    msg.buf[i] = buf[i];
  }
  msg.len = 8;
  txQueue.limitPush(msg);
}
// 後でstage(can_message_t message)に対応したやつも作る

// CANBusにデータを送信
CBTP_FUNC void CBTP_OPT::write() {
  while (!txQueue.empty()) {
    can.write(txQueue.front());
    txQueue.pop();
  }
}