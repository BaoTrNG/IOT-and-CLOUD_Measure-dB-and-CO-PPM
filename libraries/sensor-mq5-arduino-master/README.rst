#################################################################
Thư viện giao tiếp với cảm biến khí Gas MQ5 cho Arduino, ESP8266
#################################################################

:Info: Github <https://github.com/thiminhnhut/sensor-mq5-arduino>
:Author: Thi Minh Nhựt <thiminhnhut@gmail.com>
:Date: $Date: 23/03/2019 $
:Revision: $Revision: 1.0.0 $
:Description: Thư viện giao tiếp với cảm biến khí Gas MQ5 cho Arduino, ESP8266

===============================================================================

.. sectnum::

.. contents:: Nội dung

===============================================================================

Nội dung thực hiện
******************

Thư viện MQ5Sensor
======================

* Thư mục source: `src <https://github.com/thiminhnhut/sensor-mq5-arduino/blob/master/src>`_.

  * File `MQ5Sensor.h <https://github.com/thiminhnhut/sensor-mq5-arduino/blob/master/src/MQ5Sensor.h>`_:

  .. code::

    MQ5Sensor(uint8_t pinA, uint8_t pinD);

    float getGas();

    bool isDetect();

* Ví dụ: `TestSensor.ino <https://github.com/thiminhnhut/sensor-mq5-arduino/blob/master/examples/TestSensor/TestSensor.ino>`_

Chức năng của thư viện MQ5Sensor
================================

* Đọc giá trị nồng độ phần trăm khí Gas có trong không khí từ cảm biến khí Gas MQ5
