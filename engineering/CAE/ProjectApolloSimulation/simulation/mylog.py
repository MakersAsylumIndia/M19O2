#Copyright (c) 2018 Daniel B. Grunberg
#
#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program.  If not, see <http://www.gnu.org/licenses/>.

import datetime
import time
import sys
import os
import atexit

'''
usage
import mylog

dbg=mylog(outdir='./log')
...
dbg.print('This is some logging output')
'''

##############################################################################
def dbg_set_stdout():
  #Call before first dbg call to make output go to stdout instead of file
   global dbg_stdout, log_file_code
   dbg_stdout=True
   log_file_code='stdout'
################################################################################3
def jstringday(dt=None):
  #get a 2 letter string corresponding to this julian day
  if dt is None:
    dt=datetime.now()
  x = int(dt.strftime('%j'))
  x1=x%26 #(0-25)
  x2=x/26 #(0-14)
  return chr(97+x2)+chr(97+x1)
################################################################################3
def log_code(t=None, incr=0):
  #return 5 character string for a log filename for t=sec since Epoch (time.time())
  #(or now if t==None)
  #increments counter by incr minutes for use as a log file unique name finder
  #the code is 2 lowercase letters followed by 3 digit julian day, e.g.
  # aq027.  The two letter code increments once per minute
  if t is None:
    t=time.time()
  elif type(t) is not float:
    raise ValueError('need float arg or None')
  dt=datetime.datetime.fromtimestamp(t)
  jday=int(dt.strftime('%j'))
  minute=int(t/60) + int(incr)
  x2=int(minute % 26)           #(0-25)
  x1=int((minute//26)%26)        #(0-25)
  #print 'x1=',x1
  #print 'x2=',x2
  #print chr(97+x1)+chr(97+x2)+'{:02d}'.format(d)
  #print '{:c}{:c}{:02d}'.format(x1+97, x2+97, d)
  #chr(97) is 'a', x1,x2 are 0-25, so we will get a lowercase letter
  return chr(97+x1)+chr(97+x2)+'{:03d}'.format(jday)
################################################################################3
class Log():
  ERROR=1
  WARNING=2
  DEBUG=2
  def set_log_level(x):
    self.log_level=x
    
  def __init__(self, outdir=None, descr='',method=1,times=True):
    #initialize logger
    #outdir is None for stdout, otherwise a directory to place a uniquely generated
    #filename, e.g. aq023-<descr>.log
    #methods:  (1) generate a 2 letter code + julian day, 2 letter code increments to be unique
    #          (2) sequential log-<descr>-<iter>.log   
    self.times=times
    self.log_level=Log.ERROR
    self.outdir=outdir
    self.descr=descr
    self._log_code=None
    self._filename=None
    self.start_time=datetime.datetime.now()
    self.stdout=False
    log_file_code='AA'  # change below if needed
    if outdir is None:
      self._file=sys.stdout
      sys.stderr.write('logging to stdout\n')
      self.stdout=True
    else:
      iter=0
      try:
        os.mkdir(outdir)
      except OSError:
        pass
      descr2='' if descr=='' else '-'+descr
      while iter < 1000:
        if method==1:  
            log_file_code=log_code(incr=iter)
            file=os.path.join(outdir,'{}{}.log'.format(log_file_code, descr2))
        elif method==2:
            file=os.path.join(outdir,'log{}-{:03d}.log'.format(descr2,iter))
        if not os.path.isfile(file):
          break
        iter+=1
      else:
        #took to many tries, give up
        raise ValueError('Unable to get log file name after many tries')
      self._file=open(file,'w')
      sys.stderr.write('writing to log file:{}\n'.format(file))
      self._filename=file
      atexit.register(self.dbg_exit)
    self._log_code=log_file_code
    self._file.write("Starting at {}\n".format(datetime.datetime.now()))
    self._file.write("Args: {}\n".format(' '.join(sys.argv)))
        
  @property
  def log_filename(self):
    return self.dbg_filename
    
  @property
  def log_code(self):
    return self._log_code

  def close(self):
    sys.stderr.write('closing log file {}\n'.format(self._filename))
    self.print('done')
    self._file.close()
    self._file=None
    
  def dbg_exit(self):
    if self._file is not None:
      sys.stderr.write('closing log file {}\n'.format(self._filename))
      self.print('done')
      self._file.close()

        
  def print(self,*string,log=ERROR):
    #puts out debug message in the opened
    #all arguments are converted to string and printed.  E.g. dbg('hello', array)
    # __file__ is the current file (mylib.py)
    # sys.argv[0] is better - running program name, like ..path../learner.py
    if self.log_level < log:
      return
    t=''
    if self.times:
      delta=datetime.datetime.now()-self.start_time
      hours=delta.seconds // 3600
      mins=(delta.seconds - 3600*hours) // 60
      secs=(delta.seconds - 3600*hours - 60*mins)
      hundredths=(delta.microseconds)// 10000
      t="{:02d}:{:02d}:{:02d}.{:02d}".format(hours, mins, secs, hundredths)
    s=' '.join([str(x) for x in string])
    self._file.write("{} {}\n".format(t, s))
    self._file.flush()

  def log(self,*string):
    self.print(*string)
