# dga: dynamic generic arrays

dga is a tiny C library for creating generic and typesafe dynamic arrays which
are also binary compatible with native C arrays.

The idea comes from the library `sds` from Antirez the creator of Redis. Instead of
creating a structure that contains the buffer and the length like most string libraries
do/did, `sds` puts a header at the beginning of the buffer that contains the metadata,
and returns a pointer to the buffer + sizeof(header). This way `sds` guarantees
binary compatibility with the standard string functions.

dga takes the `sds` approach one step further, and implements the same idea in a way that can
be used with any C type, including variably modified types.

# example
```c
#include <stdio.h>
#include "dga.h"

int main() {
    /* dga_new() creates and initializes the header. 
     * If the first argument is any number greater than
     * zero, it already allocates that amount. 
     */

    int *iarr = dga_new(4, int);
    /* iarr can now be manipulated as if it was declared int iarr[4]; */
    iarr[0] = 0;
    iarr[1] = 1;
    iarr[2] = 2;
    iarr[3] = 3;
    
    /* you can also use the dga_len(void *arr) function to retrieve the length,
     * since it's constantly tracked anyway. And notice that we pass the normal array.
     */
     for (size_t i = 0; i < dga_len(iarr); i++) {
         iarr[i] = i * 2;
     }
     
     /* you can allocate more space for the array by either using dga_grow(arr, n)
      * or dga_resize(arr,  * n).
      */
      iarr = dga_grow(iarr, 2); // grow the array by 2 -> iarr[6]
      iarr = dga_resize(iarr, 8); // resize the array to size 8 -> iarr[8]
      iarr = dga_shrink(iarr, 2); // shrink the array by 2 elements -> iarr[6]
      
      /* dga's approach has a couple of pitfalls, and one of them being
       * pointer invalidation. So every operation that can change the size of the array
       * returns a new pointer which might or might not be the same as the one passed.
       * If running out of memory is a real concern for you, then there's no way out of
       * the following pattern.
       */
       
       int *tmp = dga_grow(iarr, 2);
       if (tmp) {
           iarr = tmp; 
       } else {
           // the array is not freed if resizing fails, so if it's either resize or die
           // you should do.
           dga_free(iarr);
           return 1;
       }
       
       dga_free(iarr);
       return 0;
}
```