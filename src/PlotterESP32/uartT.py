from machine import UART

uart = UART(2, 9600)
uart.init(baudrate=9600, rx=16, tx=17, bits=8, parity=None, stop=1)


def do_connect():
    import network
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    if not wlan.isconnected():
        print('connecting to network...')
        wlan.connect('POCOPHONE F1', 'nonaccedi')
        while not wlan.isconnected():
            pass
    print('network config:', wlan.ifconfig())


def uartSendCoords(var):
    var += '\r\n'
    uart.write(var)


def disconnect():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(False)
    print("network disconnected")


def read():
    from machine import UART
    from time import sleep
    import coords
    do_connect()
    coords = coords.getCoords()
    point = [coords[n:n + 3] for n in range(0, len(coords), 3)]

    uart = UART(2, 9600)
    uart.init(baudrate=9600, rx=16, tx=17, bits=8, parity=None, stop=1)

    print(uart.write(point[0][0] + point[0][1] + point[0][2] + "\r\n"))

    strArray = []
    for index, elem in enumerate(point):
        string = elem[0] + elem[1] + elem[2]
        strArray.append(string)

    i = 0
    uart.write(strArray[i] + "\r\n")
    print(strArray)

    if (uart.any()):
        rx = uart.read(uart.any()).decode()

        if "ok" in rx:
            print("dati ricevuti:" + rx+"\r\n")
            uart.write(strArray[i] + "\r\n")
            print("dati: "+strArray[i]+"\r\n")
            print("coords n => "+str(i))
            if i <= len(point):
                i += 1

    sleep(5)
