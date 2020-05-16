import os
import sys
import subprocess

if (len(sys.argv) < 2):
    print("Usage: python3 walk_stack.py <.dmp file>")
    exit(1)

dmp = sys.argv[1]
syms = [f for f in os.listdir(os.getcwd()) if f.endswith(".sym")]
minidump_stackwalk = os.path.dirname(os.path.realpath(__file__)) + "/minidump_stackwalk"

for sym in syms:
    with open(sym, "r") as f:
        info = f.readline()[:-1].split(" ")
    if len(info) == 5:
        dir = "sym/%s/%s/" % (info[-1], info[-2])
        subprocess.run("mkdir -p %s" % dir, shell=True, check=True)
        subprocess.run("cp %s %s" % (sym, dir), shell=True, check=True)

subprocess.run("%s %s sym" % (minidump_stackwalk, dmp), shell=True, check=True)
