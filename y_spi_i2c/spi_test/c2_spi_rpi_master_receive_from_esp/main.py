import spidev
import time
spi = spidev.SpiDev()

spi.open(0,0)

spi.max_speed_hz = 5000
spi.mode = 0
while True:
    r = spi.xfer2([42])
    print(r)
    time.sleep(1)