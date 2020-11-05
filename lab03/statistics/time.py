#!/usr/bin/env python3

from datetime import datetime

us = []
with open('visits.txt') as file:
    for line in file:
        us.append(int(line))

hours = [0] * 24
WS = ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun']
weekdays = [0] * len(WS)
MS = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']
months = [0] * len(MS)


for u in us:
    d = datetime.fromtimestamp(u)
    hours[d.hour] += 1
    weekdays[d.weekday()] += 1
    months[d.month - 1] += 1

for i, h in enumerate(hours):
    print(f'{i:02d}h: {h}')
print()

for i in range(len(weekdays)):
    print(f'{WS[i]}: {weekdays[i]}')
print()

for i in range(len(months)):
    print(f'{MS[i]}: {months[i]}')
