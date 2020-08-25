import os
import sys
import subprocess

XCRUN = "xcrun atos -o %s/Contents/Resources/DWARF/%s -arch %s -l %s %s"

if (len(sys.argv) < 4):
    print("Usage: python3 symbolicatexcode.py <bt file> <image list file> <arch>")
    exit(1)

arch = sys.argv[3]

with open(sys.argv[1], "r") as f:
    stack = [line[line.find('frame #'):] for line in f.read().split("\n") if "frame #" in line]
with open(sys.argv[2], "r") as f:
    raw_images = f.read().split("\n")

images = {}
for image in raw_images:
    if ".framework" not in image:
        continue
    parts = [part for part in image.split('/')[0].split() if '0x' in part]
    if len(parts) == 1:
        images[image.split("/")[-1].strip()] = parts[0]

dSYMs = {f[:-15]: f for f in os.listdir(os.getcwd()) if f.endswith(".framework.dSYM")}

for line in stack:
    parts = line.split()
    if (parts[2].startswith("0x")) and parts[3] in dSYMs and parts[3] in images:
        res = subprocess.run(XCRUN % (dSYMs[parts[3]], parts[3], arch, images[parts[3]], parts[2]),
            shell=True, check=True, capture_output=True)
        print(" ".join(parts[0:4]), res.stdout.decode()[:-1])
    else:
        print(" ".join(parts))
