# Track Control - **Code 2**

The `Track Control` message allows the control and query of the track. Most notably the power state (`on` / `off`).

## Available Messages

### [Primary](../../definition/message_type/primary.md)

| Offset | Area |   Size | Field                | Value   |
| -----: | ---: | -----: | -------------------- | ------- |
|      0 | Head | 1 Byte | [Unique Sequence-ID] | -       |
|      1 | Head | 1 Byte | [Header Info]        | -       |
|      2 | Head | 1 Byte | [Command Code]       | 2       |
|      3 | Data | 1 Byte | [cAction](#caction)  | \[0..3] |

---

### [Level 1 Ack](../../definition/message_type/level_1_ack.md)

| Offset | Area |   Size | Field                                   | Value      |
| -----: | ---: | -----: | --------------------------------------- | ---------- |
|      0 | Head | 1 Byte | [Unique Sequence-ID]                    | -          |
|      1 | Head | 1 Byte | [Header Info]                           | -          |
|      2 | Head | 1 Byte | [Command Code]                          | 2          |
|      3 | Head | 1 Byte | [Unique Sequence-ID] of primary message | -          |
|      4 | Data | 1 Byte | [StatusBits](#statusbits-dm000uts)      | `dm000uts` |

---

## Bitfields and Values

### **cAction**

| Value | Description                               |
| ----: | ----------------------------------------- |
|     0 | Stop broadcast (stop all locos)           |
|     1 | Track voltage `OFF`                       |
|     2 | Track voltage `ON` and leave service mode |
|     3 | Query status                              |

### **StatusBits** `dm000uts`

| Bit(-s) | Acronym | Description                      |
| ------: | ------: | -------------------------------- |
|    \[7] |     `d` | DCC (`1` on, `0` off)            |
|    \[6] |     `m` | Motorola (`1` on, `0` off)       |
| \[5..3] |     `0` | Reserved (set to 0)              |
|    \[2] |     `u` | UES (`1` on, `0` off)            |
|    \[1] |     `t` | Track voltage (`1` on, `0` off)  |
|    \[0] |     `s` | Broadcast stop (`1` on, `0` off) |

---

[Command Code]: ../../definition/structure/header/index.md#command-code
[Header Info]: ../../definition/structure/header/index.md#header-info
[Unique Sequence-ID]: ../../definition/structure/header/index.md#unique-sequence-id