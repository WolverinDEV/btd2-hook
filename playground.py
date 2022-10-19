import hashlib
import json
import time
import requests
import uuid

import playground_session as session

# request_url = "https://api.ninjakiwi.com/matchmaking/create"
# request = {
#     "maxPlayers": 2,
#     "bucket": "private",
#     "metadata": {"MM_VERSION": "30.DEFAULT"},
#     "expiresIn": 600000,
#     "unique": False,
#     "onCellConnection": False
# }

# request_url = "https://api.ninjakiwi.com/user/online/singleSession"
# request = {"duration": 90000, "gamestate": "-", "makeCurrent": False, "uniqueSessionID": ""}

request_url = "https://api.ninjakiwi.com/storage/transaction/update"
request = {
    "uuid": "8c9a9881c72c4e50a2ecbc64c2d6a20a_4",
    "transactions": [
        {
            "type": "INCREMENT",
            "path": "$.currencies.monkey_money",
            "value": 30_000,
            "metadata": "game win reward"
        },
        {
            "type": "INCREMENT",
            "path": "$.ladder.trophies",
            "value": 1,
            "metadata": "Player Open Battle Chest"
        },
        {
            "type": "INCREMENT",
            "path": "$.ladder.trophies",
            "value": 2,
            "metadata": "Player Open Battle Chest"
        },
        {
            "type": "INCREMENT",
            "path": "$.ladder.trophies",
            "value": 3,
            "metadata": "Player Open Battle Chest"
        },
        {
            "type": "INCREMENT",
            "path": "$.ladder.trophies",
            "value": 5,
            "metadata": "Player Open Battle Chest"
        },
        {
            "type": "INCREMENT",
            "path": "$.ladder.trophies",
            "value": 10,
            "metadata": "Player Open Battle Chest"
        },
        {
            "type": "INCREMENT",
            "path": "$.ladder.trophies",
            "value": 20,
            "metadata": "Player Open Battle Chest"
        },
        {
            "type": "INCREMENT",
            "path": "$.ladder.trophies",
            "value": 50,
            "metadata": "Player Open Battle Chest"
        },
        {
            "type": "INCREMENT",
            "path": "$.ladder.trophies",
            "value": 100,
            "metadata": "Player Open Battle Chest"
        },
        {
            "type": "INCREMENT",
            "path": "$.ladder.trophies",
            "value": 200,
            "metadata": "Player Open Battle Chest"
        },
        {
            "type": "INCREMENT",
            "path": "$.ladder.trophies",
            "value": 1,
            "metadata": "Player Open Battle Chest"
        },
        {
            "type": "INCREMENT",
            "path": "$.ladder.trophies",
            "value": 1,
            "metadata": "Player Open Battle Chest"
        },
        {
            "type": "INCREMENT",
            "path": "$.ladder.trophies",
            "value": 1,
            "metadata": "Player Open Battle Chest"
        },
    ],
    "filename": "Profile.save",
    "reason": "Player Open Battle Chest",
    "transactionKey": "tk.47p0cqor0sj"
}


def execute_request(endpoint: str, payload):
    payload = json.dumps(payload)
    nonce = "{}:{}:{}:".format(38, 1662219209, int(round(time.time() * 1000)))
    sign = hashlib.md5("{}{}{}{}".format(session.USER_SESSION["session"], session.APP_HTTP_SIGN_ARG2, payload, nonce).encode()).hexdigest()
    response = requests.post("https://api.ninjakiwi.com/{}".format(endpoint), data=json.dumps({
        "nonce": nonce,
        "sig": sign,
        "data": payload,
        "auth": session.USER_SESSION
    }), headers={
        "content-type": "application/json",
    })

    if response.status_code != 200:
        print("Invalid HTTP status: {}".format(response.status_code))

    response = json.loads(response.content)
    if response["error"] is not None:
        print(json.dumps(response))
        raise Exception(response["error"]["type"])

    # We ignore the signature for now....
    return json.loads(response["data"])


if __name__ == "__main__":
    transaction_uuid = uuid.uuid4()
    transaction_id = 0
    transaction_result = execute_request("storage/transaction/new", {"filename": "Profile.save"})
    print("Initial result:")
    print(json.dumps(transaction_result))

    # Transaction metadata is required
    transaction_result = execute_request("storage/transaction/update", {
        "uuid": "{}_{}".format(transaction_uuid, transaction_id),
        "transactionKey": transaction_result["transactionKey"],
        "reason": "Player Open Battle Chest",  # Reasons are filtered and must be correct.
        "filename": "Profile.save",
        "transactions": [
            # {
            #     "type": "INCREMENT",
            #     "path": "$.currencies.monkey_money",
            #     "value": 2600,
            #     "metadata": "because it works..."
            # },
            # {
            #     "type": "INCREMENT",
            #     "path": "$.ladder.trophies",
            #     "value": 100,
            #     "metadata": "Player Open Battle Chest"
            # },
            # {
            #     "type": "SET",
            #     "path": "$.towers[id=86].custom_monkey_name",
            #     "value": "I'm sorry but why u again?",
            #     "metadata": "because it works..." # Is required
            # }
            {
                "type": "INCREMENT",
                "path": "$.currencies.universal_xp",
                "value": 500_000,
                "metadata": "Quick start!"
            },
            {
                "type": "SET",
                "path": "$.tutorial_progress",
                "value": 15,
                "metadata": "Skip this shit!"
            },
            {
                "type": "INCREMENT",
                "path": "$.currencies.monkey_money",
                "value": 50_000,
                "metadata": "I want money fast!"
            },
        ]
    })
    transaction_id += 1
    print("Second result:")
    print(json.dumps(transaction_result))

# https://api.ninjakiwi.com/matchmaking/create
# {
#     "nonce":"20:1662219209:1662135256823:",
#     "sig":"01785220f397da9168ac32e1275cf0c4",
#
#     "data":"{\"maxPlayers\":2,\"bucket\":\"private\",\"metadata\":{\"MM_VERSION\":\"30.DEFAULT\"},\"expiresIn\":600000,\"unique\":false,\"onCellConnection\":false}",
#
#     "auth":{...}
# }

# {"maxPlayers":2,"bucket":"private","metadata":{"MM_VERSION":"30.DEFAULT"},"expiresIn":600000,"unique":false,"onCellConnection":false}
