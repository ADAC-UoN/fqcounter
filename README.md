# fqcounter
Speedy tool for counting reads in fastq / fastq.gz files

A c++ fastq read counter with openMP multithreading

Core functionality derived from  
https://github.com/billzt/readfq  
which was modified from  
http://lh3lh3.users.sourceforge.net/parsefastq.shtml.


```
Usage: fqcounter  <*fastq>
       fqcounter -b  -t <int> <*fastq.gz>

	-b        Add a basecount column

	-l        Add basecount and mean length columns

	-t <int>  [2] Select number of threads
	          (one fastq file/thread)

	-o        Delayed ordered output

	-h        This message
```

> Note: No fastq format checking is performed  
        Also counts fasta files


To compile use make or:
```
   g++ -o fqcounter -fopenmp  fqcounter.cpp -lz
```


