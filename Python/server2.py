import socket
import sys
 
HOST = ''   # Symbolic name, meaning all available interfaces
PORT = 8888 # Arbitrary non-privileged port

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print ('Socket created')
 
#Bind socket to local host and port
try:
    s.bind((HOST, PORT))
except socket.error as msg:
    print ('Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1])
    sys.exit()
     
print ('Socket bind complete')
 
#Start listening on socket
s.listen(10)
print ('Socket now listening')
 
conn, addr = s.accept()
print ('Connection address:', addr)

#now keep talking with the client
while 1:
    data = conn.recv(1024)
    print ("received data:", data)
    conn.send(data)  # echo
  
#data = s.recv(1024)
#sys.stdout.write(data)

#msg = sys.stdin.readline()
#s.send(msg)
   
s.close()