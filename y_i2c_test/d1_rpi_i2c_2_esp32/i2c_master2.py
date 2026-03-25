from smbus import SMBus
import time

addr = 0x08
bus = SMBus(1)

while True:
    msg = input("Send string: ")

    data = [ord(c) for c in msg]   # 字串 → ASCII list

    try:
        bus.write_i2c_block_data(addr, 0x00, data)
    except Exception as e:
        print("I2C error:", e)

    time.sleep(0.1)