UNDERSCORE = 1
NUMBER = 2
LETTERao = 4
LETTERpz = 8
LETTERAO = 16
LETTERPZ = 32

table_low = [0 for i in range(16)]
table_high = [0 for i in range(16)]
table_low[ord('_')&0xf] |= UNDERSCORE
table_high[ord('_')>>4] |= UNDERSCORE
for i in range(10):
    table_low[(ord('0')+i)&0xf] |= NUMBER
    table_high[(ord('0')+i)>>4] |= NUMBER

for i in range(15):
    table_low[(ord('a')+i)&0xf] |= LETTERao
    table_high[(ord('a')+i)>>4] |= LETTERao
    table_low[(ord('A')+i)&0xf] |= LETTERAO
    table_high[(ord('A')+i)>>4] |= LETTERAO

for i in range(11):
    table_low[(ord('p')+i)&0xf] |= LETTERpz
    table_high[(ord('p')+i)>>4] |= LETTERpz
    table_low[(ord('P')+i)&0xf] |= LETTERPZ
    table_high[(ord('P')+i)>>4] |= LETTERPZ

print(table_low)
print(table_high)

for i in range(256):
    if table_low[i&0xf] & table_high[i>>4]:
        print(chr(i), end=' ')
print()