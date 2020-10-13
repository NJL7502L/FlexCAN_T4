# CanBusクラスの扱い方

## 宣言
```c++
CanBus<CAN0> &canbus = CanBus<CAN0>::getInstance();
```
使いたいバス名を指定し、canbus参照をgetInstance()で得る。

teensy3.2/3.5ではCAN0、teensy3.6ではCAN0/1、teensy4.0ではCAN1/2/3が使える。

このリストはFlexCAN_T4クラスから得ているのでそれに依存する。

## setup
```c++
canbus.begin(1000000);
```
begin()でそのバスのボーレートを指定する。

## 受信
