import os
import sys
import subprocess

XCRUN = "xcrun atos -o %s/Contents/Resources/DWARF/%s -arch %s -l %s %s"

if (len(sys.argv) < 3):
    print("Usage: python3 symbolicatereport.py <crash report file> <arch>")
    exit(1)

arch = sys.argv[2]

with open(sys.argv[1], "r") as f:
    stack = f.read().split("\n")

dSYMs = {f[:-15]: f for f in os.listdir(os.getcwd()) if f.endswith(".framework.dSYM")}

for line in stack:
    parts = line.split()
    if (parts[3].startswith("0x")) and parts[1] in dSYMs:
        res = subprocess.run(XCRUN % (dSYMs[parts[1]], parts[1], arch, parts[3], parts[2]),
            shell=True, check=True, capture_output=True)
        print(" ".join(parts[0:3]), res.stdout.decode()[:-1])
    else:
        print(" ".join(parts))
