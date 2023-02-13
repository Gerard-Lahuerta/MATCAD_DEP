# -*- coding: utf-8 -*-
"""
Created on Mon May 31 15:01:02 2021

@author: onasa
"""

from abc import ABC, abstractmethod
import time

class Door():
    

    
  def __init__(self, id, code_handler):
    self._id = id
    self.code_handler = code_handler

    
  def process_code(self, code):
    self.code_handler.handleCode(code,self)
    
  def opened(self):
    print('open door {}'.format(self._id))
    
  def fireAlarm(self):
  
    print("S'ha avisat a l'entitat de seguretat {}".format(self._id))

class CodeHandler(ABC):
    
    _num_trials = 0
    _door_locked = False
    
    
    def __init__(self, next_handler):
        self.next_handler = next_handler
        
    
    @abstractmethod
    def handleCode(self,code,door):
     pass
    
    @staticmethod
    def _reset_door_state(door):
        _num_trials = 1 #perquè vagi a 1, 2, 3
        _door_locked = False


class Open(CodeHandler):

    def __init__(self, code,next_handler):
        
        super().__init__(next_handler)
        self.code = code
       
       

    def handleCode(self, code, door):
        print("Handle Open")
        if (CodeHandler._door_locked !=True):
         
            if (code == self.code):
                print("S'ha inserit el codi correcte\n")                
                door.opened()
                CodeHandler._reset_door_state(door)
                
                
            else:
           
                print('{} intents'.format(CodeHandler._num_trials))
                if(self.next_handler != None):
                    self.next_handler.handleCode(code,door)
                
                else:
                    CodeHandler._num_trials+=1
                
                
        else:
            pass # can't open until unlocked


class Log(CodeHandler):
    
    def __init__(self, next_handler):
        super().__init__(next_handler)
       
        
    def handleCode(self,code,door):
        print("Handle Log")
        print('Code {} , at {} , state of locked {} '.format(code, time.strftime("%c"), CodeHandler._door_locked))
        
        if(self.next_handler != None):
                    self.next_handler.handleCode(code,door)


class Lock(CodeHandler):
    
    def __init__(self, next_handler, code):
        super().__init__(next_handler)
        

    def handleCode(self, code, door):
        print("Handle Lock")
        if CodeHandler._door_locked==False :
            if ((CodeHandler._num_trials==3)):
                CodeHandler._door_locked = True #BLOQUEADO
                print("S'ha sobrepassat el màxim d'intents, la porta està bloquejada\n")
            else:
                CodeHandler._num_trials += 1
                
           
        else:#Esto i lo del elif no serian lo mismo?
            pass # can't open until unlocked


class Unlock(CodeHandler):
    
    def __init__(self, code, next_handler):
        super().__init__(next_handler)
        self.code = code
       

    def handleCode(self, code, door):
        print("Handle Unlock")
        if (CodeHandler._door_locked==True):
            if (code == self.code):
                CodeHandler._door_locked = False #DESBLOQUEADO
                print("S'ha desbloquejat la porta")
                self._reset_door_state(door) 
                
            else:
                pass
        else:
            if(self.next_handler != None):
                    self.next_handler.handleCode(code,door)

class FireAlarm(CodeHandler):
    
    def __init__(self, code, next_handler):
        super().__init__(next_handler)
        self.code = code
 

    def handleCode(self, code, door):
        print("Handle FireAlarm")
        if CodeHandler._door_locked==False:
            if (code == self.code):
                
                door.fireAlarm()
                self._reset_door_state(door)
                
            else:
                if(self.next_handler != None):
                    self.next_handler.handleCode(code,door)
                else:
                    print('{} intents'.format(CodeHandler._num_trials))
                    CodeHandler._num_trials+=1
                
        else:
            pass


if __name__ == '__main__':
    code_open = '1111'
    code_fire_alarm = '2222'
    code_unlock = '3333'
    chain1 = Log(Unlock(code_unlock, FireAlarm(code_fire_alarm, Open(code_open, Lock(None,None)))))
    chain2 = Log(Open(code_open, None))
    chain3 = Log(FireAlarm(code_fire_alarm, Open(code_open, None)))
    d1 = Door('d1', chain3)
    d1.process_code('1111') # opens
    d1.process_code('2222') # opens and fires alarm
    d1.process_code('1234') # first trial
    d1.process_code('4321') # second trial
    d1.process_code('5555') # third trial, gets locked
    d1.process_code('6666') # invalid unlock code
    d1.process_code('7777') # invalid unlock code
    d1.process_code('1111') # invalid unlock code
    d1.process_code('3333') # valid unlock code, now can be opened or fire alarm
    d1.process_code('2222') # opens and fires alarm