## :thinking: What's it about?
This project is a real-time multiplayer game based on socket programming. This game is a reflection of a popular charades game. 
One player is chosen as a drawer in in the code, we called such a person 'host'. 
The host gets a word drawn by the system, which only he can see it and other players can only see length of the word.
The goal of the host is to draw the indicated word so that the other players can get themguess. 
Players enter a hidden word in the space provided for that. 
Next by click button 'Send answer!' written word is displayed in a 'chat' where it is visible to all players. 
The round ends in the moment when someone guesses the word. Then person why guessed gets a point and the new round begins i.e. new host and word are choosen. 
Game ends when each person has already been a host. The player with the most points wins.

<p align="center">
  <img src="https://github.com/Mieszko46/DevPortfolio/blob/main/Charades/gallery/charades_sample.gif" width="860" height="500">
</p>

## 🖥️ Programming features
- **C** (Server)
- **Python** (Client)
- (framework) **Kivy**

## 🏙️ Environment requirements
- operation system -> **Linux**
- compiler -> **GCC**

Additionaly must be installed Kivy and Kivymd.

## 🚀 How to launch?
If you have all requirements then open one console for server and from 2-10 consoles for players.
To run server first you have to run makefile by just typing *'make'* in the console, then type: *./server.out 'port_nr'* for example in port_nr you can put *1100*.
To run client you have to put client code into kivymd directory, and already had set up kivy environment variable. If you have these then type in console *python main.py 'port_nr'* where port_nr must be the same as you put while starting the server.

