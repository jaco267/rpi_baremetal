- [raspberry-pi-os](https://github.com/s-matyukevich/raspberry-pi-os)
- [rpi baremetal tutorial](https://www.youtube.com/watch?v=mCuxbI0FqzM&list=PLVxiWMqQvhg9FCteL7I0aohj1_YiUx1x8&index=19)
- [rpi baremetal source code](https://github.com/rockytriton/LLD/tree/main/rpi_bm/part17)

```
       RPi gpio pin  
   3.3 v---1  2    5v   
 GPIO  2   3  4    5v          
 GPIO  3   5  6 ---gnd         黑線 ----
 GPIO  4   7  8 ---GPIO 14     白線    |-----tty2usb cable
     gnd   9  10---GPIO 15     綠色線---
 GPIO 17---11 12---GPIO 18 
 GPIO 27---13 14---gnd          
 GPIO 22---15 16---gpio23      
   3.3v ---17 18---gpio24           
 GPIO 10---19 20---gnd  
 GPIO  9---21 22---gpio25    
 GPIO 11---23 24---gpio8         
     gnd   25 26---gpio7       
 GPIO  0---27 28---gpio1 
 GPIO  5---29 30---gnd 
 GPIO  6---31 32---gpio12
 GPIO 13   33 34---gnd
 GPIO 19   35 36---GPIO 16 
 GPIO 26---37 38---GPIO 20
    gnd    39 40---GPIO 21 ....led
```

```sh
make clean
make
make armstub

mkdir mnt && mkdir mnt/fat32 && mkdir mnt/ext4
sudo mount /dev/sda1 mnt/fat32 && sudo mount /dev/sda2 mnt/ext4
#mnt/fat32 會有 kernel8.img  備份 原本的 kernel8.img 

sudo cp kernel8.img mnt/fat32/kernel8.img
sudo cp armstub-new.bin mnt/fat32/armstub-new.bin
sudo cp config.txt mnt/fat32/config.txt
sudo umount mnt/fat32 && sudo umount mnt/ext4

ls /dev/tty*
# 應該會看到 ttyusb0   
正確流程是先把rpi 插頭拔掉  
在斷電狀態下  
sudo screen /dev/ttyUSB0 115200
插上rpi 插頭  就可以看到 boot code 
# MESS:00:00:02.362362:0: Device tree loaded to 
# Rasperry PI Bare Metal OS Initializing...
# Blame...
# Blame...
# Blame...
ctrl+A
k
# sudo screen /dev/ttyUSB0 115200,cs8,-ixon,-ixoff,-crtscts
# 好像要等很久才會看到 login   
# 要記得把 cmdline.txt 的 quiet 那些刪掉  才能看到很多 很酷的log
```