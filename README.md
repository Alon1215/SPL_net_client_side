# SPL_net_client_side
A books-club platform,  reactor based server - client project. (the client side) 
Project in System Programming course ( in Ben Gurion University) , writing Reactor server in java, and client side in C++, a tpc server is also provided, and can be chosen as an argument while running the server. the 2 commands for running the server (via terminal):

Thread per client server: ` mvn exec:java -Dexec.mainClass="bgu.spl.net.impl.stomp.StompServer" - Dexec.args=" tpc’
Reactor server: ` mvn exec:java -Dexec.mainClass="bgu.spl.net.impl.stomp.StompServer" - Dexec.args=" reactor’
