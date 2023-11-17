fileurl = 'M16M.tflite'

with open(fileurl, 'rb') as fh:
    ff = fh.read()
    print(ff[10:])
