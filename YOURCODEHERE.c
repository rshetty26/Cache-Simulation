#include "YOURCODEHERE.h"

/*

  The way I tried to test whether or not my output was correct was to ignore any misses and only focus on cache hits that I was expecting,
  since any invalid locations contained data that would vary on different runs. Since that data was going to be irrelevant anyways, it's
  something I can ignore.

 */

unsigned int lg2pow2(uint64_t pow2){ // helper function for integer lg2; using (double) version from math is not safe
  unsigned int retval=0;
  while(pow2 != 1 && retval < 64) { /* -- should actually check the local VA bits, but, as seen below, if !64, will exit anyway...*/
    pow2 = pow2 >> 1;
    ++retval;
  }
  return retval;
}

void setSizesOffsetsAndMaskFields(cache* acache, unsigned int size, unsigned int assoc, unsigned int blocksize) {
  unsigned int localVAbits=8*sizeof(uint64_t*);
  if (localVAbits!=64) {
    fprintf(stderr,"Running non-portable code on unsupported platform, terminating. Please use designated machines.\n");
    exit(-1);
  }
  acache->numways=assoc; // sets associativity
  acache->blocksize=blocksize; // sets blocksize
  acache->numsets=size/(blocksize*assoc); // computes total number of sets
  acache->BO = lg2pow2(blocksize); // computes block offset
  acache->TO = acache->BO + lg2pow2(acache->numsets); // computes tag offset
  acache->VAImask= acache->numsets - 1; // computes AND mask for index
  acache->VATmask=(uint64_t)(((uint64_t)1)<<(localVAbits - acache->TO)) - (uint64_t)1; // computes AND mask for tag
}


unsigned long long getindex(cache* acache, unsigned long long address) {
  return (address>>acache->BO) & (uint64_t)acache->VAImask; //Returns index bits, masking upper bits
}

unsigned long long gettag(cache* acache, unsigned long long address) {
  return (address>>acache->TO) & (uint64_t)acache->VATmask; //Returns tag bits, masking upper bits
}

void writeback(cache* acache, unsigned int index, unsigned int oldestway) {
  unsigned long long size = sizeof(unsigned long long); // Get word size
  unsigned long long address = 0; // Initialize address to 0

  unsigned long long pointerTag = (*acache).sets[index].blocks[oldestway].tag;  
  unsigned long long acBO = (*acache).BO; // Get the block offset from the pointer to acache
  unsigned long long acTO = (*acache).TO; // Get the tag offset from the pointer to acache

  unsigned long long addressIndex = index << acBO; // Shift the address index by the block offset
  unsigned long long addressTag = pointerTag << acTO; // Shift the address tag by the tag offset
  
  address = address | addressIndex; // use the or operator to set the address index
  address = address | addressTag; // use the or operator to set the address tag

  for(int x = 0; x < ((*acache).blocksize / size); x++) { // loop through until you went through the whole acache by word size
    unsigned long long next = (*acache).nextcache; // get next cache
    unsigned long long word = (*acache).sets[index].blocks[oldestway].datawords[x]; // set the word size
    
    performaccess(next, address, size, 1, word, x);
    
    address = address + size; // incrememnt the address by word size
  }

}

void fill(cache* acache, unsigned int index, unsigned int oldestway, unsigned long long address) {
  unsigned long long size = sizeof(unsigned long long); // Get word size
  unsigned long long acBO = (*acache).BO; // Get the block offset from the pointer to acache
  unsigned long long acTO = (*acache).TO; // Get the tag offset from the pointer to acache

  unsigned long long buildIndex = (*acache).VAImask << acBO; // Shift the size of the index field by the block offset
  unsigned long long buildTag = (*acache).VATmask << acTO; // Shift the size of the tag field by the tag offset
  
  address = address & (buildTag | buildIndex); // clear the tag and index from the address

  for(int x = 0; x < (acache->blocksize / size); x++){
    unsigned long long next = (*acache).nextcache; // get next cache
    unsigned long long word = performaccess(next, address, size, 0, 0, x); // get the data
    
    (*acache).sets[index].blocks[oldestway].datawords[x] = word; // update the data
    (*acache).sets[index].blocks[oldestway].tag = gettag(acache, address); // update the tag value with the new tag
    
    address = address + size; // incrememnt the address by word size
  }
}
