make:
	mpicc -o mpitest.out  main.c
run:
	mpirun -np 14 mpitest.out 10 testfiles/numservers10_np14.txt
run2:
	mpirun -np 35 mpitest.out 15 testfiles/numservers15_np35.txt
run3:
	mpirun -np 70 mpitest.out 30 testfiles/numservers30_np70.txt
clean:
	rm mpitest.out
