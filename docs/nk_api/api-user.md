# Online User API
This document covers all endpoints under `https://api.ninjakiwi.com/user/online/`.
The following URLs are known:
- [`https://api.ninjakiwi.com/user/online/singleSession`](online-singleSession)
- [`https://api.ninjakiwi.com/user/current`](current)

## Online singleSession
Endpoint URL: `https://api.ninjakiwi.com/user/online/singleSession`

### Request parameters
TODO!

### Response parameters
TODO!

### Example
Request:  
```json
{
    "duration": 90000,
    "gamestate": "-",
    "makeCurrent": false,
    "uniqueSessionID": ""
}
```

Response:
```json
{
    "online": {
        "sessionID": "e64e5c9ab27337be4f5e7e85b627404c",
        "uniqueSessionID": null,
        "expiresIn": 90000,
        "hasSession": true,
        "gamestate": "-",
        "currentSessionIsActive": true,
        "isNewLock": false
    }
}
```

## Current user
Endpoint URL: `https://api.ninjakiwi.com/user/current`

Request:
```json
{}
```

Response:
TODO???