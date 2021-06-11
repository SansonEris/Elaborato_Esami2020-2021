import network
from machine import Pin, SoftI2C, UART
import ssd1306
from time import sleep
import machine

machine.freq()

uart = UART(2, 9600)
uart.init(baudrate=9600, rx=16, tx=17, bits=8, parity=None, stop=1)

print(uart)

i2c = SoftI2C(scl=Pin(22), sda=Pin(21))
oled_width = 128
oled_height = 64
display = ssd1306.SSD1306_I2C(oled_width, oled_height, i2c)


def connect():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    if not wlan.isconnected():
        print('connecting to network...')
        display.text("connecting to", 0, int(oled_height / 3))
        display.text("network...", 0, int(oled_height / 1.7))
        display.show()
        try:
            wlan.connect('POCOPHONE F1', 'nonaccedi')
        except:
            display.fill(0)
            display.show()
            display.text("Check WiFi conn", 0, int(oled_height / 3))
            display.text(":/", int(oled_width / 2), int(oled_height / 1.7))
            display.fill(0)
            display.show()

        while not wlan.isconnected():
            pass
    print('network config:', wlan.ifconfig())


def disconnect():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(False)
    print("network disconnected")

def caricamento():
    display.fill(0)
    display.show()
    display.text("Caricamento...", 0, int(oled_height / 2))
    display.show()

def drawingComplete():
    display.fill(0)
    display.show()
    display.text("Drawing complete", 0, int(oled_height / 2.5))
    display.text(":)", int(oled_width / 2), int(oled_height / 1.3))
    display.show()
    sleep(5)


def noPointFound():
    display.fill(0)
    display.show()
    display.text("no point found", 7, int(oled_height / 2))
    display.text(":/", int(oled_width / 2), int(oled_height / 1.3))
    display.show()
    sleep(5)
    display.fill(0)
    display.show()


def byteImg():
    import framebuf
    buffer = bytearray(
        b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xe0\x10\x8a\n0\x10$\xc8\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00x\xfe\xff?\xfe\xfd|\xfb\xf8\xfc\xff|\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80\xc0@\xe0\xe0\xe0\xc0\xc1c\x9f\xff\x0f\xce\xff\xff\xcf\xe3\xf0pp\xa0\xe0\xe0\xc0\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xf0\xff\xfb\xe7\xdf\x8e\x9e\xfd\x95"$%\x00\xfe\xff\xff\xed\xdd\xce\xce\xff\xe7\xe7\xf7\xfb\xf9\xfep\xe0\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00@6\x9bg7\xcfo\x9d\xdb?\xbbws\xf7\xfa\xfa\xf9\xf4\xfd\xf7\xf3\xf1\xfdsy\xbd;\xd9\x9do\xcc6g\x9b6@\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x02\x01\x04\x03\t\x06\x13\x0c&\x19\r\x1b\x1b\r\x19&\x0c\x13\x06\t\x03\x04\x01\x02\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00')
    fb = framebuf.FrameBuffer(buffer, 48, 48, framebuf.MVLSB)
    # framebuf.MONO_HLSB
    display.blit(fb, -5, int(oled_height / 3.3))
    display.show()


def start():
    display.fill(0)
    display.show()
    display.text("ESP32 with", 0, 0)
    display.text("MICROPYTHON", int(oled_width / 3), int(oled_height / 2))
    byteImg()
    display.show()
    sleep(2)
    display.fill(0)
    display.show()


def drawCoords():
    startDraw = False
    global progress, status
    import gfx
    import coords
    import urequests as requests

    coords = coords.getCoords()

    r = requests.get("https://elaboratosevbt.altervista.org/PlotterWebsite/canvasData/dimension.txt")
    dim = r.text.replace('\n', ' ').split()
    r.close()

    CanvasY = dim[0]
    CanvasX = dim[1]
    print("dimensioni canvas:  " + CanvasX + "   " + CanvasY)  # dimensione del canvas web

    while uart.any() == 0:
        while not startDraw:
            if uart.any():
                try:
                    rx = uart.read(uart.any()).decode()
                    print(rx)
                    if "ready" in rx:
                        print("dento ready")
                        uart.write(bytearray(str(CanvasX+CanvasY)+"\r"))
                        print("\n"+CanvasX+CanvasY)
                        sleep(3)
                        startDraw = True
                    else:
                        startDraw = False

                except UnicodeError as e:
                    for p in dir(e):
                        if not p.startswith('_'):
                            print('%s=%r' % (p, getattr(e, p)))
    if uart.any():
        print(uart.read(uart.any()))

    # prende le coordinate di ogni punto [[x,y,z][x,y,z]...]
    point = [coords[n:n + 3] for n in range(0, len(coords), 3)]

    strArray = []

    lenghtPoint = len(point)

    print("numero punti da disegnare:  " + str(lenghtPoint))
    display.fill(0)
    display.show()

    for index, elem in enumerate(point):
        string = elem[0] + elem[1] + elem[2]
        strArray.append(string)
        print("punti da inviare: " + str(strArray[index]) + " index: " + str(index))

    ind = 0
    status = 0
    if lenghtPoint != 0 & startDraw:

        while status != 100:
            #deve essere prima dell'incremento dell'indice
            #creazione stringa da visualizzare sul display
            z = int(point[ind][2])
            s = "X:" + str(point[ind][0]) + " " + "Y:" + str(point[ind][1]) + " " + "Z:" + str(z)
            sleep(0.7)

            if uart.any():
                try:
                    rx = uart.read(uart.any()).decode()
                    print("dati TX PIC16f887:" + rx + "\r\n")
                    if "ok" in rx:
                        sleep(1)
                        print("dati inviati TXESP: ")
                        print(strArray[ind])
                        print("index: " + str(ind) + "\r\n")
                        uart.write(bytearray(str(strArray[ind]) + "\r"))
                        ind += 1
                        sleep(3)
                except UnicodeError as e:
                    for p in dir(e):
                        if not p.startswith('_'):
                            print('%s=%r' % (p, getattr(e, p)))

            # proporzioni per la barra del progresso sullo schermo
            equivalentValWidth = oled_width / lenghtPoint
            equivalentVal = 100 / lenghtPoint
            graphics = gfx.GFX(oled_width, oled_height, display.pixel)

            progress = ind * equivalentValWidth
            status = equivalentVal * ind
            display.fill(0)
            display.show()
            # scrittura dei valori sul display
            display.text(s, 0, 0)
            display.text(str(round(status, 3)) + "% ", 0, 23)
            graphics.rect(0, 38, oled_width, 10, 1)
            graphics.fill_rect(0, 38, progress, 10, 1)
            display.show()

            sleep(1)

            # trasmissione dati
            print("\nstatus: "+ str(status)+"\n")
            if status != 100:
                sleep(1)
                print("invio ok")
                uart.write(bytearray("ok\r"))

        if status == 100:
            if uart.any():
                try:
                    rx = uart.read(uart.any()).decode()
                    print("PIC16f887: ")
                    print(rx)
                except UnicodeError as e:
                    for p in dir(e):
                        if not p.startswith('_'):
                            print('%s=%r' % (p, getattr(e, p)))
            sleep(2)
            print("end")
            uart.write(bytearray("end\r"))
            drawingComplete()

    else:
        noPointFound()


while True:
    start()
    connect()
    caricamento()
    drawCoords()
