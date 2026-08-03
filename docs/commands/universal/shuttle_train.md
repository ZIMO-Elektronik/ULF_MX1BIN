# Shuttle Train - **Code 6**

## Available Messages

### [Primary](../../definition/message_type/primary.md)

| Offset | Section |   Size | Field                    | Value             | Description              |
| -----: | ------: | -----: | ------------------------ | ----------------- | ------------------------ |
|      0 |    Head | 1 Byte | [Unique Sequence-ID]     | -                 | -                        |
|      1 |    Head | 1 Byte | [Header Info]            | -                 | -                        |
|      2 |    Head | 1 Byte | [Command Code]           | 6                 | -                        |
|      3 |    Data | 2 Byte | [cAdr]                   | ffaaaaaa aaaaaaaa | Loco Address             |
|      5 |    Data | 1 Byte | [cData](#cdata-rrrrffff) | rrrrffff          | Contact rail information |

---

### [Level 1 Ack](../../definition/message_type/level_1_ack.md)

| Offset |       Section | Size   | Field                                   | Value | Description    |
| -----: | ------------: | ------ | --------------------------------------- | ----- | -------------- |
|      0 |          Head | 1 Byte | [Unique Sequence-ID]                    | -     | -              |
|      1 |          Head | 1 Byte | [Header Info]                           | -     | -              |
|      2 |          Head | 1 Byte | [Command Code]                          | 3     | -              |
|      3 |          Head | 1 Byte | [Unique Sequence-ID] of primary message | -     | -              |
|      4 |          Data | 1 Byte | Error                                   | -     | If any - Error |
|      5 | Optional Data | 1 Byte | [Status](#status-ff00ss0t)              | -     | -              |

---

## Bitfields and Values

### **cData** `rrrrffff`

| Bit(-s) | Acronym | Description                   |
| ------: | ------: | ----------------------------- |
| \[7..4] |     `r` | Contact rails \[4..1] forward |
| \[3..0] |     `f` | Contact raild \[4..1] reverse |

---

### **Status** `ff00ss0t`

| Bit(-s) | Acronym | Description                         |
| ------: | ------: | ----------------------------------- |
| \[7..6] |     `f` | Format Specifier                    |
| \[5..4] |       - | Reserved (set to `0`)               |
| \[3..2] |     `s` | Speed Step System                   |
|    \[1] |       - | Reserved (set to `0`)               |
|    \[0] |     `t` | Track state (`0` normal, `1` fault) |

---

[cAdr]: ../../definition/globals/index.md#cadr-ffaaaaaa-aaaaaaaa
[Command Code]: ../../definition/structure/header/index.md#command-code
[Header Info]: ../../definition/structure/header/index.md#header-info
[Unique Sequence-ID]: ../../definition/structure/header/index.md#unique-sequence-id