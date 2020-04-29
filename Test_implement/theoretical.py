def FIFO(processes):
    t, idx = 0, 0
    running = None
    readyQueue = list()
    ret = dict()

    while True:
        while idx < len(processes) and processes[idx][1] <= t:
            readyQueue.append(idx)
            ret[idx] = [None, None]
            idx += 1

        if running is None and len(readyQueue) > 0:
            running = readyQueue[0]
            readyQueue = readyQueue[1:]

            if ret[running][0] is None:
                ret[running][0] = t

        t += 1
        if running is not None:
            processes[running][2] -= 1
        
        if running is not None and processes[running][2] == 0:
            ret[running][1] = t
            running = None

        if running is None and len(readyQueue) == 0 and idx == len(processes):
            break

    ret = [(processes[i][0],) + tuple(ret[i]) for i in ret]
    return ret

def RR(processes):
    t, TTL, idx = 0, 0, 0
    running = None
    readyQueue = list()
    ret = dict()

    while True:
        while idx < len(processes) and processes[idx][1] <= t:
            readyQueue.append(idx)
            ret[idx] = [None, None]
            idx += 1

        if running is not None and TTL == 0:
            readyQueue.append(running)
            running = None

        if running is None and len(readyQueue) > 0:
            running = readyQueue[0]
            readyQueue = readyQueue[1:]

            if ret[running][0] is None:
                ret[running][0] = t

            TTL = 500

        t += 1
        if running is not None:
            processes[running][2] -= 1
            TTL -= 1
        
        if running is not None and processes[running][2] == 0:
            ret[running][1] = t
            running = None

        if running is None and len(readyQueue) == 0 and idx == len(processes):
            break

    ret = [(processes[i][0],) + tuple(ret[i]) for i in ret]
    return ret

def SJF(processes):
    t, idx = 0, 0
    running = None
    readyQueue = list()
    ret = dict()

    while True:
        while idx < len(processes) and processes[idx][1] <= t:
            readyQueue.append(idx)
            ret[idx] = [None, None]
            idx += 1

        if running is None and len(readyQueue) > 0:
            readyQueue.sort(key = lambda k : processes[k][2])
            running = readyQueue[0]
            readyQueue = readyQueue[1:]

            if ret[running][0] is None:
                ret[running][0] = t

        t += 1
        if running is not None:
            processes[running][2] -= 1
        
        if running is not None and processes[running][2] == 0:
            ret[running][1] = t
            running = None

        if running is None and len(readyQueue) == 0 and idx == len(processes):
            break

    ret = [(processes[i][0],) + tuple(ret[i]) for i in ret]
    return ret

def PSJF(processes):
    t, idx = 0, 0
    running = None
    readyQueue = list()
    ret = dict()

    while True:
        while idx < len(processes) and processes[idx][1] <= t:
            readyQueue.append(idx)
            ret[idx] = [None, None]
            idx += 1

        if running is not None and len(readyQueue) > 0:
            readyQueue.sort(key = lambda k : processes[k][2])
            head = readyQueue[0]
            if processes[running][2] > processes[head][2]:
                readyQueue.append(running)
                running = None

        if running is None and len(readyQueue) > 0:
            readyQueue.sort(key = lambda k : processes[k][2])
            running = readyQueue[0]
            readyQueue = readyQueue[1:]

            if ret[running][0] is None:
                ret[running][0] = t


        t += 1
        if running is not None:
            processes[running][2] -= 1
        
        if running is not None and processes[running][2] == 0:
            ret[running][1] = t
            running = None

        if running is None and len(readyQueue) == 0 and idx == len(processes):
            break

    ret = [(processes[i][0],) + tuple(ret[i]) for i in ret]
    return ret

def main():
    policy = input().strip()
    N = int(input())

    processes = list(map(lambda k : [k[0], int(k[1]), int(k[2])], [input().split() for _ in range(N)]))

    processes.sort(key = lambda k : (k[1], k[0]))

    if policy == 'FIFO':
        result = FIFO(processes)
    elif policy == 'RR':
        result = RR(processes)
    elif policy == 'SJF':
        result = SJF(processes)
    elif policy == 'PSJF':
        result = PSJF(processes)

    result.sort(key = lambda k : k[2])
    for p in result:
        print(' '.join(map(str, p)))

if __name__ == '__main__':
    main()