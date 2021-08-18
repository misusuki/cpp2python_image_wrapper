import cv2
import numpy as np

def Received_Image_1(img=None):
    print("Received_Image_1")
    if img is not None:

      
        cv2.imshow("Received_Image_1", img)
        cv2.waitKey(1)
        return bytearray(img)
        
def Received_Image_2(img=None):
   
    if img is not None:
        print("Received_Image_2")
        gray = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
        cv2.imshow("Received_Image_2", img)
        cv2.waitKey(1)
        return bytearray(gray)

def main():
    while True:
        print("Hello World!")

if __name__ == "__main__":
    main()
    #Received_Image_3
   
