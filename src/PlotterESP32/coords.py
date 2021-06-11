import urequests as requests


def getCoords():
    r = requests.get("https://elaboratosevbt.altervista.org/PlotterWebsite/canvasData/coords.txt")
    coords = r.text.replace('\t', '\n').split()
    r.close()
    return coords

