# tutorial_event-driven-flow
The tutorial on the event-driven software using an optical flow example. An introduction can be followed below explaining the basic components required to create an event-driven module.

Requirements:

https://github.com/robotology-playground/event-driven.git

YARP

icub //only required for some modules

icubcontrib //for easy install

#include "iCub/eventdriven/all.h"
using namespace ev;

event-driven library overview
Datastructures
- events
- vBottle
- vQueue
- vtsHelper

Modules
- vFramer

some other useful (but not covered today) things:
event storage structures (e.g. surface), camera un-distortion, salt and pepper filter, robot interfaces, circle detection, cluster tracking ...

Events in event-driven libraries:

IMAGE OF INHERITANCE

EXAMPLE OF VEVENT - snapshot of code - store time

ADDRESSEVENT  - adds x and y
FLOWEVENT     - adds dx/dt and dy/dt

(shared memory) event pointers are declared as:

event<AddressEvent>

or (as events are all inhereted from vEvent) we can point to any type with:

event<> //is equal to event<vEvent>

to move down the inheretence we can use:

event<> v_any = ...
event<FlowEvent> v = getas<FlowEvent>(v_any);

if v_any is not a flowEvent v = 0;
if v_any is a flowEvent we can access x, y, dx/dt and dy/dt;

A typical workflow will be:

             ADDRESSEVENT       FLOWEVENT
CAMERA -> GRABBER -> PROCESS (VFLOW) -> ROBOT BEHAVIOUR
                                     -> VFRAMER

How do we send events between processes?
Using YARP obviously!

Event Packets (vBottle):

The vBottle is a class that inherets from a standard YARP bottle so we can:
1. can be sent over YARP ports/bufferedports without modification
1. instant compatibility with yarpdataplayer and yarpdatadumper
1. we use the flexibility and search-ability of bottles so we can send different event types easily

However you can only put and get vEvents with a vBottle. It wraps some encoding and decoding functions.

To use a vBottle:

vBottle vbot;
event<> v;
//adding events is simple
vbot.add(v);

//you can get all events, get a specific event type, ensure sorted (by timestamp)
vQueue = vbot.get(v);
vQueue = vbot.get<AddressEvent>(v);
vQueue = vbot.getSorted<AddressEvent>(v);

The vQueue is a wrapper for a std::deque< event<> > so we can:

vQueue q;
event<> v;

q.push_front(v);
q.push_back(v);
q.pop_front(v);
q.pop_back(v);

and we can iterate through the q to access events:

vQueue::iterator qi;
for(qi = q.front(); qi != q.end(); qi++) {

    //access events
    event<AddressEvent> = getas<AddressEvent>(*qi);
    //do some processing

}

Help with events:


//ADD CODE SNIPPET FOR VTSHELPER

Timestamps are stored with (at the moment) 24 bits (unsigned integer) and the clock performing timestamps have a period of 80 nanaoseconds.
If we do the maths we will overflow every: 2^24 * 80e-9 = 1.34217728 seconds. Therefore our code needs to handle overflows ("timestamp wraps").


Instantiating a vtsHelper can be used to convert timestamps to a  long unsigned int (many years before an overflow):
vtsHelper unwrap;
event<> v;
unsigned long int unwrappedstamp = unwrap(v->getStamp());
*NOTE: we don't send the unwrapped timestamps in bottles so every module needs to track its own unwrapped timestamp

Alternatively, as we often only need relative time between events we can account for wraps using:

event<> v; //allocated somewhere

int currenttimestamp = v->getStamp();
int previoustimestamp;
if(previoustimstamp > currenttimestamp) //assuming events are in chronological order this indicates a wrap
    previoustimestamp -= vtsHelper::getMaxStamp();
int dt = currenttimestamp - previoustimestamp;

Modules:

vFramer - unlike standard cameras, there is no "images" or "frames" when using events so we need to create an image frame from recent events that have occurred if we want to visualise the output.






























