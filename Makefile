# Makefile
VocalKiller: VocalKiller.c WavUtil.c
	gcc VocalKiller.c WavUtil.c -o vocalkiller
