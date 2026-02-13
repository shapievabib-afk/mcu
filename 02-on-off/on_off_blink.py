import serial, time, glob

p = (glob.glob('/dev/ttyACM*') + glob.glob('/dev/ttyUSB*'))[0]
s = serial.Serial(p, 115200, timeout=1)
time.sleep(2)

while 1:
    c = input().strip()
    if c == 'd': s.write(b'd\n')
    if c == 'v': s.write(b'v\n')
    if c == 'e': s.write(b'e\n')
    if c and c[0] == 'e' and len(c) > 1:
        try:
            t = float(c[1:])
            for _ in range(10):
                s.write(b'e\n'); time.sleep(t)
                s.write(b'd\n'); time.sleep(t)
        except: pass
    if c == 'q': break