# Read / Set Multiple Decoder CVs - **Code 20**

This command allows read / write operations on up to four consecutive CVs. Since this currently uses the DCC Xpom mechanism, this is only available for DCC Addresses.

!!! info "Only one query of this type can be active at a time"

!!! warning 

    This requires both a relatively new Command Station AND Decoder firmware. To check if the package is supported, one test package may be sent. If this works, the software can assume that it will continue to work with this address (and Command Station(-Version))

## Available Messages

### [Primary](../../definition/message_type/primary.md)

| Offset |       Section |   Size | Field                | Value             | Description                     |
| -----: | ------------: | -----: | -------------------- | ----------------- | ------------------------------- |
|      0 |          Head | 1 Byte | [Unique Sequence-ID] | -                 | -                               |
|      1 |          Head | 1 Byte | [Header Info]        | -                 | -                               |
|      2 |          Head | 1 Byte | [Command Code]       | 20                | -                               |
|      3 |          Data | 2 Byte | [cAdr]               | ffaaaaaa aaaaaaaa | Decoder Address                 |
|      5 |          Data | 2 Byte | Index                | -                 | CV page index (CV31 CV32)       |
|      7 |          Data | 1 Byte | Variable             | -                 | CV index                        |
|      8 |          Data | 1 Byte | SequenceID           | \[0..3]           | Xpom SequenceID                 |
|      8 | Optional Data | 1 Byte | Value1               | -                 | If present, set as new CV-value |
|      9 | Optional Data | 1 Byte | Value2               | -                 | If present, set as new CV-value |
|     10 | Optional Data | 1 Byte | Value3               | -                 | If present, set as new CV-value |
|     11 | Optional Data | 1 Byte | Value4               | -                 | If present, set as new CV-value |

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

| Offset |       Section | Size   | Field                                   | Value             | Description                           |
| -----: | ------------: | ------ | --------------------------------------- | ----------------- | ------------------------------------- |
|      0 |          Head | 1 Byte | [Unique Sequence-ID]                    | -                 | -                                     |
|      1 |          Head | 1 Byte | [Header Info]                           | -                 | -                                     |
|      2 |          Head | 1 Byte | [Command Code]                          | 20                | -                                     |
|      3 |          Head | 1 Byte | [Flow Information]                      | dd00nnnn          | -                                     |
|      4 |          Head | 1 Byte | [Unique Sequence-ID] of primary message | -                 | -                                     |
|      5 |          Data | 1 Byte | cBusy                                   | 4                 | Unable to handle query ATM            |
|      6 |          Data | 2 Byte | [cAdr]                                  | ffaaaaaa aaaaaaaa | Decoder Address                       |
|      8 |          Data | 2 Byte | Index                                   | -                 | CV page index                         |
|     10 |          Data | 1 Byte | Variable                                | -                 | CV index                              |
|     11 |          Data | 1 Byte | SequenceID                              | -                 | Xpom SequenceID                       |
|     12 | Optional Data | 1 Byte | cID                                     | -                 | ID of the blocking query              |
|     13 | Optional Data | 1 Byte | cError                                  | -                 | Error                                 |
|     14 | Optional Data | 2 Byte | [cAdr]                                  | ffaaaaaa aaaaaaaa | Decoder Address of the blocking query |
|     16 | Optional Data | 2 Byte | Index                                   | -                 | CV page index of the blocking query   |
|     18 | Optional Data | 1 Byte | Variable                                | -                 | CV index of the blocking query        |
|     19 | Optional Data | 1 Byte | SequenceID                              | \[0..3]           | Xpom SequenceID of the blocking query |

---

### [Reply Level 2](../../definition/message_type/reply_level_2.md)

If the query was processed successfully, the package contains the input values, as well as the result values.

| Offset | Section | Size   | Field                                   | Value             | Description     |
| -----: | ------: | ------ | --------------------------------------- | ----------------- | --------------- |
|      0 |    Head | 1 Byte | [Unique Sequence-ID]                    | -                 | -               |
|      1 |    Head | 1 Byte | [Header Info]                           | -                 | -               |
|      2 |    Head | 1 Byte | [Command Code]                          | 20                | -               |
|      3 |    Head | 1 Byte | [Unique Sequence-ID] of primary message | -                 | -               |
|      4 |    Data | 2 Byte | [cAdr]                                  | ffaaaaaa aaaaaaaa | Decoder Address |
|      6 |    Data | 2 Byte | Index                                   | -                 | CV page index   |
|      8 |    Data | 1 Byte | Variable                                | -                 | CV index        |
|      9 |    Data | 1 Byte | SequenceID                              | \[0..3]           | Xpom SequenceID |
|     10 |    Data | 1 Byte | cError                                  | -                 | Error           |
|     11 |    Data | 1 Byte | cValue1                                 | -                 | First CV Value  |
|     12 |    Data | 1 Byte | cValue2                                 | -                 | Second CV Value |
|     13 |    Data | 1 Byte | cValue3                                 | -                 | Third CV Value  |
|     14 |    Data | 1 Byte | cValue4                                 | -                 | Fourth CV Value |

---

### [Error - Reply Level 2](../../definition/message_type/reply_level_2.md)

If any error occurred during processing (e.g. a timeout), this message is sent. The value of `cError` is currently not specified

| Offset | Section | Size   | Field                                   | Value             | Description     |
| -----: | ------: | ------ | --------------------------------------- | ----------------- | --------------- |
|      0 |    Head | 1 Byte | [Unique Sequence-ID]                    | -                 | -               |
|      1 |    Head | 1 Byte | [Header Info]                           | -                 | -               |
|      2 |    Head | 1 Byte | [Command Code]                          | 20                | -               |
|      3 |    Head | 1 Byte | [Unique Sequence-ID] of primary message | -                 | -               |
|      4 |    Data | 2 Byte | [cAdr]                                  | ffaaaaaa aaaaaaaa | Decoder Address |
|      6 |    Data | 1 Byte | cError                                  | -                 | Error           |

---

### [Level 2 Ack](../../definition/message_type/level_2_ack.md)

| Offset | Section | Size   | Field                                   | Value | Description |
| -----: | ------: | ------ | --------------------------------------- | ----- | ----------- |
|      0 |    Head | 1 Byte | [Unique Sequence-ID]                    | -     | -           |
|      1 |    Head | 1 Byte | [Header Info]                           | -     | -           |
|      2 |    Head | 1 Byte | [Command Code]                          | 20    | -           |
|      3 |    Head | 1 Byte | [Unique Sequence-ID] of primary message | -     | -           |

---

[cAdr]: ../../definition/globals/index.md#cadr-ffaaaaaa-aaaaaaaa
[Command Code]: ../../definition/structure/header/index.md#command-code
[Flow Information]: ../../definition/structure/header/index.md#flow-information-dd00nnnn
[Header Info]: ../../definition/structure/header/index.md#header-info
[Unique Sequence-ID]: ../../definition/structure/header/index.md#unique-sequence-id