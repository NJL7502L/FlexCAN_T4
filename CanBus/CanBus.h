#ifndef CanBus_h
#define CanBus_h

#include <Arduino.h>
#include <map>
#include <queue>
#include <vector>
#include "../FlexCAN_T4.h"

#define CBTP_CLASS                                                             \
  template <CAN_DEV_TABLE BUS, int RX_LIMIT = 10, int TX_LIMIT = 10>
#define CBTP_FUNC template <CAN_DEV_TABLE BUS, int RX_LIMIT, int TX_LIMIT>
#define CBTP_OPT CanBus<BUS, RX_LIMIT, TX_LIMIT>

CBTP_CLASS class CanBus {
protected:
  CanBus();

  void operator=(const CBTP_OPT &obj) {}
  CanBus(const CBTP_OPT &obj) {}

  int baudRate = 1000000;

  int txQueueSize = 10;

  template <class T, int SIZE = 10> class MyQueue : protected std::queue<T> {
    uint limitSize = SIZE;

  public:
    using std::queue<T>::empty;
    using std::queue<T>::size;
    using std::queue<T>::front;
    using std::queue<T>::back;
    using std::queue<T>::push;
    using std::queue<T>::pop;

    void setLimitSize(uint size) { limitSize = size; }
    void limitPush(T msg) {
      push(msg);
      if (size() > limitSize) {
        pop();
      }
    }
  };

public:
  FlexCAN_T4<BUS, RX_SIZE_256, TX_SIZE_16> can;
  std::map<uint32_t, MyQueue<CAN_message_t, RX_LIMIT>> rxMap;
  MyQueue<CAN_message_t, TX_LIMIT> txQueue;

  static CanBus &getInstance();

  void begin(int baudRate);
  void reset();

  void fetch();
  int read(uint32_t canid, uint8_t data[8]);
  int available(uint32_t canid) { return rxMap[canid].size(); };

  void stage(uint32_t canid, uint8_t buf[8]);
  void write();
};

#include "CanBus/CanBus.tpp"
#endif