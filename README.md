# Tutorial on Handling Event Driven Data Stream -- Optical Flow
This tutorial introduces the basic components for handling event-driven data stream. We will use an example application, the **event-driven optical flow**, to learn the dedicated data structures and functions in the event-driven library and the basic components required to create an event-driven module.

# Requirements:
Your current setup comprises YARP, iCub (some of the event-driven modules require iCub) and iCubContrib (for easy install of the executables). In addition you need the **robotology-playground/event-driven**(https://github.com/robotology-playground/event-driven.git) library.

### Note: 
The event-driven code is already installed in your virtual machine (alternatively, you should have followed the installing instructions in the vvv [wiki](https://github.com/vvv-school/vvv-school.github.io/blob/master/instructions/how-to-prepare-your-system.md#install-event-driven)), however, as there have been few updates, you need to `git pull`. 

# The event-driven library

The event-driven library allows you to use datastructures and helper modules to handle events. 
You need to include it in your header file `vOptFlow.h`. 

```javascript
#include "iCub/eventdriven/all.h"
using namespace ev;
``` 

In this tutorial you will learn:

- Datastructures

  - events
  - vBottle
  - vQueue
  - vtsHelper

- Modules
  - vFramer

Some other useful (but not covered today) modules are:
- event storage structures (e.g. surface), 
- camera un-distortion, 
- salt and pepper filter, 
- robot interfaces, 
- circle detection, 
- cluster tracking 
- ...

### Events in event-driven libraries:
Different types of events can be defined, inheriting from the basic type **vEvent** that holds the _timestamp_ of the event.

![IMAGE OF INHERITANCE](./misc/vEventInheritance.png)

```javascript
EXAMPLE OF VEVENT - snapshot of code - store time
//FILL HERE THE CODE

``` 

A new event type can add information to the current event, or a completely new event can be generated by a module:

- ADDRESSEVENT (AE)  - adds x and y
- FLOWEVENT     - adds dx/dt and dy/dt
- CLUSTER EVENT - if a cluster is detected creates a new event with x,y of the cluster centre of mass, vx, vy, m (number of events forming the cluster)

(shared memory) event pointers are declared as `event<AddressEvent>` or (as events are all inhereted from vEvent) we can point to any type with `event<> //is equal to event<vEvent>`.

to move down the inheretence we can use:

```javascript
event<> v_any = ...
event<FlowEvent> v = getas<FlowEvent>(v_any);
```

if v_any is not a flowEvent v = 0;
if v_any is a flowEvent we can access x, y, dx/dt and dy/dt;

A typical workflow will be:

ADDRESSEVENT       FLOWEVENT
CAMERA -> GRABBER -> PROCESS (VFLOW) -> ROBOT BEHAVIOUR
                                     -> VFRAMER

### YARP vBottle
How do we send events between processes?

**Using YARP obviously!**

Event Packets (vBottle):

The vBottle is a class that inherits from a standard YARP bottle so we can:

1. can be sent over YARP ports/bufferedports without modification
1. instant compatibility with yarpdataplayer and yarpdatadumper
1. we use the flexibility and search-ability of bottles so we can send different event types easily

However you can only put and get vEvents with a vBottle. It wraps some encoding and decoding functions.

Code to use a vBottle:

```javascript
vBottle vbot;
event<> v;
//adding events is simple
vbot.add(v);

//you can get all events, get a specific event type, ensure sorted (by timestamp)
vQueue = vbot.get(v);
vQueue = vbot.get<AddressEvent>(v);
vQueue = vbot.getSorted<AddressEvent>(v);
```
The vQueue is a wrapper for a `std::deque< event<> >` so we can:

```javascript
vQueue q;
event<> v;

q.push_front(v);
q.push_back(v);
q.pop_front(v);
q.pop_back(v);
```

and we can iterate through the q to access events:

```javascript
vQueue::iterator qi;
for(qi = q.front(); qi != q.end(); qi++) {

    //access events
    event<AddressEvent> = getas<AddressEvent>(*qi);
    //do some processing

}
```

### vtsHelper

Timestamps are stored with (at the moment) 24 bits (unsigned integer) and the clock performing timestamps has a period of 80 nanoseconds.
If we do the maths we will overflow every: 2^24 * 80e-9 = 1.34217728 seconds. Therefore our code needs to handle overflows ("timestamp wraps").

```javascript
//ADD CODE SNIPPET FOR VTSHELPER
```

Instantiating a `vtsHelper` can be used to convert timestamps to a  long unsigned int (many years before an overflow):

```javascript
vtsHelper unwrap;
event<> v;
unsigned long int unwrappedstamp = unwrap(v->getStamp());
```

##### NOTE: We don't send the unwrapped timestamps in bottles so every module needs to track its own unwrapped timestamp

Alternatively, as we often only need relative time between events, we can account for wraps using:

```javascript
event<> v; //allocated somewhere

int currenttimestamp = v->getStamp();
int previoustimestamp;
if(previoustimstamp > currenttimestamp) //assuming events are in chronological order this indicates a wrap
    previoustimestamp -= vtsHelper::getMaxStamp();
int dt = currenttimestamp - previoustimestamp;
```

### Modules: vFramer

Unlike standard cameras, there is no "images" or "frames" when using events so we need to create an image frame from recent events that have occurred if we want to visualise the output on a synchronous display. This means that we are going to grab all of the events within a given time window (e.g. 30ms) and create a frame.































