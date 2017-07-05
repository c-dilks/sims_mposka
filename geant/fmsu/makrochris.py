import math
import subprocess

theta=0;
while theta<=30: 
    Pi=math.pi
    thetaD=theta*Pi/180
    xdir=0
    ydir=math.sin(thetaD)
    zdir=math.cos(thetaD)
    print 'theta = %d' %theta
    print 'xdir = %d' %xdir
    print 'ydir = %d' %ydir
    print 'zdir = %d' %zdir
    subprocess.call(['bash','./chrismacro.sh',str(xdir),str(ydir),str(zdir)]);
    theta+=2;
