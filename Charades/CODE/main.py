from tkinter import Button
from kivymd.app import MDApp
from kivy.core.window import Window
from kivy.uix.widget import Widget
from kivy.utils import get_color_from_hex
from kivy.config import Config
from kivy.uix.behaviors import ToggleButtonBehavior
from kivy.uix.togglebutton import ToggleButton
from kivy.graphics import Color, Line
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.textinput import TextInput
from kivymd.uix.dialog import MDDialog
from kivy.lang import Builder
import socket
import threading
import sys
import signal
import time


Config.set('graphics', 'width', '960')
Config.set('graphics', 'height', '540')
Window.clearcolor = get_color_from_hex('#ffffff')

class AnswerInput(BoxLayout):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        b1 = TextInput(text = "a")
        b2 = Button(text = "b")
        self.add_widget(b1)
        self.add_widget(b2)


class MyButton(ToggleButton):
    def _do_press(self):
        if self.state == 'normal':
           ToggleButtonBehavior._do_press(self)

class Box(BoxLayout):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        b1 = Button(text = "a")
        b2 = Button(text = "b")
        self.add_widget(b1)
        self.add_widget(b2)

class CanvasWidget(Widget):
    line_width = 2
    wordlen = 0
    cordpoint = []
    color = []
    word = ""
    scores = ""
    wordpass = "x73pass"
    hostpass = "x73host"
    drawpass = "fx73draw"
    clearpass = "x73clear"
    colorpass = "fx73color"
    scorepass = "x73scor"
    host = False
    clearpressed = False
    changedcolor = False
    receivedcolor = False
    clearreceived = False
            
    def __init__(self, **kwargs):
        super(CanvasWidget, self).__init__(**kwargs)

    def drawline(self, argpoints, argwidth):
        with self.canvas:
            Line(points=argpoints, width=argwidth)
        #     print("tu dziala")
        # print("a tu dziala")

    def set_color(self, new_color):
        if self.host or self.receivedcolor:
            self.last_color = new_color
            self.canvas.add(Color(*new_color))
            self.color = self.last_color
            if self.host:
                self.changedcolor = True

    def on_touch_down(self, touch):
        if Widget.on_touch_down(self, touch) and self.host:
            return
        with self.canvas:
            touch.ud['current_line'] = Line(points=(int(touch.x), int(touch.y)), width=self.line_width)

    def on_touch_move(self, touch):
        if 'current_line' in touch.ud and self.host:
            x = int(touch.x)
            y = int(touch.y)
            touch.ud['current_line'].points += (x, y)
            # self.cordpoint = self.drawpass + " " + str(x) + " " + str(y)
            # print(touch.ud['current_line'].points)

    def on_touch_up(self, touch):
        if 'current_line' in touch.ud and self.host:
            self.cordpoint = touch.ud['current_line'].points
            # add drawing points to send to other clients
            self.cordpoint.append(self.drawpass)
            self.cordpoint.insert(0, self.line_width)
            # print(self.cordpoint)
            

    def clear_canvas(self):
        if self.host or self.clearreceived:
            if self.clearreceived:
                self.receivedcolor = True
            if self.clearreceived == False:
                self.clearpressed = True
            saved = self.children[:]
            self.clear_widgets()
            self.canvas.clear()
            for widget in saved:
                self.add_widget(widget)
            time.sleep(0.2)
            self.set_color(self.color)

    def set_line_width(self, line_width='Normal'):
        self.line_width = {'Thin': 1, 'Normal': 2, 'Thick': 4}[line_width]

class PaintApp(MDApp):

    nickname = ""
    client = 0
    chathistory = []
    chatstring = ""
    chatchanged = False

    def build(self):
        self.canvas_widget = CanvasWidget()
        self.canvas_widget.set_color(get_color_from_hex('#2980b9'))
        self.init_client()
        return self.canvas_widget

    def catch_ctrl_c_and_exit(self, sig, frame):
        print("You pressed Ctrl+c")
        sys.exit(0)

    # Sending Messages To Server
    def write(self):
        while True:
            # send coordinates when something has been drown
            if self.canvas_widget.cordpoint != []:
                message = '{}: {}'.format(self.nickname, self.canvas_widget.cordpoint)
                self.canvas_widget.cordpoint = []
                self.client.send(message.encode('ascii'))
            #  send new color when it has been changed
            elif self.canvas_widget.changedcolor:
                self.canvas_widget.changedcolor = False
                sendedcolor = self.canvas_widget.color[:]
                sendedcolor.append(self.canvas_widget.colorpass)
                message = '{}: {}'.format(self.nickname, sendedcolor)
                # print("wyslany color", message)
                self.client.send(message.encode('ascii'))
            elif self.canvas_widget.clearpressed:
                self.canvas_widget.clearpressed = False
                message = '{}: {}'.format(self.nickname, self.canvas_widget.clearpass)
                # print("clear pressed", message)
                self.client.send(message.encode('ascii'))
            elif self.chatchanged:
                self.chatchanged = False
                message = '{}: {}'.format(self.nickname, self.chathistory[-1])
                self.client.send(message.encode('ascii'))
        self.client.close()

    # Listening to Server and Sending Nickname
    def receive(self):
        while True:
            try:    # Receiving drawing coordinates from other client
                message = self.client.recv(1024).decode('ascii')
                if message.find(self.canvas_widget.drawpass) > 0:
                    temp_start = message.find('[')+1
                    temp_end = -(len(self.canvas_widget.drawpass)+5)
                    message = message[temp_start:temp_end]
                    message = list(map(int, list(message.split(", "))))
                    self.canvas_widget.drawline(message[1:], message[0])

                elif message.find(self.canvas_widget.colorpass) > 0:
                    temp_start = message.find('[')+1
                    temp_end = -(len(self.canvas_widget.colorpass)+5)
                    message = message[temp_start:temp_end]
                    message = list(map(float, list(message.split(", "))))
                    # print("dostarczony color", message)
                    # print("color", self.canvas_widget.color)
                    self.canvas_widget.receivedcolor = True
                    if message != self.canvas_widget.color:
                        self.canvas_widget.set_color(message)
                    self.canvas_widget.receivedcolor = False

                elif message.find(self.canvas_widget.clearpass) > 0:
                    self.canvas_widget.clearreceived = True
                    self.canvas_widget.clear_canvas()
                    self.canvas_widget.clearreceived = False

                elif message.find(self.canvas_widget.hostpass) > 0:
                    # print("jestem hostem: ", message)
                    self.reset()
                    self.canvas_widget.host = True
                    temp_start = message.find(':')+2
                    message = message[temp_start:]
                    # print("slowo: ", message)
                    self.canvas_widget.word = message
                    self.updatewordlabel()

                elif message.find(self.canvas_widget.wordpass) > 0:
                    self.canvas_widget.host = False
                    self.reset()
                    temp_start = message.find(':')+2
                    message = message[temp_start:]                    
                    self.canvas_widget.wordlen = int(message)
                    # print("otrzymana dlugosc slowa: ", self.canvas_widget.wordlen)
                    self.updatewordlabel()

                elif message.find(self.canvas_widget.scorepass) > 0:
                    # print("wyniki: ", message)
                    temp_start = message.find(':')+2
                    message = message[temp_start:]
                    message = message.replace(";", "\n\n")
                    self.canvas_widget.scores = message
                    # print("uciete: ", message)
                    self.updatescoreslabel()

                else:
                    print(message)
                    if len(self.chathistory) > 15:
                        self.chathistory = []
                        self.chatstring = ""
                    self.chathistory.append(message)
                    self.chatstring += message + "\n"
                    self.logger()
            except:
                # Close Connection When Error
                print("An error occured!")
                self.client.close()
                break
        self.client.close()

    def init_client(self):
        # input processing
        if len(sys.argv) != 2:
            print(f"Usage: {sys.argv[0]} <port>")
            exit()

        PORT = int(sys.argv[-1])

        signal.signal(signal.SIGINT, self.catch_ctrl_c_and_exit)

        # Choosing nickname
        nickname = input("Choose your nickname: ")
        if len(nickname) < 2 or len(nickname) > 32:
            print("Name must be less than 30 and more than 2 characters")
            sys.exit(1)

        # Connecting To Server
        self.client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            self.client.connect(('127.0.0.1', PORT))
        except:
            print("Server is not responding!")
            self.client.close()
            sys.exit(1)

        # sending nickname
        self.nickname = nickname
        self.client.send(nickname.encode('ascii'))
        print("--- WELCOME IN LOBBY ---")

        # Starting Threads For Listening And Writing
        try:
            receive_thread = threading.Thread(target=self.receive)
            receive_thread.start()
        except:
            print("ERROR: threading read")
            sys.exit(1)

        try:
            write_thread = threading.Thread(target=self.write)
            write_thread.start()
        except:
            print("ERROR: threading write")
            sys.exit(1)

    def delete(self):
        self.root.ids.user.text = ''

    def reset(self):
        self.chathistory = []
        self.chatstring = ""
        self.logger()

    def updatescoreslabel(self):
        self.root.ids.scores.text = f'{self.canvas_widget.scores}'

    def generatewordhints(self):
        self.canvas_widget.word = ""
        for i in range(self.canvas_widget.wordlen):
            self.canvas_widget.word += "_ "

    def updatewordlabel(self):
        if not self.canvas_widget.host:
            self.generatewordhints()
        self.root.ids.password.text = f'{self.canvas_widget.word}'

    def convertlisttostring(self):
        self.chatstring += "You: " + self.chathistory[-1] + "\n"

    def updatechat(self):
        if len(self.chathistory) > 15:
            self.chathistory = []
            self.chatstring = ""
        self.chathistory.append(self.root.ids.user.text)
        self.convertlisttostring()
        self.chatchanged = True
    
    def logger(self):
	    self.root.ids.chat.text = f'{self.chatstring}'


if __name__ == "__main__":
    PaintApp().run()
