#include "Arduino.h"
#include <CanBus/CanBus.h>

CBTP_CLASS CBTP_FNC::CanBus() {}
CBTP_CLASS CBTP_FNC &CBTP_FNC::getInstance() {
  static CanBus canbus;
  return canbus;
}

CBTP_CLASS void CBTP_FNC::begin(int baudRate) {
  this->baudRate = baudRate;
  can.begin();
  can.setBaudRate(baudRate);

  delay(500);
}

CBTP_CLASS void CBTP_FNC::reset() {
  can.reset();
  mestable.clear();
  senddata.clear();
  begin(baudRate);
}

// canbusからデータを受信する関数
CBTP_CLASS void CBTP_FNC::fetch() {
  CAN_message_t msg;
  while (can.fetch(msg)) {
    // mestable[msg.id] = msg;
    gotMessages[msg.id].push_back(msg);
    if (available(msg.id) > 10)
      gotMessages[canid].erase(gotMessages[canid].begin());
  }
}

// ID指定でデータ読み出し
CBTP_CLASS int CBTP_FNC::read(uint32_t canid, uint8_t data[8]) {
  if (available(canid) == 0)
    return 0;

  CAN_message_t msg = gotMessages[canid].front();
  memcpy(data, msg.buf, 8);
  gotMessages[canid].erase(gotMessages[canid].begin());
  return 1;
}

// canmessageの生データをvectorに登録
CBTP_CLASS void CBTP_FNC::stage(uint32_t canid, uint8_t buf[8]) {
  CAN_message_t msg;
  msg.id = canid;
  for (int i = 0; i < 8; i++) {
    msg.buf[i] = buf[i];
  }
  msg.len = 8;
  senddata.push_back(msg);
}

// CANBusにデータを送信
CBTP_CLASS void CBTP_FNC::write() {
  for (CAN_message_t outmes : senddata) {
    can.write(outmes);
  }
  senddata.clear();
}