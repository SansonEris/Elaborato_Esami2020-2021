documentazione: https://micropython.org/download/esp32/

esptool.py --chip esp32 --port /dev/ttyUSB0 erase_flash

esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 115200 write_flash -z 0x1000 esp32-20210429-unstable-v1.15-22-ga708848b0.bin



per connettersi al micro
>rshell -p /dev/ttyUSB0 

Per copiare un file nel micro 
>rshell
> cp [nomefile].py /pyboard/

per vedere i file nel micro

>ls /pyboard/

per entrare in mod compilazione 
> repl 
> import [nomefile]

eseguire una funzione
>repl
>[nomefileDellaFunzione].[nomeFunzione()]

uscire "repl mode" -> Ctrl-x
uscire rshell -> Ctrl-Z

