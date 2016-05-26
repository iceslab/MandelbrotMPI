#!/usr/bin/python

import sys

file_lines = []
results = []


def parse():
    last_size = 0
    last_seed = 0

    start = 0
    start_task = 0
    counter = 0
    task = 0
    all = 0

    for line in file_lines:
        splited = line.split(" ")

        if splited[0] == "":
            result = [];
            result.append(last_size)
            result.append(last_seed)
            result.append(str((all / counter)))
            result.append(str((task / counter)))
            print result
            results.append(result)
            counter = task = all = 0;

        if splited[0] == "Size":
            last_size = splited[1]

        if splited[0] == "Seed":
            last_seed = splited[1]

        if splited[0] == "start":
            start = splited[2]

        if splited[0] == "start_tasks":
            start_task = splited[2]

        if splited[0] == "end_tasks":
            task += (int(splited[2]) - int(start_task))

        if splited[0] == "end_all":
            all += (int(splited[2]) - int(start))
            counter += 1


def read_file(filename):
    with open(filename) as f:
        for line in f:
            file_lines.append(line[:-1])


def write_file(filename):
    f = open(filename, 'w')

    for res in results:
        f.write(";".join(res)+"\n")


def main(argv):
    read_file(argv[0])
    parse()
    write_file(argv[1])

if __name__ == "__main__":
    main(sys.argv[1:])