#!/usr/bin/env python3

import rospy
from std_msgs.msg import Int64

import sounddevice as sd
import numpy as np

def playsound(frequency,duration):
    # Generate a time array
    sample_rate = 44100  # samples per second
    t = np.linspace(0, duration, int(sample_rate * duration), endpoint=True)

    # Generate a sine wave
    y = np.sin(2 * np.pi * frequency * t)

    # Play the sound
    sd.play(y, sample_rate)

    # Wait for the sound to finish playing
    sd.wait()

class feedback():
    def __init__(self):
        self.sub = rospy.Subscriber("/stroke_rate", Int64, self.StrokeRateCallBack)
        self.cur_time = rospy.get_rostime().to_sec()
        rospy.loginfo("Feedback node is up!")

    def StrokeRateCallBack(self, stoke_rate):
        target = 70
        if (rospy.get_rostime().to_sec() - self.cur_time) < 5:
            return
        else:
            self.cur_time = rospy.get_rostime().to_sec()
            if stoke_rate.data > target + 5:
                rospy.loginfo("Too fast")
                playsound(800, 1)
            elif stoke_rate.data < target - 5:
                rospy.loginfo("Too slow")
                playsound(400, 1)
            else:
                rospy.loginfo("In sync")
                playsound(600, 1)


if __name__ == "__main__":
    rospy.init_node("feedback")
    node = feedback()
    rospy.spin()