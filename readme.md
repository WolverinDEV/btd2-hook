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

## Bonus: Python API
The file `playground.py` currently includes a simple ninja kiwi API wrapper.  
Since it is a playground it does not guarantee any API stability nor should be used in any third party app.  
The main goal is to explore the NK API as well serve as a sample implementation.  
  
In order to use the `playground.py` you have to create a `playground_session.py` containing your session credentials.  
To obtain them, simple inject the hook and click on "Copy Auth". Now paste them into`playground_session.py`.  
Example contents:  
```python
USER_SESSION = {
    "session": "e64e5c9ab27337be4f5e7e85b627404c",
    "device": "NO_LINK{6e8e68db35f017b979b5579a665057e3}",
    "skuID": 1704,
    "appID": 17
}
```