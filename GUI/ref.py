from PyQt6 import QtWidgets, uic, QtGui
import sys
import numpy as np

from std_msgs.msg import String, Int32MultiArray, Float32, Float64, Float32MultiArray, Int32
# import matplotlib.pyplot as plt
# from matplotlib.animation import FuncAnimation
# from  matplotlib.backends.backend_qt5agg  import  ( NavigationToolbar2QT  as  NavigationToolbar ) 
 
from itertools import count
import random

class Ui(QtWidgets.QMainWindow):
    def _init_(self):
        super(Ui, self)._init_()
        uic.loadUi('VAAYU.ui', self)
        self.show()
        self.nodebtn.clicked.connect(self.fxn2)
        icon = QtGui.QIcon()
        self.setWindowTitle("VAAYU DATA")
        # self.graphbtn.clicked.connect(self.fxnplt)
        self.pushButton_3.clicked.connect(self.start_roscore)
        # self.addToolBar(NavigationToolbar(self.MplWidget.canvas, self.MplWidget))

    def start_roscore(self):
        parent.start()
    def fxn2(self):
        # rospy.init_node('subsciber_node', anonymous=True)
        # rospy.Subscriber('motor_vals', Int32MultiArray, self.fxntyre)
        rospy.init_node('gui_node', anonymous=True)
        # rospy.Subscriber('talkingtopic', Float64, self.fxnplt)
        rospy.Subscriber('MQ135_1_readings', Float32, self.fxnmq135_1)
        rospy.Subscriber('MQ135_2_readings', Float32, self.fxnmq135_2)
        rospy.Subscriber('bmp1_readings', Float32, self.fxnbmp1)
        rospy.Subscriber('dht1_reading1', Float32, self.fxndht1)
        rospy.Subscriber('dht1_reading2', Float32, self.fxndht2)
        rospy.Subscriber('SHT1_reading1', Float32, self.fxnsht11)
        rospy.Subscriber('SHT1_reading2', Float32, self.fxnsht12)
        rospy.Subscriber('SHT2_reading1', Float32, self.fxnsht21)
        rospy.Subscriber('SHT2_reading2', Float32, self.fxnsht22)
        rospy.Subscriber('MLX_reading1', Float32, self.fxnmlx1)
        rospy.Subscriber('MLX_reading2', Float32, self.fxnmlx2)
        rospy.Subscriber('load1_readings', Float32, self.fxnload1)
        rospy.Subscriber('load2_readings', Float32, self.fxnload2)
        rospy.Subscriber('load3_readings', Float32, self.fxnload3)



    def fxnmq135_1(self, msg):
        s_one=str(msg.data)
        self.data_AQG1.setText(s_one+'ppm')

    def fxnmq135_2(self, msg):
        s_two=str(msg.data)
        self.data_AQG2.setText(s_two+'ppm')

    def fxntyre(self, msg):
        left=str(msg.data[0])
        right=str(msg.data[1])
        self.label_l.setText(left)
        self.label_r.setText(right)

    def fxnbmp1(self, msg):
        temp=str(msg.data)
        self.data_Baro1.setText(temp+'atm')

    def fxndht1(self,msg):
        temp=str(msg.data)
        self.data_DHT1.setText('Humidity='+temp+'%')

    def fxndht2(self,msg):
        temp=str(msg.data)
        self.data_DHT2.setText('Temp='+temp+'°C')

    def fxnsht11(self,msg):
        temp=str(msg.data)
        self.data_SHT11.setText(temp+'%')

    def fxnsht12(self,msg):
        temp=str(msg.data)
        self.data_SHT12.setText(temp+'°C')

    def fxnsht21(self,msg):
        temp=str(msg.data)
        self.data_SHT21.setText(temp+'%')

    def fxnsht22(self,msg):
        temp=str(msg.data)
        self.data_SHT22.setText(temp+'°C')

    def fxnmlx1(self, msg):
        temp=str(msg.data)
        self.data_MLX1.setText(temp+'°C')

    def fxnmlx2(self, msg):
        temp=str(msg.data)
        self.data_MLX2.setText(temp+'°C')    

    def fxnload1(self, msg):
        temp=str(msg.data)
        self.data_load1.setText(temp+'g')

    def fxnload2(self, msg):
        temp=str(msg.data)
        self.data_load2.setText(temp+'g')

    def fxnload3(self, msg):
        temp=str(msg.data)
        self.data_load3.setText(temp+'g')




    # def fxnsetdummy2(self, msg):
    #     load=str(msg.data+101325)
    #     load1=str(msg.data)
    #     self.data_load.setText(load1+'g')
    #     self.data_Lcell.setText(load1+'g')

    #     temp1=str(random.randint(25,27))
    #     mst1=str(random.randint(1,10))
    #     aqg1=str(random.randint(110,115))


    #     self.data_SHT1.setText(temp1+'°C')
    #     self.data_MST1.setText(mst1+'%')
    #     self.data_Baro1.setText(load)

    #     self.data_AQG2.setText(aqg1+'ppm')
    #     self.data_TH2.setText(temp1+'Rh')
    #     self.data_MST2.setText(mst1+'%')
    #     self.data_Baro2.setText(load+'Pa')

    # def fxnplt(self, msg):

    #     # self.data_AQG1.setText(msg)
    #     # fs = 500
    #     # f = 50
    #     # ts = 1/fs
    #     # length_of_signal = 100
    #     # t = np.linspace(0,1,length_of_signal)
    #     # rospy.loginfo("Rec %f", msg)
    #     reqd=(int(msg.data))
    #     print("~~~~%f~~~~" %reqd)
    #     press_data=[]
    #     index=count()
    #     t=[]

    #     def fxnplt2(i):
    #         press_data.append(reqd)
    #         # print(i)
    #         # press_data.append(random.randint(0,5))
    #         t.append(next(index))
    #         self.MplWidget.canvas.axes.plot(t,press_data)
    #     # sinus_signal = np.sin(2*np.pi*f*t)
    #     pres_d=FuncAnimation(self.MplWidget, fxnplt2, interval=1000)
    #     # self.MplWidget.canvas.axes.clear()
    #     self.MplWidget.canvas.axes.legend(('Pressure'),loc='upper right')
    #     self.MplWidget.canvas.axes.set_title('Pressure Measurement Plot')
    #     self.MplWidget.canvas.draw()

    #     plt.show()


parent = ROSLaunchParent('run_id', [], is_core=True)     # run_id can be any string

app = QtWidgets.QApplication(sys.argv)
window = Ui()
app.exec_()

parent.shutdown()