# Command Station Equipment Query - **Code 13**

## Available Messages

### [Primary](../../definition/message_type/primary.md)

!!! info "No one really seems to know what `zero` does"

| Offset | Section |   Size | Field                | Value | Description |
| -----: | ------: | -----: | -------------------- | ----- | ----------- |
|      0 |    Head | 1 Byte | [Unique Sequence-ID] | -     | -           |
|      1 |    Head | 1 Byte | [Header Info]        | -     | -           |
|      2 |    Head | 1 Byte | [Command Code]       | 13    | -           |
|      3 |    Data | 1 Byte | `zero`               | 0     | -           |

---

### [Level 1 Ack](../../definition/message_type/level_1_ack.md)

!!! info "No one really seems to know what `zero` does, but we can't exactly remove it from the middle of the message"

| Offset |       Section | Size   | Field                                   | Value | Description                 |
| -----: | ------------: | ------ | --------------------------------------- | ----- | --------------------------- |
|      0 |          Head | 1 Byte | [Unique Sequence-ID]                    | -     | -                           |
|      1 |          Head | 1 Byte | [Header Info]                           | -     | -                           |
|      2 |          Head | 1 Byte | [Command Code]                          | 13    | -                           |
|      3 |          Head | 1 Byte | [Unique Sequence-ID] of primary message | -     | -                           |
|      4 |          Head | 1 Byte | length                                  | 5     | -                           |
|      5 |          Data | 2 Byte | cAddress                                | -     | Unique (CAN-)address        |
|      7 |          Data | 1 Byte | [cDevice](#cdevice)                     | -     | Device ID                   |
|      8 |          Data | 1 Byte | cRom_size                               | -     | ROM size of command station |
|      9 |          Data | 1 Byte | cRam_size                               | -     | RAM size of command station |
|     10 |          Data | 2 Byte | cPrintVer                               | -     | Print version number?       |
|     12 |          Data | 1 Byte | cVersion_h                              | -     | SW-Version major            |
|     13 |          Data | 1 Byte | cVersion_m                              | -     | SW-Version minor            |
|     14 |          Data | 1 Byte | cDate_day                               | `D`   | Day of SW release           |
|     15 |          Data | 1 Byte | cDate_month                             | `M`   | Month of SW release         |
|     16 |          Data | 1 Byte | cDate_century                           | `C`   | Century of SW release       |
|     17 |          Data | 1 Byte | cDate_year                              | `Y`   | Year of SW release          |
|     18 |          Data | 1 Byte | cSwitches                               | -     | Switches                    |
|     19 |          Data | 1 Byte | cVersion_l                              | -     | SW-Version patch            |
|     20 |          Data | 1 Byte | cBootRom                                | -     | Boot ROM version number     |
|     21 |          Data | 1 Byte | `zero`                                  | `0`   | Values for command station? |
|     22 | Optional Data | 4 Byte | cSerNum                                 | -     | 4-byte Serial number        |

---

## Bitfields and Values

### **cDevice**

The Device ID is an identifier for the actual device. This means, for every device supporting this protocol, the list needs to be extended

| Value | Device        |
| ----: | ------------- |
|     1 | MX1 2000 / HS |
|     2 | MX1 2000 EC   |
|     3 | MX32 ZL       |
|     4 | MXULF         |
|     5 | KLUG          |

---

[Command Code]: ../../definition/structure/header/index.md#command-code
[Header Info]: ../../definition/structure/header/index.md#header-info
[Unique Sequence-ID]: ../../definition/structure/header/index.md#unique-sequence-id