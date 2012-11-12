CIAI: mere.o log_disque.o simulation.o
gcc -o CIAI mere.o simulation.o log_disque.o -Wall -lpthread -lrt

mere.o: mere.c
gcc -c mere.c -Wall -lpthread -lrt

log_disque.o: log_disque.c
gcc -c log_disque.c -Wall -lpthread -lrt

simulation.o: Simulation/simulation.c
gcc -c Simulation/simulation.c -Wall -lpthread -lrt