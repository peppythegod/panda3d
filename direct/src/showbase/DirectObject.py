
from MessengerGlobal import *
from DirectNotifyGlobal import *

class DirectObject:
    """
    This is the class that all Direct/SAL classes should inherit from
    """
    # Event Handling

    # object.accept('mouse', object.handleMouse)
    # object.accept('mouse', 'handleMouse')
    # object.accept('mouse', 'handleMouse', [1,2])
    
    def accept(self, event, method, extraArgs=[]):
        messenger.accept(event, self, method, extraArgs, 1)
    def acceptOnce(self, event, method, extraArgs=[]):
        messenger.accept(event, self, method, extraArgs, 0)
    def ignore(self, event):
        messenger.ignore(event, self)
    def isAccepting(self, event):
        return messenger.isAccepting(event, self)
    def isIgnoring(self, event):
        return messenger.isIgnoring(event, self)
    

