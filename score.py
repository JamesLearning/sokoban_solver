with open('scores.txt','r') as f:
    data = f.readlines()
data = data[2:]
data = [x.strip().split() for x in data]
for i in range(len(data)):
    if len(data[i]) == 6:
        data[i] = ['-1'] + data[i]

bestmove = {}
for x in data:
    level = int(x[2])
    moves = int(x[3])
    if level not in bestmove or moves < int(bestmove[level][3]):
        bestmove[level] = x

bestmove_levels = sorted(list(bestmove.keys()))
assert bestmove_levels == list(range(1,91))
bestmove_values = [bestmove[x] for x in bestmove_levels]

with open('bestmove.txt','w') as f:
    for x in bestmove_values:
        f.write('\t'.join(x) + '\n')

with open('bestmove_simplified.txt','w') as f:
    for x in bestmove_values:
        f.write(f'{x[3]}\n')