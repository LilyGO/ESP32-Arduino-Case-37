#this is to overcome Python2/3 version
try:
    #this is style for Python 3 import 
    from  tkinter import *
except ImportError:
    #in case import for Python 3 is error
    #use this for Python 2 import
    from  Tkinter import *
import sys
#similar to comments above
try:
    import paho.mqtt.publish as publish
except ImportError:
    import os
    import inspect
    cmd_subfolder = os.path.realpath(os.path.abspath(os.path.join(os.path.split(inspect.getfile( inspect.currentframe() ))[0],"../src")))
    if cmd_subfolder not in sys.path:
        sys.path.insert(0, cmd_subfolder)
    import paho.mqtt.publish as publish

#for demo I use 2 nodes
data = [   
    ['sss8', 'sss8.gif', 'SamSung S8', '800USD'],
    ['ip7', 'ip7.gif', 'iPhone 7S', '700USD']
]

#this is the simple GUI part
root = Tk()
root.wm_title("PriceTag")
rows = []
img = []
for i in range(2):
    cols = []
    for j in range(4):
        if(j == 1):
            img.append(PhotoImage(file=data[i][j]))
            e = Button(root, image=img[-1])
            e.grid(row=i, column=j, sticky=NSEW)
        elif(j == 0):
            e = Entry(root, relief=RIDGE, justify='center')
            e.grid(row=i, column=j, sticky=NSEW)
            e.insert(END, data[i][j])
            e.config(state=DISABLED)
            cols.append(e)
        else:
            e = Entry(root, relief=RIDGE, justify='center')
            e.grid(row=i, column=j, sticky=NSEW)
            e.insert(END, data[i][j])
            cols.append(e)
    rows.append(cols)
#this function will be invoked when the Update button is clicked
def onUpdate():
    msgs = []
    #we loop through all products and publish new price
    for row in rows:
        #build topic: update/pricetag/sss8 : {'name': 'samsung8', 'value': '800USD'}
        payload = str(row[1].get()) + '-' + str(row[2].get())
        #every topic is stored under dictionary like json
        pair = {'topic':'update/pricetag/'+row[0].get(), 'payload':payload}
        #here we send multiple topics instead of looping through every topic and send
        #so we append multiple topics in a list
        msgs.append(pair);
    #and pass the list of topics to publish.multiple()
    publish.multiple(msgs, hostname="localhost")
    
Button(text='Update', command=onUpdate).grid()
mainloop()