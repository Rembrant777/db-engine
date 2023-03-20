# lock in cpp 

## types of lock in linux/unix 

* spinlock 
```
keep checking whether the condition changing and release the lock 
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


