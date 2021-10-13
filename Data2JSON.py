import time
import json

packet = (1,2,3,4,5)

named_tuple = time.localtime() # get struct_time
time_string = time.strftime("%m_%d_%Y#%H-%M-%S", named_tuple)

with open('{}'.format(time_string), 'w') as outfile:
    json.dump(packet, outfile)
