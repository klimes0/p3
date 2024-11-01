#!/usr/bin/python3

from argparse import ArgumentParser
from subprocess import run
from resource import getrusage as resource_usage, RUSAGE_CHILDREN
from time import time as timestamp

parser = ArgumentParser(description="Profile pzip execution time")
parser.add_argument('program', type=str, help="program to profile")
parser.add_argument('input', type=str, help="name of input file")
parser.add_argument('output', type=str, help="name of output file")
parser.add_argument('nThreads', type=int, help="Number of threads")
res = parser.parse_args()

cmd_list = [res.program, res.input, res.output, str(res.nThreads)]

start_time, start_resources = timestamp(), resource_usage(RUSAGE_CHILDREN)
run_result = run(cmd_list)

if run_result.returncode != 0:
    print(f"Execution failed with error code {run_result.returncode}")
    exit(run_result.returncode)

end_resources, end_time = resource_usage(RUSAGE_CHILDREN), timestamp()

real = end_time - start_time
sys= end_resources.ru_stime - start_resources.ru_stime
user = end_resources.ru_utime - start_resources.ru_utime
result = ((user + sys )/ real)/ res.nThreads

print(f"Wall time: {real:.5f} seconds")
print(f"CPU Time (System): {sys:.5f}  seconds")
print(f"CPU Time (User): {user:.5f} seconds")
print(f"Number of threads: {res.nThreads}")

print(f"Parallel efficiency (PE): {result:.5f}")
