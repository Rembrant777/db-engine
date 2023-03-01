# Question List 
##  what's the difference between socket and rpc
### socket 
sockets are software structures that allow two processes to communicate over a network. 
it mainly responds to the need for functions that enable remote information exchange. 
additionalyy, we use sockets in a centralized process of communication. 

and what's more socket combines an ip address and port number. an ip address is a unique number that 
identifies a machine in the network. additionally, a port number is a unique logical identifier for a process in the network. 
it helps to identify a process. 

in general, we use two major communication modes in socket programming: a non-connected UDP mode and a connected TCP mode. 

a socket is an endpoint of a distributed communication machine that uses sockets to implement the client-server mode and solve different network applications.
the distributed communication requires two sockets provided as two endpoints: one in the client and one in the server. 

thus, the server sockets are generally responsible for receiving information from client sockets and responding to them based on the 
client-server application request. in contrast, there's no possibility of finding an ip address and port number bound to different process in a single machine in network communicaiton. 

however, if a client knows the server's IP address and port number, it is possible to create a socket communcaiton between two machines. 

### rpc
the rpc is a communication protocol between processes that allows a machine to execute specific procedures using parameters from another machine across a network. 
it's generally implemented in distributed systems, where different sites in different locations are physically separated. rpc is a high-level paradigm that considers 
the existence of transmission protocols during implementation.


### difference poitns
|--- | ---|
| socket | rpc | 
| software structure used to maintain communcation. | a protocol of communciation | 
| independent structure that can be used a long to create connection |  uses sockets to provide connection | 
| developers must set configuration of every socket |  setting configuration is not required | 
| lower layer with complex code but gerater flexibility and efficiency | higher layer with simple code but lower efficiency | 
| dependes on it own functions | depends on the socket functions with some additional functions | 
| programming with sockets allows only two modes of connections the udp and tcp | rpc can be implemented in variety of protocols | 
| difficult and time consuming due to the natural constrains of network communciation | less difficult and time consuming |     


* diff points
> socket communication more suitable for client-server mode, while the rpc is more suitlbe for distributed 
> rpc communcation devs can define more complex protocols complex computing will not executed on the client side, client side only execute proxy function on local env and send the calculation requests to remote side, and the server side in charge of receive the calculation request, parse the calculated parameters from request, execute calculation, get the result and encapsulate the calculated result into response body and finally return the response body to client.  i think socket is more suitable small amount data or connection requests process? (i'm not sure), rpc is more standard and stable for large amout of data/request process and suitable for distributed env. 
> however the socket and related socket based functions are more closely to the posix(not sure actually i am not figure out the relationship between posix, system call and kernel mode yet) or kernel functions, so i think invoke socket apis is more efficient and more flexible than calling rpc (middleware layer take google protobuf as an instance, codes need to be translated from the proto and then the native coding language like c, cpp or java). 


##  should I re-factor current's server - client communication mode from socket into rpc (based on the google protobuf framework) ?  if so what apis should be maintained and what apis should be deprecated ? 
for now i prefer socket, if there are extra time i would like to figure out the core apis and refactor the project based on protobuf   
