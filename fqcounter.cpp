#include <zlib.h>  
#include <stdio.h> 
#include "kseq.h"  
#include <omp.h>
#include <getopt.h>
#include <libgen.h>
#include <iostream>
#include <iomanip>
#include <map>

// reference: http://lh3lh3.users.sourceforge.net/parsefastq.shtml
// reference: https://github.com/billzt/readfq

// Andrew Warry ADAC 15-02-2016
// 

KSEQ_INIT(gzFile, gzread)  
  
int main(int argc, char *argv[])  
{  
//    gzFile fp;  
//    kseq_t *seq;  
//    int l;  
    int threadnum;
    int c;
    
    int bflag = 0;
    int hflag = 0;
    int lflag = 0;
    int oflag = 0;
    int tvalue = 0;
    int nprocs = sysconf(_SC_NPROCESSORS_CONF);
    
    while( ( c = getopt (argc, argv, "t:bhol") ) != -1 ) 
    {
        switch(c)
        {
             case 'b':
                 bflag =1;
                 break;
             case 'l':
                 lflag =1;
                 break;
             case 'h':
                 hflag =1;
                 break;
             case 'o':
                 oflag =1;
                 break;
            case 't':
                if(optarg) tvalue = atoi(optarg) ;
                break;
        }

    }

 //     printf("nprocs: %d\n", nprocs);
    if(lflag ==1){
      bflag =1; 
    }
    
    if(tvalue){
      threadnum = tvalue;
    }else{
      threadnum = 2;
    }
//    printf("threads: %d\n", threadnum);
    if(threadnum >= nprocs){
      threadnum = (nprocs -1);
      fprintf(stderr, "## threads to use reset to: %d\n", threadnum);
    }
 //   printf("threads: %d\n", threadnum);


    if ((argc == 1)||(hflag == 1)){  
        fprintf(stderr, "%s  is a tool to count reads in fastq/fastq.gz/fasta/fasta.gz files\n\n", basename(argv[0])); 
        fprintf(stderr, "Usage: %s  <*fastq>\n", basename(argv[0]));  
        fprintf(stderr, "       %s -b  -t <int> <*fastq.gz>\n", basename(argv[0]));  
	fprintf(stderr, "\n\t-b        Add a basecount column\n"); 
	fprintf(stderr, "\n\t-l        Add basecount and mean length columns\n");
	fprintf(stderr, "\n\t-t <int>  [2] Select number of threads\n");
	fprintf(stderr,   "\t          Note: one fastq file/thread\n"); 
	fprintf(stderr, "\n\t-o        Delayed ordered output\n"); 
	fprintf(stderr, "\n\t-h        This message\n\n"); 
        return 1;  
    }
 //   int i;
 
 //#pragma omp parallel
 
 
//std::map<std::string, long> count_results;
//    if(bflag == 1){
      std::map<std::string, std::pair<long, long> > count_results2;
//    } else {
      std::map<std::string, long> count_results; 
//    }
 
 #pragma omp parallel for num_threads(threadnum)
   for(int i = optind; i < argc; ++i ) {
 //   for(int i = 1; i < argc; ++i ) {
 //       printf( "argv[ %d ] = %s\n", i, argv[i] );
 //   }

    gzFile fp;  
    kseq_t *seq;  
    int l; 
 
    long int n = 0, slen = 0;
    fp = gzopen(argv[i], "r"); // open the file handler  
    seq = kseq_init(fp); //  initialize seq  
    if(bflag == 1){
	  while ((l = kseq_read(seq)) >= 0) { // STEP 4: read sequence  
	      ++n, slen += seq->seq.l;
	  }  
      //    printf("%s\tread_count:%ld\tbase_count:%ld\n", argv[i], n, slen); 
                if(oflag){
                  count_results2[argv[i]] = std::make_pair(n, slen );
                 } else {
		   
		   if(lflag){
		     float avgl =  slen / n;
                     printf("%s\t%ld\t%ld\t%.0f\n", argv[i], n, slen, avgl); 
		   } else {
		     printf("%s\t%ld\t%ld\n", argv[i], n, slen); 
		   }
		 }
      //    printf("%s\t%ld\n", argv[i], n); 
    }else{
	  while ((l = kseq_read(seq)) >= 0) { // STEP 4: read sequence  
	      ++n;//, slen += seq->seq.l;
	  }  
      //    printf("%s\tread_count:%ld\tbase_count:%ld\n", argv[i], n, slen); 
      //    printf("%s\t%ld\t%ld\n", argv[i], n, slen); 
      if(oflag == 1){
          count_results[argv[i]] = n ;
      } else {
          printf("%s\t%ld\n", argv[i], n); 
      }
    }
    kseq_destroy(seq); // STEP 5: destroy seq  
    gzclose(fp); // STEP 6: close the file handler  
    
    }
    //std::cout << "================\n";
    if((oflag == 1)&&(bflag == 0)){
	  for(std::map<std::string, long>::const_iterator it = count_results.begin();
	      it != count_results.end(); ++it)
	  {
	      std::cout << it->first << "\t" << it->second << "\n";
	  }  
    }
 
     std::cout << std::setprecision(0) << std::fixed;
    // float avgl;
    if((oflag == 1)&&(bflag == 1)){
	  for(std::map<std::string, std::pair<long, long> >::const_iterator it = count_results2.begin();
	      it != count_results2.end(); ++it)
	  {
	   // avgl = second.second/second.first;of
	    if(lflag == 1){
	    std::cout << it->first << "\t" << it->second.first << "\t" << it->second.second << "\t" << (it->second.second / it->second.first)  << std::endl;
	    } else {
	      std::cout << it->first << "\t" << it->second.first << "\t" << it->second.second << std::endl;
	    }
	  }  
    }
 
    return 0;  
}  
