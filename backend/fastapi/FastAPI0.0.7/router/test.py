
a = "2023,11,,12,00,00,2,127,,1,255"
b = a.split(',')



for i in range(11):
    if b[i]=="":
        b[i] = None

print(b)


wowow
