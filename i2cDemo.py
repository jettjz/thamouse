import smbus
import time

IR = smbus.SMBus(1)

#SlaveAddr = 0xb0
SlaveAddr = 0x58

IR.write_i2c_block_data(SlaveAddr, 0x30, [0x01])
time.sleep(.100)
IR.write_i2c_block_data(SlaveAddr, 0x30, [0x08])
time.sleep(.100)
IR.write_i2c_block_data(SlaveAddr, 0x06, [0x90])
time.sleep(.100)
IR.write_i2c_block_data(SlaveAddr, 0x08, [0xc0])
time.sleep(.100)
IR.write_i2c_block_data(SlaveAddr, 0x1a, [0x40])
time.sleep(.100)
IR.write_i2c_block_data(SlaveAddr, 0x33, [0x33])
time.sleep(.100)
IR.write_i2c_block_data(SlaveAddr, 0x36, [])
time.sleep(.100)
irIn = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
while True:
    for i in range(16):
        irIn[i] = IR.read_byte(SlaveAddr)
    x1 = irIn[1]
    y1 = irIn[2]
    s = irIn[3]
    x1 += (s & 0x30) << 4
    y1 += (s & 0xc0) << 2
    
    x2 = irIn[4]
    y2 = irIn[5]
    s = irIn[6]
    x2 += (s & 0x30) << 4
    y2 += (s & 0xc0) << 2

    x3 = irIn[7]
    y3 = irIn[8]
    s = irIn[9]
    x3 += (s & 0x30) << 4
    y3 += (s & 0xc0) << 2

    x4 = irIn[10]
    y4 = irIn[11]
    s = irIn[12]
    x4 += (s & 0x30) << 4
    y4 += (s & 0xc0) << 2

    print("READING: X1  Y1  X2  Y2  X3  Y3")
    print("         ", x1, "\t", y1, "\t", x2, "\t", y2, "\t", x3, "\t", y3, "\t", x4, "\t", y4)
    time.sleep(.5)









