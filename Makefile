all:i2cpotWrite  writewiper readtolarance
i2cpotWrite: i2cpotWrite.c i2cpot.h
	gcc -o i2cpotWrite  i2cpotWrite.c

writewiper: writewiper.c i2cpot.h
	gcc -o writewiper  writewiper.c
readtolarance: readtolarance.c i2cpot.h
	gcc  -g -Wall -o readtolarance  readtolarance.c



clean:
	rm -rf readtolarance writewiper i2cpotWrite *.o

