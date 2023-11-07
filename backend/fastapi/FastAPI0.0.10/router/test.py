import subprocess
fileURL = "./wow"
data = b""
with open(fileURL, 'rb') as fHandler:
    data = fHandler.read(2044)
    data = fHandler.read(2044)
if data == b"":
    print("End of File")
    subprocess.run(['rm',f'{fileURL}'])


