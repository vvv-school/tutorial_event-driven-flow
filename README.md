# Tutorial on Handling Event Driven Data Stream -- Optical Flow
This tutorial introduces additional functionalities of the event-driven library. We will use an example application, the **event-driven optical flow**.

# Requirements:
By now you should know the basics of event-driven library (from [tutorial_event-driven-handling](https://github.com/vvv-school/tutorial_event-handling)).


### Further event inheritance
As in the previous tutorial, different types of events can be defined, inheriting from the basic type **vEvent** that holds the _timestamp_ of the event. We can then create other types of events in a heirarchy that further adds information:

![IMAGE OF INHERITANCE](./misc/vEventInherit.png)


A new event type can add information to the current event, or a completely new event can be generated by a module:

- ADDRESSEVENT (AE)  - you should know by now....
- FLOWEVENT     - adds dx/dt and dy/dt
- CLUSTER EVENT - if a cluster is detected creates a new event with x,y of the cluster centre of mass, vx, vy, m (number of events forming the cluster)

### vSurface

The optical flow tutorial uses a helper data structure that stores the event _surface_. That is, it remembers only the most recent event that occurs on each pixel location. The datastructure also indexes events by position rather than in chronological order, which greatly improves the speed when querying for spatial subsets of events. In this way the data structure is extremely useful for calculating the optical flow using a plane fitting technique.

We'll start the tutorial making a system as:

![optflow](./misc/tutorial2.png)


























