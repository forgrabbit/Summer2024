# Single Color RGB565 Blob Tracking Example
#
# This example shows off single color RGB565 tracking using the OpenMV Cam.

import sensor, image, time, math
import pyb,time
from pyb import UART,LED,Pin


threshold_index = 0 # 0 for red, 1 for green, 2 for blue

# Color Tracking Thresholds (L Min, L Max, A Min, A Max, B Min, B Max)
# The below thresholds track in general red/green/blue things. You may wish to tune them...
thresholds = [(16, 86, 32, 127, -19, 86), # generic_red_thresholds
              (30, 100, -64, -8, -32, 32), # generic_green_thresholds
              (0, 30, 0, 64, -128, 0),# generic_blue_thresholds
              (31, 83, 30, 83, -18, 58),#redball
              (30, 91, -39, 11, 33, 71),#球网整体
              (57, 69, -45, -14, 28, 46),#球框
              (28, 86, -40, 11, 25, 57)] #测试样例

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
sensor.set_auto_exposure(False)
clock = time.clock()
LEDB = LED(3)                  # 蓝色LED

uart = UART(3, 115200)         # 初始化串口3，波特率115200 --》母板上 UART接口

tag_families = 0
tag_families |= image.TAG16H5  # comment out to disable this family
tag_families |= image.TAG25H7  # comment out to disable this family
tag_families |= image.TAG25H9  # comment out to disable this family
tag_families |= image.TAG36H10  # comment out to disable this family
tag_families |= image.TAG36H11  # comment out to disable this family (default family)
tag_families |= image.ARTOOLKIT  # comment out to disable this family

def family_name(tag):
    if tag.family() == image.TAG16H5:
        return "TAG16H5"
    if tag.family() == image.TAG25H7:
        return "TAG25H7"
    if tag.family() == image.TAG25H9:
        return "TAG25H9"
    if tag.family() == image.TAG36H10:
        return "TAG36H10"
    if tag.family() == image.TAG36H11:
        return "TAG36H11"
    if tag.family() == image.ARTOOLKIT:
        return "ARTOOLKIT"
# Only blobs that with more pixels than "pixel_threshold" and more area than "area_threshold" are
# returned by "find_blobs" below. Change "pixels_threshold" and "area_threshold" if you change the
# camera resolution. "merge=True" merges all overlapping blobs in the image.

def find_max(blobs):
    max_size=0
    for blob in blobs:
        if blob[2]*blob[3] > max_size:
            max_blob=blob
            max_size = blob[2]*blob[3]
    return max_blob

green_last_position = 2   #默认右转
green_med = 0
obstruction = False

while True:
    clock.tick()  # 保持固定帧率，以控制循环速度
    img = sensor.snapshot()  # 获取摄像头的当前图像帧

    '''for tag in img.find_apriltags(families=tag_families):
        if tag.family() == image.TAG25H9:
            img.draw_rectangle(tag.rect(), color=(255, 0, 0))
            img.draw_cross(tag.cx(), tag.cy(), color=(0, 255, 0))
            uart.write("%d" % 7)
            print(f"Detected TAG25H9 AprilTag with ID {tag.id()}, rotation {math.degrees(tag.rotation())} degrees")
        else:
            img.draw_rectangle(tag.rect(), color=(255, 0, 0))
            img.draw_cross(tag.cx(), tag.cy(), color=(0, 255, 0))
            print_args = (family_name(tag), tag.id(), (180 * tag.rotation()) / math.pi)
            print("Tag Family %s, Tag ID %d, rotation %f (degrees)" % print_args)'''


    #print(red_last_position)
    #print(green_last_position)



    # 在图像中查找红及绿色色块
    blobs_red = img.find_blobs([thresholds[3]])
    if blobs_red:
        blob_red = find_max(blobs_red)
    blobs_green = img.find_blobs([thresholds[4]])
    if blobs_green:
        blob_green = find_max(blobs_green)
        green_cx = blob_green.cx()
        green_cy = blob_green.cy()


        img.draw_edges(blob_green.min_corners(), color=(255, 0, 0))  # 在色块的最小角点画边界
        img.draw_line(blob_green.major_axis_line(), color=(0, 255, 0))  # 画出色块的主轴线
        img.draw_line(blob_green.minor_axis_line(), color=(0, 0, 255))  # 画出色块的次轴线

        #print("green_area:" ,blob_green.area())
        if blob_green.area()>17000: #距离球门过近，
            uart.write("%d" % 5)
            print("5")
            obstruction=False
            near_goal = True
            continue


        #记录上一次绿门出现时左中右位置
        if green_cx > 2*img.width() // 7 and green_cx < 5* img.width() // 7:  #色块在中间
            green_last_position = 1

        elif green_cx < 2*img.width() // 7:  # 色块在右侧
            green_last_position = 3

        elif green_cx >= 5* img.width() // 7:  # 色块在左侧
            green_last_position = 2

        if green_cx<img.width()//2:
            green_med = 0
        else:
            green_med = 1

    flag_tag = False

    for tag in img.find_apriltags(families=tag_families):
        if tag.family() == image.TAG25H9:   # 如果是 TAG25H9 家族的 AprilTag
            img.draw_rectangle(tag.rect(), color=(255, 0, 0))
            img.draw_cross(tag.cx(), tag.cy(), color=(0, 255, 0))
            #print(f"Detected TAG25H9 AprilTag with ID {tag.id()}, rotation {math.degrees(tag.rotation())} degrees")
            if tag.id() ==0:
                if tag.cx()>50 and tag.cx()<110 :
                    #obstruction = True
                    if green_med ==0:
                        uart.write("%c"% 'd')  #球门在右边
                        print("d")
                    else:
                        uart.write("%c"% 'e')  #球门在左边
                        print("e")
                flag_tag = True
            elif tag.id() ==1:
                if tag.cx()>50 and tag.cx()<110 :
                    #obstruction = True
                    uart.write("%c"% 'f')
                    print("f")
                flag_tag = True
            elif tag.id() ==2:
                if tag.cx()>50 and tag.cx()<110 :
                    #obstruction = True
                    uart.write("%c"% 'g')
                    print("g")
                flag_tag = True

        else:
            # 处理其他类型的 AprilTag 或未知标签家族
            img.draw_rectangle(tag.rect(), color=(255, 0, 0))
            img.draw_cross(tag.cx(), tag.cy(), color=(0, 255, 0))
            print_args = (family_name(tag), tag.id(), (180 * tag.rotation()) / math.pi)
            # print("Tag Family %s, Tag ID %d, rotation %f (degrees)" % print_args)

    if flag_tag:
        continue


    '''
    if obstruction == True:
        if blobs_green:
            if green_cx < img.width() // 3:  # 色块在左侧
                uart.write("%d" % 2)  # 发送数字3
                #print("RD: 2")

            elif green_cx>=img.width() //3 and green_cx<2 * img.width()//3:  # 色块在中间
                uart.write("%d" % 1)  # 发送数字1
                #print("RD: 2")
            elif green_cx>=2*img.width() //3 :  # 色块在右中侧
                uart.write("%d" % 3)  # 发送数字2
                #print("RD: 3")
        else:    #视野中没有绿门

            uart.write("%d" % 0)   #小车原地自转，方向由KEIL决定
    '''
    #else:
    if blobs_red:
        # 获取红色块的中心坐标
        red_cx = blob_red.cx()
        red_cy = blob_red.cy()

        # 画出色块的矩形框
        img.draw_rectangle(blob_red.rect())
        # 在色块的中心点画一个十字
        img.draw_cross(blob_red.cx(), blob_red.cy() )

        # 在色块的中心点附近画一个关键点，标记出色块的旋转角度
        img.draw_keypoints([(blob_red.cx(), blob_red.cy(), int(math.degrees(blob_red.rotation())))], size=20)

        print("red_area:",blob_red.area())
        #判断是否距离红球过远
        if blob_red.area()<1500:
            uart.write("%c" % 'a')    #远离红球高速行驶
            time.sleep(0.01)
            if red_cx < img.width() // 5:  # 色块在左侧
                uart.write("%c" % 'b')  # 发送数字3
                #print("RD: 2")
            elif red_cx>=img.width() //5 and red_cx<2 * img.width()//5:  # 色块在左中侧
                uart.write("%d" % 2)  # 发送数字2
                #print("RD: b")
            elif 2*red_cx>=img.width() //5 and red_cx<3 * img.width()//5:  # 色块在中间
                uart.write("%d" % 1)  # 发送数字1
                #print("RD: 2")
            elif 3*red_cx>=img.width() //5 and red_cx<4 * img.width()//5:  # 色块在右中侧
                uart.write("%d" % 3)  # 发送数字3
                #print("RD: 3")
            elif 4*red_cx>=img.width() //5 and red_cx< img.width():    # 色块在右侧
                uart.write("%c" % 'c')
                #print("RD: c")

        else:   # 靠近球后开始准备自转的逻辑
            uart.write("%d" % 4)
            time.sleep(0.01)
            print("4")
            if red_cx > img.width() // 3 and red_cx < 2 * img.width() // 3:
                if green_last_position == 1:
                    uart.write("%d" % 1)
                    print("RD: 1")
                elif green_last_position == 2:
                    uart.write("%d" % 6)
                    print("RD: 6")
                elif green_last_position == 3:
                    uart.write("%d" % 7)
                    print("RD: 7")


            elif red_cx < img.width() // 3:  # 色块在左侧,右轮向左抖一下
                uart.write("%d" % 9)  # 发送数字9

            elif red_cx > 2 * img.width() // 3:  # 色块在右侧，左轮向右抖一下
                uart.write("%d" % 8)  # 发送数字8

    elif not blobs_red:    #视野中没有红球

        uart.write("%d" % 0)   #小车原地自转，方向由KEIL决定






