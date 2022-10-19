# BTD2 Hook
This repository contains documentation as well as a working game hook for the game Balloons Tower Defense 2 Battles.  


## The Hook
The hook itself (contained in `src`) aims to explore game internals especially network communication.  
Currently, knowledge of the following parts have been gained:  
- In game low level network protocol (Sending & Receiving packets)
- Some parts of in game communication such as game events (Simulation sync etc. are still to explore)
- HTTP requests & responses executed by the game to manipulate its own profile data or create matches

## The Documentation
The current version of BTD2 documentation can be found in `doc`.  
Most of the documentation doesn't seem to be game specific and might work for other Ninja Kiwi games as well.  

Currently, only the HTTP API `api.ninjakiwi.com` has been documented.  
More will hopefully follow soon such as game internals, network stack, etc.