###
#
# Software controller for button sorter
#
# Serial command set:
#    positions
#    a - left side extreme
#    b - right side extreme
#    c - center position
#    e - ID plate position
#
#    end effector actions
#    u - raise actuator
#    d - lower actuator
#    l - midposition of actuator
#
#    sorting mechanism actions
#    r - turn clockwise
#    t - turn counter-clockwise
#    s - return to home position
#
#    utility
#    p - print status
###

#Standards
import math,time,random

#Import serial communication
import serial

#Start serial port
ser = serial.Serial("COM8")

#Function to get a transmission on the serial port
def get_packet(P,blocking):

    if P == None: #If no packet awaiting
        while (ser.inWaiting() > 0): #Read in serial data
            ser.read()
        ser.write('p') #Send command to print status
        while (ser.inWaiting() == 0)&blocking: #Blocking request send period until data on port again
            delay(100)
        S = '' #Output string
        s = '' #parse segment line
        while s != '\n': #While not at end of line
            s = ser.read() #Read in data
            if s != -1 and s != '\n': #If not empty or end, add to output
                S = S + s
    else: #If a packet available
        S = P #Output is packet values
    S = S.split(",") #Split by delimiter
    #print S #Optional report of reading
    D = {} #Output dictionary
    for a in S: #Append all values with label keys
        D[a[0]] = int(a[1:])
    return D #Return dictionary

def to_states(D):
    #Utility function to convert dictionary parse to variables
    a = D['a'] #Grab all relevant keys
    b = D['b']
    c = D['c']
    e = D['e']
    d = D['d']
    u = D['u']
    t = D['t']

    #Convert QTR reading to human
    r = "top"*(D['r'] <=3 ) + "bottom"*(D['r'] >=4)*(D['r'] <= 7) + "None"*(D['r'] >= 8) 
    return a,b,c,e,d,u,t,r
    
def send_commands(s):
    #Helper function to send a command set
    for a in s: #For each command char
        ser.write(a) #Send command 
        time.sleep(0.1); #Short receipt delay

def delay(duration):
    #Utility delay function
    time.sleep(duration/1000.0)

#System initialization
delay(1000) #Wait 1 second
send_commands('a') #Send to left side

delay(1000) #Wait a second
send_commands('b') #send to right side

delay(1000) #Wait a sec
send_commands('a') #Back to left side

delay(1000) #hold up
send_commands('s') #home position

#Wait until entry to start
k = input("tick")

#print(get_packet(None,1)) #optional diagnostic

while (k != 's'):

    #Send retreive and check commands:
    #  Lower actuator, snag button
    #  Raise actuatot, lift button
    #  Move to ID plate
    #  Lower actuator, deposit button
    #  Raise actuator, without button
    #  Back to left side
    send_commands("duedua")

    #Fetch the current machine state
    D = get_packet(None,1)
    a,b,c,e,d,u,t,r = to_states(D) #Convert to variables

    print(r) #Report detected button state

    #If top side up button
    if (r == "top"):
        send_commands("rwwwwwwws") #Clockwise turn, delay, home sweep armature

    #If bottom side up button
    elif (r == "bottom"):
        send_commands("twwwwwwws") #Counter-clockwise turn, delay, home sweep armature

    #If button not successfully read
    else:
        send_commands('twr')#Counter-clockwise turn, delay, clockwise turn (nudge the button)

    #print(get_packet(None,1)) #Optional diagnostic reading

    delay(100) #SMoothness delay

    #k = input("tick") #Optional manual toggle
        
    
            
