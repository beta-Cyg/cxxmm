#!/usr/bin/python3
import shutil
import sys
import os

def pack(dirname, packname):
	shutil.make_archive(packname, 'tar', dirname)

def unpack(packname, dirname):
	shutil.unpack_archive(packname, dirname)

if __name__ == '__main__':
	if sys.argv == 1 or sys.argv == 2:
		exit(1)
	else:
		if sys.argv[1] == 'pack':
			pack(sys.argv[2], sys.argv[2].rstrip('/'))
		elif sys.argv[1] == 'unpack':
			unpack(sys.argv[2]+".tar", sys.argv[2])
		elif sys.argv[1] == 'remove':
			os.remove(sys.argv[2]+".tar")
		else:
			exit(1)
