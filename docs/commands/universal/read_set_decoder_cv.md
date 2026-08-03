# Read / Set Decoder CV - **Code 19**

## Available Messages

### [Primary](../../definition/message_type/primary.md)

| Offset |       Section |   Size | Field                | Value             | Description                     |
| -----: | ------------: | -----: | -------------------- | ----------------- | ------------------------------- |
|      0 |          Head | 1 Byte | [Unique Sequence-ID] | -                 | -                               |
|      1 |          Head | 1 Byte | [Header Info]        | -                 | -                               |
|      2 |          Head | 1 Byte | [Command Code]       | 19                | -                               |
|      3 |          Data | 2 Byte | [cAdr]               | ffaaaaaa aaaaaaaa | Decoder Address                 |
|      5 |          Data | 2 Byte | Variable             | -                 | CV Address                      |
|      7 | Optional Data | 1 Byte | Value                | -                 | If present, set as new CV-value |

---

### [Level 1 Ack](../../definition/message_type/level_1_ack.md)

| Offset | Section | Size   | Field                                   | Value | Description |
| -----: | ------: | ------ | --------------------------------------- | ----- | ----------- |
|      0 |    Head | 1 Byte | [Unique Sequence-ID]                    | -     | -           |
|      1 |    Head | 1 Byte | [Header Info]                           | -     | -           |
|      2 |    Head | 1 Byte | [Command Code]                          | 19    | -           |
|      3 |    Head | 1 Byte | [Unique Sequence-ID] of primary message | -     | -           |

---

### [Busy - Level 1 Ack](../../definition/message_type/level_1_ack.md)

| Offset |       Section | Size   | Field                                   | Value             | Description                           |
| -----: | ------------: | ------ | --------------------------------------- | ----------------- | ------------------------------------- |
|      0 |          Head | 1 Byte | [Unique Sequence-ID]                    | -                 | -                                     |
|      1 |          Head | 1 Byte | [Header Info]                           | -                 | -                                     |
|      2 |          Head | 1 Byte | [Command Code]                          | 19                | -                                     |
|      3 |          Head | 1 Byte | [Unique Sequence-ID] of primary message | -                 | -                                     |
|      4 |          Data | 1 Byte | cBusy                                   | 4                 | Unable to handle query ATM            |
|      5 |          Data | 2 Byte | [cAdr]                                  | ffaaaaaa aaaaaaaa | Decoder Address                       |
|      7 |          Data | 2 Byte | Variable                                | -                 | CV Address                            |
|      9 | Optional Data | 1 Byte | cID                                     | -                 | ID of the blocking query              |
|     10 | Optional Data | 1 Byte | cError                                  | -                 | Error                                 |
|     11 | Optional Data | 2 Byte | [cAdr]                                  | ffaaaaaa aaaaaaaa | Decoder Address of the blocking query |
|     13 | Optional Data | 2 Byte | Variable                                | -                 | CV Address of the blocking query      |

---

### [Reply Level 2](../../definition/message_type/reply_level_2.md)

| Offset | Section | Size   | Field                                   | Value             | Description     |
| -----: | ------: | ------ | --------------------------------------- | ----------------- | --------------- |
|      0 |    Head | 1 Byte | [Unique Sequence-ID]                    | -                 | -               |
|      1 |    Head | 1 Byte | [Header Info]                           | -                 | -               |
|      2 |    Head | 1 Byte | [Command Code]                          | 19                | -               |
|      3 |    Head | 1 Byte | [Unique Sequence-ID] of primary message | -                 | -               |
|      4 |    Data | 2 Byte | [cAdr]                                  | ffaaaaaa aaaaaaaa | Decoder Address |
|      6 |    Data | 2 Byte | Variable                                | -                 | CV Address      |
|      8 |    Data | 1 Byte | cValue                                  | -                 | CV Value        |
|      9 |    Data | 1 Byte | cError                                  | -                 | Error           |

---

### [Error - Reply Level 2](../../definition/message_type/reply_level_2.md)

| Offset | Section | Size   | Field                                   | Value             | Description     |
| -----: | ------: | ------ | --------------------------------------- | ----------------- | --------------- |
|      0 |    Head | 1 Byte | [Unique Sequence-ID]                    | -                 | -               |
|      1 |    Head | 1 Byte | [Header Info]                           | -                 | -               |
|      2 |    Head | 1 Byte | [Command Code]                          | 19                | -               |
|      3 |    Head | 1 Byte | [Unique Sequence-ID] of primary message | -                 | -               |
|      4 |    Data | 2 Byte | [cAdr]                                  | ffaaaaaa aaaaaaaa | Decoder Address |
|      6 |    Data | 1 Byte | cError                                  | -                 | Error           |

---

### [Level 2 Ack](../../definition/message_type/level_2_ack.md)

| Offset | Section | Size   | Field                                   | Value | Description |
| -----: | ------: | ------ | --------------------------------------- | ----- | ----------- |
|      0 |    Head | 1 Byte | [Unique Sequence-ID]                    | -     | -           |
|      1 |    Head | 1 Byte | [Header Info]                           | -     | -           |
|      2 |    Head | 1 Byte | [Command Code]                          | 19    | -           |
|      3 |    Head | 1 Byte | [Unique Sequence-ID] of primary message | -     | -           |

---

[cAdr]: ../../definition/globals/index.md#cadr-ffaaaaaa-aaaaaaaa
[Command Code]: ../../definition/structure/header/index.md#command-code
[Header Info]: ../../definition/structure/header/index.md#header-info
[Unique Sequence-ID]: ../../definition/structure/header/index.md#unique-sequence-id