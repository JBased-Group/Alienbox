#ifndef COUNTER_H
#define COUNTER_H

#define COUNTER_CONCAT_A(a,b,c,d) 0x ## a ## b ## c ## d
#define COUNTER_CONCAT(a,b,c,d) COUNTER_CONCAT_A(a,b,c,d)

#define COUNTER_A 0x0000
#define COUNTER_B 0x0000

#define INCREMENT_COUNTER_A "counterAinc.h"
#define INCREMENT_COUNTER_B "counterBinc.h"


#endif