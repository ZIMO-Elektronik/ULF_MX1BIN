# Accessory Memory Query - **Code 9**

## Available Messages

### [Primary](../../definition/message_type/primary.md)

| Offset | Section |   Size | Field                | Value             | Description     |
| -----: | ------: | -----: | -------------------- | ----------------- | --------------- |
|      0 |    Head | 1 Byte | [Unique Sequence-ID] | -                 | -               |
|      1 |    Head | 1 Byte | [Header Info]        | -                 | -               |
|      2 |    Head | 1 Byte | [Command Code]       | 9                 | -               |
|      3 |    Data | 2 Byte | [cAdr]               | ffaaaaaa aaaaaaaa | Decoder Address |

---

### [Level 1 Ack](../../definition/message_type/level_1_ack.md)

| Offset | Section |   Size | Field                                   | Value             | Description      |
| -----: | ------: | -----: | --------------------------------------- | ----------------- | ---------------- |
|      0 |    Head | 1 Byte | [Unique Sequence-ID]                    | -                 | -                |
|      1 |    Head | 1 Byte | [Header Info]                           | -                 | -                |
|      2 |    Head | 1 Byte | [Command Code]                          | 9                 | -                |
|      3 |    Head | 1 Byte | [Unique Sequence-ID] of primary message | -                 | -                |
|      4 |    Data | 1 Byte | Error                                   | -                 | If any - Error   |
|      5 |    Data | 2 Byte | [cAdr]                                  | ffaaaaaa aaaaaaaa | Decoder Address  |
|      7 |    Data | 1 Byte | [cPair](#cpair)                         | -                 |                  |
|      8 |    Data | 1 Byte | cOutputs                                | -                 | State of outputs |

---

## Bitfields and Values

### **cPair**

| Value | Description            |
| ----: | ---------------------- |
|   `0` | Paired output function |
|   `1` | Single output function |

---

[cAdr]: ../../definition/globals/index.md#cadr-ffaaaaaa-aaaaaaaa
[Command Code]: ../../definition/structure/header/index.md#command-code
[Header Info]: ../../definition/structure/header/index.md#header-info
[Unique Sequence-ID]: ../../definition/structure/header/index.md#unique-sequence-id