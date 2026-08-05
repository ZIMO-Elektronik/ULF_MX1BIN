# Address Control - **Code 10**

## Available Messages

### [Primary](../../definition/message_type/primary.md)

| Offset |       Section |   Size | Field                | Value               | Description         |
| -----: | ------------: | -----: | -------------------- | ------------------- | ------------------- |
|      0 |          Head | 1 Byte | [Unique Sequence-ID] | -                   | -                   |
|      1 |          Head | 1 Byte | [Header Info]        | -                   | -                   |
|      2 |          Head | 1 Byte | [Command Code]       | 10                  | -                   |
|      3 |          Data | 2 Byte | [cAdr]               | `ffaaaaaa aaaaaaaa` | Decoder Address     |
|      5 |          Data | 1 Byte | cControl             | -                   | Control information |
|      6 | Optional Data | 1 Byte | cOutputs             | -                   | Outputs to lock     |

---

### [Level 1 Ack](../../definition/message_type/level_1_ack.md)

| Offset |       Section | Size   | Field                                   | Value      | Description    |
| -----: | ------------: | ------ | --------------------------------------- | ---------- | -------------- |
|      0 |          Head | 1 Byte | [Unique Sequence-ID]                    | -          | -              |
|      1 |          Head | 1 Byte | [Header Info]                           | -          | -              |
|      2 |          Head | 1 Byte | [Command Code]                          | 10         | -              |
|      3 |          Head | 1 Byte | [Unique Sequence-ID] of primary message | -          | -              |
|      4 |          Data | 1 Byte | Error                                   | -          | If any - Error |
|      5 |          Data | 1 Byte | [Status](#status-ffa0sskl)              | `ffa0sskl` | -              |
|      6 | Optional Data | 1 Byte | cOutputs                                | -          | Locked outputs |

---

## Bitfields and Values

### **Status** `ffa0sskl`

| Bit(-s) | Acronym | Description                            |
| ------: | ------: | -------------------------------------- |
| \[7..6] |     `f` | Format Specifier                       |
|    \[5] |     `a` | Address type (`0` loco, `1` accessory) |
|    \[4] |       - | Reserved (set to `0`)                  |
| \[3..2] |     `s` | Speed Step System                      |
|    \[1] |     `k` | Locked (`0` not locked, `1` locked)    |
|    \[0] |     `l` | Log changes (`0` off, `1` on)          |

---

[cAdr]: ../../definition/globals/index.md#cadr-ffaaaaaa-aaaaaaaa
[Command Code]: ../../definition/structure/header/index.md#command-code
[Header Info]: ../../definition/structure/header/index.md#header-info
[Unique Sequence-ID]: ../../definition/structure/header/index.md#unique-sequence-id