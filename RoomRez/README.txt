Unix Final Project

Group:  
Zeid Ayssa, Kaleb Dufera, Michael Surratt, Alazar Hailemariam, Brian Chen

Instructions:

1. Download and save files under one folder.

2. Under the terminal, change to that directory and type the following commands:
$ qmake
$ make

3. After successfull compilation, type the following command to initiate this application:
$ ./RoomRez


Summary

Our application is a booking manager for rooms on the Virginia Tech campus. A drop-down menu reveals a listing for all buildings on grounds, and a second drop-down menu allows users to select a room number therein. After specifying date and time, a simple click of a button reserves that room for that specified time slot. The application does check for errors and parameters, rejecting a reservation due to invalid time frames and previous bookings. After a successful booking, the application assigns that booking a number that we can use to reference it later. This is mainly to keep track of our bookings, and allows us to cancel reservations by entering it into a box we provide below. The entire system is simple, yet practical.
