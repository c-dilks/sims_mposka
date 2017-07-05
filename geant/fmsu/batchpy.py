import math
import subprocess

f=open("batchpy.bat","w")
f.write("Executable = ./chrismacro.sh\n")
f.write("Universe = vanilla\n")
f.write("notification = never\n")
f.write("getenv = True\n\n")

theta=0
while theta<=30:
    Pi=math.pi
    thetaD=theta*Pi/180
    xdir=0
    ydir=math.sin(thetaD)
    zdir=math.cos(thetaD)
    print("xdir = ",xdir," ydir = ",ydir,"zdir = ",zdir)

    f.write("Arguments = {} {} {}\n".format(str(xdir),str(ydir),str(zdir)))
    f.write("Log = log/%d.log\n" % theta)
    f.write("Output = log/%d.out\n" % theta)
    f.write("Error = log/%d.err\n" % theta)
    f.write("Queue\n\n")

    theta+=2

f.close()
subprocess.call('condor_submit batchpy.bat')
