# Read Command Station I/O State - **Code 11**

## Available Messages

### [Primary](../../definition/message_type/primary.md)

!!! info "No one really seems to know what `zero` does, but we can't remove it without tests"

| Offset | Section |   Size | Field                | Value | Description |
| -----: | ------: | -----: | -------------------- | ----- | ----------- |
|      0 |    Head | 1 Byte | [Unique Sequence-ID] | -     | -           |
|      1 |    Head | 1 Byte | [Header Info]        | -     | -           |
|      2 |    Head | 1 Byte | [Command Code]       | 11    | -           |
|      3 |    Data | 1 Byte | `zero`               | 0     | -           |

### [Level 1 Ack](../../definition/message_type/level_1_ack.md)

!!! info "No one really seems to know what `zero` does, but we can't remove it without tests"

| Offset | Section | Size   | Field                                   | Value | Description                      |
| -----: | ------: | ------ | --------------------------------------- | ----- | -------------------------------- |
|      0 |    Head | 1 Byte | [Unique Sequence-ID]                    | -     | -                                |
|      1 |    Head | 1 Byte | [Header Info]                           | -     | -                                |
|      2 |    Head | 1 Byte | [Command Code]                          | 11    | -                                |
|      3 |    Head | 1 Byte | [Unique Sequence-ID] of primary message | -     | -                                |
|      4 |    Data | 1 Byte | `zero`                                  | 0     | -                                |
|      5 |    Data | 2 Byte | [cCurrent1](#ccurrentx)                 | -     | $\text{I}_1 = value \cdot 0.01A$ |
|      7 |    Data | 1 Byte | cVoltage1                               | -     | $\text{U}_1 = value \cdot 0.1V$  |
|      8 |    Data | 2 Byte | [cCurrent2](#ccurrentx)                 | -     | $\text{I}_2 = value \cdot 0.01A$ |
|     10 |    Data | 1 Byte | cVoltage2                               | -     | $\text{U}_2 = value \cdot 0.1V$  |
|     11 |    Data | 1 Byte | cAux                                    | -     | Auxiliary input                  |

## Bitfields and Values

### **cCurrentx**

Both values of cCurrent can double as one of the special values below. This only applies if bit \[15] is set

!!! info "Note:"

    If no special value applies, the actual value can be calculated by $\text{I}_x = value \cdot 0.01A$.

| Special Value | Description                      |
| ------------: | :------------------------------- |
|        0x8000 | no 2<sup>nd</sup> value (MX1_EC) |
|        0x8001 | "OFF"                            |
|        0x8002 | "UEP"                            |
|        0x8003 | "UES"                            |
|        0x8004 | "AUS"                            |
|        0x8005 | "SSP"                            |
|        0x8006 | "No Si"                          |
|        0x8007 | "SL UES"                         |

---

[Command Code]: ../../definition/structure/header/index.md#command-code
[Header Info]: ../../definition/structure/header/index.md#header-info
[Unique Sequence-ID]: ../../definition/structure/header/index.md#unique-sequence-id