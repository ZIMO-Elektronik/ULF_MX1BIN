# Globals

Within the `MX1Bin` protocol, many definitions are used, some of them across the protocol. These "Global" definitions are collected here.

!!! info "This page exists mainly to avoid having to define them over and over again per messgage"

## Bitfields

### **info**

The `info` byte is the second byte of each message. It contains metadata that will assist in parsing a stream into a structured message.

| Bit(-s) |                              Value | Name         | Description                                                                                                                                                                                                    |
| ------: | ---------------------------------: | ------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
|    \[7] |                       `0` <br> `1` | Frame Type   | Short Frame <br> Long Frame                                                                                                                                                                                    |
| \[6..5] | `00` <br> `10` <br> `01` <br> `11` | Message Type | [Primary](../message_type/primary.md) <br> [Level 1 Ack](../message_type/level_1_ack.md#level-1-ack) <br> [Reply Level 2](../message_type/reply_level_2.md) <br> [Level 2 Ack](../message_type/level_2_ack.md) |
|    \[4] |                       `0` <br> `1` | Sender       | Command Station <br> PC                                                                                                                                                                                        |
| \[3..0] |              `0` <br> `1` <br> `2` | Station Type | MX1 <br> MX8 <br> MX9                                                                                                                                                                                          |

### **cAdr** `ffaaaaaa aaaaaaaa`

The Address of a decoder, including the corresponding [Format specifier `ff`](#format-specifier-ff)

|   Bit(-s) | Acronym | Description      |
| --------: | ------: | ---------------- |
| \[15..13] |     `f` | Format Specifier |
|  \[12..0] |     `a` | Decoder Address  |

## Values

### **Format Specifier** `ff`

A two bit value represending the protocol format, the following address represents.

!!! info "Because of reasons, depending on the context of the message, the format specifier definition varies"

***For [Primary](../message_type/primary.md) messages:***

| Value | Description                              |
| ----: | ---------------------------------------- |
| `0 0` | Use protocol last used with this address |
| `1 0` | Force DCC protocol                       |
| `0 1` | Force Motorola protocol                  |
| `1 1` | `Reserved`                               |

***For [Level 1 Ack](../message_type/level_1_ack.md), [Reply Level 2](../message_type/reply_level_2.md) and [Level 2 Ack](../message_type/level_2_ack.md)***

| Value | Description                              |
| ----: | ---------------------------------------- |
| `0 0` | Use protocol last used with this address |
| `1 0` | Address uses DCC protocol                |
| `0 1` | Address uses Motorola protocol           |
| `1 1` | `Reserved`                               |

***For [Decoder CV access via serv-prog](../../commands/universal/read_set_decoder_cv.md)***

| Value | Description                              |
| ----: | ---------------------------------------- |
| `0 0` | Use protocol last used with this address |
| `1 0` | Force DCC protocol                       |
| `0 1` | Force Motorola protocol                  |
| `1 1` | `Reserved`                               |

***For [Decoder CV access via on-the-main programming](../../commands/universal/read_set_decoder_cv.md)***

| Value | Description               |
| ----: | ------------------------- |
| `0 0` | `Reserved`                |
| `1 0` | `Reserved`                |
| `0 1` | Decoder address           |
| `1 1` | Accessory decoder address |

### **Speed Step System specifier** `ss`

A two bit value represending the speed step system, the following speed is to be interpreted for

!!! info "Because of reasons, depending on the context of the message, the format specifier definition varies"

***For [Primary](../message_type/primary.md) messages:***

| Value | Description                                       |
| ----: | ------------------------------------------------- |
| `0 0` | Use speed step system last used with this address |
| `1 0` | Force 14 speed steps (0..14)                      |
| `0 1` | Force 28 speed steps (0..28)                      |
| `1 1` | Force 126 speed steps (0..126)                    |

***For [Level 1 Ack](../message_type/level_1_ack.md), [Reply Level 2](../message_type/reply_level_2.md) and [Level 2 Ack](../message_type/level_2_ack.md)***

| Value | Description                    |
| ----: | ------------------------------ |
| `0 0` | not used                       |
| `1 0` | Force 14 speed steps (0..14)   |
| `0 1` | Force 28 speed steps (0..28)   |
| `1 1` | Force 126 speed steps (0..126) |

### **Data flow specifier** `dd`

The fourth header byte of long data frames contains, besides the length of the header (original length
without counting escape character prefixes), additional data flow information. This information de-
scribes whether there will be any consecutive frames with data.

| Value | Description                    |
| ----: | ------------------------------ |
| `0 0` | No more data                   |
| `0 1` | More data will follow          |
| `1 0` | More data will possibly follow |
| `1 1` | `Reserved`                     |

### **Error codes**

The reply to message `0x0A` inherits an additional return code. This return code reflects the logical er-
ror-status of the sent message. If the message is transmitted correctly but there is a logical error
within the message that prevents the command station from executing it, then the codes below indi-
cate the reason for it.

| Return code         | Value | Description                              |
| ------------------- | ----- | ---------------------------------------- |
| NO_ERROR            | 0x00  | No Error                                 |
| ERR_ADRESSE         | 0x01  | Not a valid address                      |
| ERR_INDEX           | 0x02  | Error with the index of extended address |
| ERR_FORWARD         | 0x03  | Request could't be forwarded             |
| ERR_BUSY            | 0x04  | Busy with another command                |
| ERR_NO_MOT          | 0x05  | Motorola jumper off                      |
| ERR_NO_DCC          | 0x06  | DCC jumper off                           |
| ERR_CV_ADRESSE      | 0x07  | Not a valid CV address                   |
| ERR_SECTION         | 0x08  | Not a valid section                      |
| ERR_NO_MODUL        | 0x09  | Module with given address doesn't exist  |
| ERR_MESSAGE         | 0x0A  | Error within message                     |
| ERR_SPEED           | 0x0B  | Given speed not valid                    |
| ERR_ADRESSE_OCUPIED | 0x0C  | MXULF controls address                   |
