# lock in cpp 

## types of lock in linux/unix 

* spinlock 
```
feature:
keep checking whether the condition changing and release the lock, before release spinlock cpu always in busy state  

scenario:
* in short lock's scenarios use spinlock will reduce cpu context switching raised resource consuming  
* in long lock's scenarios will increase cpu resource consuming  
```


* semaphore 
```
atomic counter provided by the os, in some standards if the counter < 0 means the lock is occupied by other thread, otherwise the lock is not occupied or in idle state.  
```

* mutex 
```
mutex's implementation also based on atomic counter but implemented logic is more complex than atomic counter, mutex adopt lots of strategies to cut down cpu overhead. 
```

## why need lock?
make sure resouce only be accessible for one unique task
or make sure that during the task modifying one resource, the resource cannot be accessed by other tasks 

## Protocol of MESI 
* what's MESI protocol 
The MESI protocol is a method to maintain the coherence of the cache memory content in hierachical memory systems. 
MESI protocol is based on four possible states of the cache blocks: Modified, Exclusive, Shared and Invalid.

Each accessed block is in one of these stages and the transitions among them define the MESI protcol.

MESI protocol often adopted in the multi-core CPU architecture. 

And the `MESI` is referring to M(Modified), E(Exclusive), S(Shared) and I(Invalid).

### Modified 
* current cpu core cached data value will not appear in other cpu core's cache 

### Exclusive 
* current cpu core cached data value will not apprear in other cpu core's cache
* current cpu core cached data value not modified by current cpu core 

### Shared 
* current cpu core cached data value is shared among at least 1 cpu core 

### Invalid
* cached data value can be occupied by other cpu cores 

### MESI Message Structure 
* read: read order, this order should contaisn an validate and accessible address, when cpu recieves this order, it will extract the address and the get the value for the address
 
* read response: after the above read operation is executed successfully, it will encapsulate the data value (from the read order's address) and respond. read order may be from main memory or the other cpu cores. 

* invalidate: invalidate order, this order should contains an validate address, when receive this order , the cpu will extract the address and then invalidate the address(tag it as invalide) and then response a message of invalidate acknowledge(it invalidate operaiton is executed successfully).
 
* invalidate acknowledge: invalidate acknowledge response message, when cpu decides invalidate an address successfully, it will resposne an (adddress) invalidate ok message this is the invalidate acknowledge

* read invalidate: read invalid order, after receiving this order will trigger action: read data value from the `invalidate` taged address, it is the order combines both read order and invalidate order. after read data value from the read order's specified address, then set the address state to invalidate, and then return both read response and invalidate acknowledge response. 

* writeback: write message, after receiving this order will trigger action:  write data value from current cache to main memory, and at the same time allow data values that tagged `modified` poped from cache 
