from __future__ import annotations

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


class NKApi:
    auth: dict
    sign_arg2: str

    def __init__(self, auth):
        self.auth = auth
        self.sign_arg2 = "B42D1230A78185FF"

    def execute(self, endpoint: str, payload: dict):
        session_token = self.auth.get("session")
        if session_token is None:
            raise Exception("Missing 'session' in auth")

        payload = json.dumps(payload)
        nonce = "{}:{}:{}:".format(38, 1662219209, int(round(time.time() * 1000)))
        sign = hashlib.md5("{}{}{}{}".format(session_token, self.sign_arg2, payload, nonce).encode()).hexdigest()
        response = requests.post("https://api.ninjakiwi.com/{}".format(endpoint), data=json.dumps({
            "nonce": nonce,
            "sig": sign,
            "data": payload,
            "auth": self.auth
        }), headers={
            "content-type": "application/json",
        })

        response.raise_for_status()
        response = json.loads(response.content)
        if response.get("error") is not None:
            # TODO: Move this into logging
            print(json.dumps(response))
            raise Exception(response["error"]["type"])

        if "data" not in response:
            print(json.dumps(response))
            raise Exception("Missing data in response")

        # We ignore the signature for now....
        return json.loads(response["data"])


class NKTransaction:
    api: NKApi
    filename: str

    _uuid: str
    _transaction_key: str | None
    _index: int

    def __init__(self, api: NKApi, filename: str):
        self.api = api
        self.filename = filename

        self._uuid = uuid.uuid4().hex
        self._index = 1
        self._transaction_key = None
        pass

    def _create_transaction(self):
        """
        Create a new transaction.
        """
        response = self.api.execute("storage/transaction/new", {
            "filename": self.filename
        })
        self._transaction_key = response.get("transactionKey")
        if self._transaction_key is None:
            raise Exception("missing transactionKey in response")

    def _current_transaction_key(self):
        if self._transaction_key is None:
            self._create_transaction()

        return self._transaction_key

    def acquire(self):
        self._current_transaction_key()

    def update(self, reason: str, transactions: [dict]):
        transaction_key = self._current_transaction_key()
        result = self.api.execute("storage/transaction/update", {
            "uuid": "{}_{}".format(self._uuid, self._index),
            "transactionKey": transaction_key,
            "filename": self.filename,

            "reason": reason,
            "transactions": transactions
        })

        self._transaction_key = result.get("transactionKey")
        self._index += 1
        return result["transaction"]


if __name__ == "__main__":
    nkapi = NKApi(session.USER_SESSION)

    # transaction_profile = NKTransaction(api=nkapi, filename="Profile.save")
    # transaction_profile.acquire()
    # Chaning "owner" to something invalid -> {'this': "is why we can't have nice things"}
    # The owner is the "nkapiID"
    response = nkapi.execute("storage/load", {"owner": "57534730391a7dbc0913f0ab", "filename": "Profile.save", "includeBody": True })
    print(response)
    # transaction_profile = NKTransaction(api=nkapi, filename="Profile.save")
    # print(transaction_profile.update("Player Open Battle Chest", [
    #     {
    #         "type": "INCREMENT",
    #         "path": "$.currencies.universal_xp",
    #         "value": 10,
    #         "metadata": "Some testing"
    #     },
    # ]))
    # print(transaction_profile.update("Player Open Battle Chest", [
    #     {
    #         "type": "DECREMENT",
    #         "path": "$.currencies.universal_xp",
    #         "value": 10,
    #         "metadata": "Some testing"
    #     },
    # ]))

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
