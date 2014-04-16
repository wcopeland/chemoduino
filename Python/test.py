__author__ = 'Wilbert'

from datetime import datetime

class Measurement(object):
    def __init__(self, effluent, reactor):
        self.Time = datetime.now()
        return


class Reactor(object):
    def __init__(self, id):
        return



number_reactors = int(raw_input('How many reactors? '))
initial_pulse_frequency = float(raw_input('Initial pulse frequency? '))


start = datetime.now()

a = [x for x in range(1000000)]

span = (datetime.now() - start).microseconds / 1000
print(span)








