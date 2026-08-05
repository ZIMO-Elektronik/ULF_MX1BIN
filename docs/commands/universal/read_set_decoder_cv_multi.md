# Read / Set Multiple Decoder CVs - **Code 20**

This command allows read / write operations on up to four consecutive CVs. Since this
currently uses the DCC Xpom mechanism, this is only available for DCC Addresses.

To allow overlapping queries (for more info, consult the RCN214 and RCN217 respectively),
up to **four** queries can be open at a time. The requirement for this is, that all queries
must have a matching decoder Address **_AND_** have unique Xpom sequence identifiers.

!!! warning 

    This requires both a relatively new Command Station AND Decoder firmware. To check
    if the package is supported, one test package may be sent. If this works, the software
    can assume that it will continue to work with this address (and Command Station(-Version))

## Available Messages

### [Primary](../../definition/message_type/primary.md)

| Offset |       Section |   Size | Field                | Value               | Description                     |
| -----: | ------------: | -----: | -------------------- | ------------------- | ------------------------------- |
|      0 |          Head | 1 Byte | [Unique Sequence-ID] | -                   | -                               |
|      1 |          Head | 1 Byte | [Header Info]        | -                   | -                               |
|      2 |          Head | 1 Byte | [Command Code]       | 20                  | -                               |
|      3 |          Data | 2 Byte | [cAdr]               | `ffaaaaaa aaaaaaaa` | Decoder Address                 |
|      5 |          Data | 2 Byte | Index                | -                   | CV page index (CV31 CV32)       |
|      7 |          Data | 1 Byte | Variable             | -                   | CV index                        |
|      8 |          Data | 1 Byte | SequenceID           | \[0..3]             | Xpom SequenceID                 |
|      9 | Optional Data | 1 Byte | Value1               | -                   | If present, set as new CV-value |
|     10 | Optional Data | 1 Byte | Value2               | -                   | If present, set as new CV-value |
|     11 | Optional Data | 1 Byte | Value3               | -                   | If present, set as new CV-value |
|     12 | Optional Data | 1 Byte | Value4               | -                   | If present, set as new CV-value |

!!! info "The given CV is read if the `Value` bytes ommitted. Otherwise the number and values of CVs written matches the `Value` list (up to four)"

---

### [Level 1 Ack](../../definition/message_type/level_1_ack.md)

This response is sent if the query is open and being processed.

| Offset | Section | Size   | Field                                   | Value | Description |
| -----: | ------: | ------ | --------------------------------------- | ----- | ----------- |
|      0 |    Head | 1 Byte | [Unique Sequence-ID]                    | -     | -           |
|      1 |    Head | 1 Byte | [Header Info]                           | -     | -           |
|      2 |    Head | 1 Byte | [Command Code]                          | 20    | -           |
|      3 |    Head | 1 Byte | [Unique Sequence-ID] of primary message | -     | -           |

---

### [Busy - Level 1 Ack](../../definition/message_type/level_1_ack.md)

If the command station is either busy with another query of this type (or can't handle another query for some reason), the busy package is sent.

!!! info "If another query is active, usually this package contains the metadata from this query"

| Offset |       Section | Size   | Field                                   | Value               | Description                           |
| -----: | ------------: | ------ | --------------------------------------- | ------------------- | ------------------------------------- |
|      0 |          Head | 1 Byte | [Unique Sequence-ID]                    | -                   | -                                     |
|      1 |          Head | 1 Byte | [Header Info]                           | -                   | -                                     |
|      2 |          Head | 1 Byte | [Command Code]                          | 20                  | -                                     |
|      3 |          Head | 1 Byte | [Flow Information]                      | dd00nnnn            | -                                     |
|      4 |          Head | 1 Byte | [Unique Sequence-ID] of primary message | -                   | -                                     |
|      5 |          Data | 1 Byte | cBusy                                   | 4                   | Unable to handle query ATM            |
|      6 |          Data | 2 Byte | [cAdr]                                  | `ffaaaaaa aaaaaaaa` | Decoder Address                       |
|      8 |          Data | 2 Byte | Index                                   | -                   | CV page index                         |
|     10 |          Data | 1 Byte | Variable                                | -                   | CV index                              |
|     11 |          Data | 1 Byte | SequenceID                              | -                   | Xpom SequenceID                       |
|     12 | Optional Data | 1 Byte | cID                                     | -                   | ID of the blocking query              |
|     13 | Optional Data | 1 Byte | cError                                  | -                   | Error                                 |
|     14 | Optional Data | 2 Byte | [cAdr]                                  | `ffaaaaaa aaaaaaaa` | Decoder Address of the blocking query |
|     16 | Optional Data | 2 Byte | Index                                   | -                   | CV page index of the blocking query   |
|     18 | Optional Data | 1 Byte | Variable                                | -                   | CV index of the blocking query        |
|     19 | Optional Data | 1 Byte | SequenceID                              | \[0..3]             | Xpom SequenceID of the blocking query |

---

### [Reply Level 2](../../definition/message_type/reply_level_2.md)

If the query was processed successfully, the package contains the input values, as well as the result values.

| Offset | Section | Size   | Field                                   | Value               | Description     |
| -----: | ------: | ------ | --------------------------------------- | ------------------- | --------------- |
|      0 |    Head | 1 Byte | [Unique Sequence-ID]                    | -                   | -               |
|      1 |    Head | 1 Byte | [Header Info]                           | -                   | -               |
|      2 |    Head | 1 Byte | [Command Code]                          | 20                  | -               |
|      3 |    Head | 1 Byte | [Unique Sequence-ID] of primary message | -                   | -               |
|      4 |    Data | 2 Byte | [cAdr]                                  | `ffaaaaaa aaaaaaaa` | Decoder Address |
|      6 |    Data | 2 Byte | Index                                   | -                   | CV page index   |
|      8 |    Data | 1 Byte | Variable                                | -                   | CV index        |
|      9 |    Data | 1 Byte | SequenceID                              | \[0..3]             | Xpom SequenceID |
|     10 |    Data | 1 Byte | cError                                  | -                   | Error           |
|     11 |    Data | 1 Byte | cValue1                                 | -                   | First CV Value  |
|     12 |    Data | 1 Byte | cValue2                                 | -                   | Second CV Value |
|     13 |    Data | 1 Byte | cValue3                                 | -                   | Third CV Value  |
|     14 |    Data | 1 Byte | cValue4                                 | -                   | Fourth CV Value |

---

### [Error - Reply Level 2](../../definition/message_type/reply_level_2.md)

If any error occurred during processing (e.g. a timeout), this message is sent. The value of `cError` is currently not specified

| Offset | Section | Size   | Field                                   | Value               | Description     |
| -----: | ------: | ------ | --------------------------------------- | ------------------- | --------------- |
|      0 |    Head | 1 Byte | [Unique Sequence-ID]                    | -                   | -               |
|      1 |    Head | 1 Byte | [Header Info]                           | -                   | -               |
|      2 |    Head | 1 Byte | [Command Code]                          | 20                  | -               |
|      3 |    Head | 1 Byte | [Unique Sequence-ID] of primary message | -                   | -               |
|      4 |    Data | 2 Byte | [cAdr]                                  | `ffaaaaaa aaaaaaaa` | Decoder Address |
|      6 |    Data | 1 Byte | cError                                  | -                   | Error           |

---

### [Level 2 Ack](../../definition/message_type/level_2_ack.md)

| Offset | Section | Size   | Field                                   | Value | Description |
| -----: | ------: | ------ | --------------------------------------- | ----- | ----------- |
|      0 |    Head | 1 Byte | [Unique Sequence-ID]                    | -     | -           |
|      1 |    Head | 1 Byte | [Header Info]                           | -     | -           |
|      2 |    Head | 1 Byte | [Command Code]                          | 20    | -           |
|      3 |    Head | 1 Byte | [Unique Sequence-ID] of primary message | -     | -           |

---

## Example

To provide an example, we can just read the CV `1` till `4` on the `DCC` decoder with the address `3`

``` mermaid
sequenceDiagram
    participant PC
    participant CommandStation

    PC->>CommandStation: [ID 1] Read Address 3 CV 1
    CommandStation-->>PC: Ack [ID 128] [ReID = 1]
    Note right of PC: Wait till response
    CommandStation->>PC: ReplyL2 [ID 129] [ReID = 1] [Data of query plus result]
    PC-->>CommandStation: AckL2 [ID 3] [ReID = 129]
    Note right of PC: Ready for next query
```

To make use of the overlapping queries, we must make sure, that the Xpom sequence ID is unique

``` mermaid
sequenceDiagram
    participant PC
    participant CommandStation

    PC->>CommandStation: [ID 1] Read Address 3 CV 1 - 4
    CommandStation-->>PC: Ack [ID 128] [ReID = 1]
    PC->>CommandStation: [ID 2] Read Address 3 CV 5 - 8
    CommandStation-->>PC: Ack [ID 129] [ReID = 2]
    PC->>CommandStation: [ID 3] Read Address 3 CV 9 - 12
    CommandStation-->>PC: Ack [ID 130] [ReID = 3]
    Note right of PC: Accidentally attempt to read Address 4
    PC->>CommandStation: [ID 4] Read Address 4 CV 13 - 16
    CommandStation-->>PC: Busy [ID 131] [ReID = 4] [Data of rejected query] [Data of first blocking query]
    Note right of PC: Send correct address now
    PC->>CommandStation: [ID 5] Read Address 3 CV 13 - 16
    CommandStation-->>PC: Ack [ID 132] [ReID = 5]
    Note right of PC: Wait till response
    CommandStation->>PC: ReplyL2 [ID 133] [ReID = 1] [Data of query plus result]
    PC-->>CommandStation: AckL2 [ID 6] [ReID = 133]
    CommandStation->>PC: ReplyL2 [ID 134] [ReID = 2] [Data of query plus result]
    PC-->>CommandStation: AckL2 [ID 7] [ReID = 134]
    CommandStation->>PC: ReplyL2 [ID 135] [ReID = 3] [Data of query plus result]
    PC-->>CommandStation: AckL2 [ID 8] [ReID = 135]
    CommandStation->>PC: ReplyL2 [ID 136] [ReID = 5] [Data of query plus result]
    PC-->>CommandStation: AckL2 [ID 9] [ReID = 136]
    Note right of PC: Now we can read Address 4
    PC->>CommandStation: [ID 10] Read Address 4 CV 13 - 16
    CommandStation-->>PC: Ack [ID 137] [ReID = 10]
    CommandStation->>PC: ReplyL2 [ID 138] [ReID = 10] [Data of query plus result]
    PC-->>CommandStation: AckL2 [ID 11] [ReID = 138]
```

[cAdr]: ../../definition/globals/index.md#cadr-ffaaaaaa-aaaaaaaa
[Command Code]: ../../definition/structure/header/index.md#command-code
[Flow Information]: ../../definition/structure/header/index.md#flow-information-dd00nnnn
[Header Info]: ../../definition/structure/header/index.md#header-info
[Unique Sequence-ID]: ../../definition/structure/header/index.md#unique-sequence-id