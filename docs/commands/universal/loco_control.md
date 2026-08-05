# Loco Control - **Code 3**

This command is used to control the drive data of a loco via the protocol.

## Available Messages

### [Primary](../../definition/message_type/primary.md)

| Offset |       Section |   Size | Field                      | Value               | Description                        |
| -----: | ------------: | -----: | -------------------------- | ------------------- | ---------------------------------- |
|      0 |          Head | 1 Byte | [Unique Sequence-ID]       | -                   | -                                  |
|      1 |          Head | 1 Byte | [Header Info]              | -                   | -                                  |
|      2 |          Head | 1 Byte | [Command Code]             | 3                   | -                                  |
|      3 |          Data | 2 Byte | [cAdr]                     | `ffaaaaaa aaaaaaaa` | Loco Address                       |
|      5 |          Data | 1 Byte | [cSpeed](#cspeed-esssssss) | `esssssss`          | Loco Speed                         |
|      6 | Optional Data | 1 Byte | [cData1](#cdata1)          | -                   | -                                  |
|      7 | Optional Data | 1 Byte | [cData2](#cdata2)          | -                   | Function group F1..F8 (DCC only)   |
|      8 | Optional Data | 1 Byte | [cData3](#cdata3)          | -                   | Function group F9..F12 (DCC only)  |
|      9 | Optional Data | 1 Byte | [cData4](#cdata4)          | -                   | Function group F13..F20 (DCC only) |
|     10 | Optional Data | 1 Byte | [cData5](#cdata5)          | -                   | Function group F21..F28 (DCC only) |

---

### [Level 1 Ack](../../definition/message_type/level_1_ack.md)

| Offset |       Section | Size   | Field                                   | Value      | Description    |
| -----: | ------------: | ------ | --------------------------------------- | ---------- | -------------- |
|      0 |          Head | 1 Byte | [Unique Sequence-ID]                    | -          | -              |
|      1 |          Head | 1 Byte | [Header Info]                           | -          | -              |
|      2 |          Head | 1 Byte | [Command Code]                          | 3          | -              |
|      3 |          Head | 1 Byte | [Unique Sequence-ID] of primary message | -          | -              |
|      4 |          Data | 1 Byte | Error                                   | -          | If any - Error |
|      5 | Optional Data | 1 Byte | [Status](#status-ff00ss0t)              | `ff00ss0t` | -              |

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