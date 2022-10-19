# User Storage API
This document covers all endpoints under `https://api.ninjakiwi.com/storage/`.
The following URLs are known:
- [`/transaction/new`](transaction-new)
- [`/transaction/update`](transaction-update)

## Transaction new
Create a new transaction for a give file.  
My understanding of "file" is a mongo db object.  
  
Request Method: `POST`  
Endpoint URL: `https://api.ninjakiwi.com/storagetransaction/new`

### Request parameters
The request parameters is a JSON encoded object containing the following fields:
- `filename` The target file name we want to update

### Response parameters
The response is a JSON encoded object containing the same fields as the transaction update response (see [here](transaction-update)).
Unless the transaction update response, `$.transaction.success` will be false and `$.transaction.transactionsApplied` zero.
The `transactionKey` contains the new transaction key, which could be used for the next transaction update.

### Example
Request:
TODO!

Response:
```json
{
  "transaction": {
    "success": false,
    "gotLock": false,
    "transactionsApplied": 0,
    "errors": [],
    "retryRequest": false,
    "info": "none"
  }, 
  "transactionKey": "tk.5n4ksf0fc51",
  "lockDuration": 172800000,
  "fileData": {}
}
```

## Transaction update
Update the "file" identified by the transaction key.  
  
Request Method: `POST`  
Endpoint URL: `https://api.ninjakiwi.com/storage/transaction/update`  


### Request parameters
The request parameters is a JSON encoded object containing the following fields:
- `uuid` A string containing the transaction UUID
- `transactionKey` A string containing the current transaction key
- `reason` A string giving a reason to why performing this transaction
- `filename` A string containing the transactions file name.  
Note: The filename *should*  be equal to the `filename` given when creating a new transaction.  
The behaviour when changing the filename hasn't been tested.
- `transactions` A JSON array containing the transactions to perform

### Generation of the transaction UUID
The transaction UUID contains two parts joined togather with an underscore (`_`).  
The first part is a random generated UUID which *must* be equal for all transactions.  
The second part is the transaction sequence number starting with 1 and increased by 1 every transaction (ignoring the outcome).  

### Generation of the transaction key
The transaction key must be initially obtained by starting a new transaction (see [here](transaction-new)).  
After each transaction the returned (new) transaction key must be used.

### Valid reasons
The `reason` parameters must be one of a fixed set of reasons.  
Available reasons are:
- `Player Open Battle Chest`
- `Apply rewards and clear match in progress`

Note: Not all available reasons are currently known.

### Available transactions
Each transaction is a JSON object containing the following fields:
- `type` A string representing the operation type. Available types are:
  - `SET`
  - `INCREMENT`
  - `DECREMENT`
- `path` The objects path the operation should operate on.  
I'm not sure what kind of syntax this follows, but it seems to be similar to mongodb.  
Example: `$.towers[id=86].custom_monkey_name`
- `value` The target value. This could be anything allowed by JSON.
- `metadata` A string which can be anything.

## Response parameters
The response is a JSON encoded object containing the following fields:
- `transaction` A JSOn object containing the following fields:
  - `success` A boolean indicating if the transaction has been successfully executed.
  - `gotLock` A boolean which might indicate if we still have the lock?
  - `transactionsApplied` A number representing how many transactions have been applied successfully
  - `errors` An array of strings containing the transaction error for each transaction
  - `retryRequest` A boolean which might indicate if we should try it again later?
  - `info` A string which contains some general transaction info.
- `transactionKey` A string containing the new transaction key for the next update request.
- `lockDuration` A number indicating how long the file will be locked (in milliseconds?) by the transaction.
- `fileData` A JSON object with no fields. The purpose is unknown.


### Example
Request:
```json
{
    "uuid": "e1c58b863d8d4802b67bd58c1edc4d76_1",
    "transactionKey": "tk.5n4ksf0fc51",
    "reason": "Player Open Battle Chest",
    "filename": "Profile.save",
    "transactions": [
          {
            "type": "SET",
            "path": "$.tutorial_progress",
            "value": 15,
            "metadata": "I don't need this any more."
          },
          {
              "type": "INCREMENT",
              "path": "$.currencies.monkey_money",
              "value": 2600,
              "metadata": "Let's give myself a little start boost"
          }
    ]
}
```

Response:
```json
{
  "transaction": {
    "success": true,
    "gotLock": true,
    "transactionsApplied": 3,
    "errors": ["", "", ""],
    "retryRequest": false,
    "info": "Completed with 0 errors"
  }, 
  "transactionKey": "tk.168iabvlti3.o",
  "lockDuration": 172800000,
  "fileData": {}
}
```