#!/usr/bin/python3

import sys
import re

tok = re.compile("^\(?[ \t]*([A-Za-z0-9_]+)[ \t]*\,[ \t]*\"*(.+?)\"*[ \t]*\)?$")
toks = {}
i = 0

for line in sys.stdin:
    res = tok.match(line)
    if (res):
        if (res.group(1) not in toks):
            toks[res.group(1)] = i
            i+=1

        print(str(res.group(2)) + " -> " + str(toks[res.group(1)]))
