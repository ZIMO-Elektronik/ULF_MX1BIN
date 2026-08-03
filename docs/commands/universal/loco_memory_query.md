# Loco Memory Query - **Code 8**

## Available Messages

### [Primary](../../definition/message_type/primary.md)

| Offset | Section |   Size | Field                | Value             | Description     |
| -----: | ------: | -----: | -------------------- | ----------------- | --------------- |
|      0 |    Head | 1 Byte | [Unique Sequence-ID] | -                 | -               |
|      1 |    Head | 1 Byte | [Header Info]        | -                 | -               |
|      2 |    Head | 1 Byte | [Command Code]       | 8                 | -               |
|      3 |    Data | 2 Byte | [cAdr]               | ffaaaaaa aaaaaaaa | Decoder Address |

---

### [Level 1 Ack](../../definition/message_type/level_1_ack.md)

| Offset |       Section |   Size | Field                                   | Value             | Description                |
| -----: | ------------: | -----: | --------------------------------------- | ----------------- | -------------------------- |
|      0 |          Head | 1 Byte | [Unique Sequence-ID]                    | -                 | -                          |
|      1 |          Head | 1 Byte | [Header Info]                           | -                 | -                          |
|      2 |          Head | 1 Byte | [Command Code]                          | 8                 | -                          |
|      3 |          Head | 1 Byte | [Unique Sequence-ID] of primary message | -                 | -                          |
|      4 |          Data | 1 Byte | Error                                   | -                 | If any - Error             |
|      5 |          Data | 2 Byte | [cAdr]                                  | ffaaaaaa aaaaaaaa | Decoder Address            |
|      7 |          Data | 1 Byte | [cSpeed](#cspeed-esssssss)              | -                 | Speed in Speed Step System |
|      8 |          Data | 1 Byte | [cData1](#cdata1)                       | -                 | -                          |
|      9 |          Data | 1 Byte | [cData2](#cdata2)                       | -                 | -                          |
|     10 |          Data | 1 Byte | [cData3](#cdata3)                       | -                 | -                          |
|     11 |          Data | 1 Byte | [cAzBz](#cazbz-aaaabbbb)                | -                 | -                          |
|     12 |          Data | 1 Byte | [cStatus](#cstatus)                     | -                 | -                          |
|     13 | Optional Data | 1 Byte | [cData4](#cdata4)                       | -                 | -                          |
|     14 | Optional Data | 1 Byte | [cData5](#cdata5)                       | -                 | -                          |

---

## Bitfields and Values

### **cSpeed** `esssssss`

| Bit(-s) | Acronym | Description    |
| ------: | ------: | -------------- |
|    \[7] |     `e` | Emergency stop |
| \[6..0] |     `s` | Speed          |

---

### **cData1**

| Bit(-s) | Description                    |
| ------: | ------------------------------ |
|    \[7] | Manual (ignore limits)         |
|    \[6] | Reserved (set to `0`)          |
|    \[5] | Direction (`0` fw, `1` bw)     |
|    \[4] | Headlights (`0` off, `1` on)   |
| \[3..2] | Speed step system              |
|    \[1] | Deceleration (`0` off, `1` on) |
|    \[0] | Acceleration (`0` off, `1` on) |

---

### **cData2**

| Bit(-s) | Description |
| ------: | ----------- |
|    \[7] | F1          |
|     ... | ...         |
|    \[0] | F8          |

---

### **cData3**

| Bit(-s) | Description           |
| ------: | --------------------- |
| \[7..4] | Reserved (set to `0`) |
|    \[3] | F9                    |
|     ... | ...                   |
|    \[0] | F12                   |

---

### **cAzBz** `aaaabbbb`

| Bit(-s) | Acronym | Description       |
| ------: | ------: | ----------------- |
| \[7..4] |     `a` | Acceleration time |
| \[3..0] |     `b` | Deceleration time |

---

### **cStatus**

| Value | Description   |
| ----: | ------------- |
|   `0` | Loco inactive |
|   `1` | Loco active   |

### **cData4**

| Bit(-s) | Description |
| ------: | ----------- |
|    \[7] | F13         |
|     ... | ...         |
|    \[0] | F20         |

---

### **cData5**

| Bit(-s) | Description |
| ------: | ----------- |
|    \[7] | F21         |
|     ... | ...         |
|    \[0] | F28         |

---

[cAdr]: ../../definition/globals/index.md#cadr-ffaaaaaa-aaaaaaaa
[Command Code]: ../../definition/structure/header/index.md#command-code
[Header Info]: ../../definition/structure/header/index.md#header-info
[Unique Sequence-ID]: ../../definition/structure/header/index.md#unique-sequence-id