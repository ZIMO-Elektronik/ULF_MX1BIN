# Header

Each stream contains a header prepending the message. The header contains metadata to help
setting the context of the following message and parse it correctly.

## Short Frames

For short frames (Message size < 15 Byte) the header is structured as follows

| Offset | Value | Description                               |
| -----: | ----- | ----------------------------------------- |
|      0 | -     | [Unique sequence-ID](#unique-sequence-id) |
|      1 | I     | [Header info](#header-info)               |
|      2 | C     | [Command Code](#command-code)             |

If the header is within a reply frame, the header contains an additional byte,
matching the unique sequence-ID of the previous primary message.

| Offset | Value | Description                                                  |
| -----: | ----- | ------------------------------------------------------------ |
|      0 | -     | [Unique sequence-ID](#unique-sequence-id)                    |
|      1 | I     | [Header info](#header-info)                                  |
|      2 | C     | [Command Code](#command-code)                                |
|      3 | -     | [Unique sequence-ID](#unique-sequence-id) of primary message |

## Long Frames

Long frames contain an additional bitfield with flow metadata and the size of
the header

| Offset | Value    | Description                                    |
| -----: | -------- | ---------------------------------------------- |
|      0 | -        | [Unique sequence-ID](#unique-sequence-id)      |
|      1 | I        | [Header info](#header-info)                    |
|      2 | C        | [Command Code](#command-code)                  |
|      3 | dd00nnnn | [Flow Information](#flow-information-dd00nnnn) |

If the header is within a reply frame, the header contains an additional byte,
matching the unique sequence-ID of the previous primary message.

| Offset | Value    | Description                                                  |
| -----: | -------- | ------------------------------------------------------------ |
|      0 | -        | [Unique sequence-ID](#unique-sequence-id)                    |
|      1 | I        | [Header info](#header-info)                                  |
|      2 | C        | [Command Code](#command-code)                                |
|      3 | dd00nnnn | [Flow Information](#flow-information-dd00nnnn)               |
|      4 | -        | [Unique sequence-ID](#unique-sequence-id) of primary message |

## Bitfields and values

### Unique sequence-ID

An identifier associated with a message. A recommended implementation would be to
just increment the value for the next message. It is illegal to send two messages
with different context using the same ID.

### Header info

A metadata bitfield that specifies the context and origin of the following message.

|  Bit(s) |                              Value | Description                                                                                                     |
| ------: | ---------------------------------: | --------------------------------------------------------------------------------------------------------------- |
|    \[7] |                       `0` <br> `1` | Short frame <br> Long frame                                                                                     |
| \[6..5] | `00` <br> `10` <br> `01` <br> `11` | Primary message <br> Ack / Reply level 1 <br> Reply level 2 <br> Ack (for Reply level 2)                        |
|    \[4] |                       `0` <br> `1` | Sent by command station <br> Sent by PC                                                                         |
| \[3..0] |              `0` <br> `1` <br> `2` | Target is command station (MX1) <br> Target is accessory module (MX8) <br> Target is track section module (MX9) |

Bits 4-0 may alternatively considered to be a 5 bit part of the unique message
identifier. It should not be taken for granted that “useless” bit combinations
involving bit 4 strictly interpreted as origin identifier that are not used at
this stage might not be assigned some other meaning some day.

### Command Code

The code specifying the underlying command. This, in combination with the
[Header info](#header-info) byte largely specifies the structure of the following
message, as any message (-type) is very context sensitive.

A list of commands can be found [here](../../../commands/index.md)

### **Flow Information** `dd00nnnn`

The flow information byte contains the following two values

#### **Data flow information** `dd`

| Value | Description           |
| ----: | --------------------- |
| `0 0` | No more data follows  |
| `0 1` | More data WILL follow |
| `1 0` | More data MAY follow  |
| `1 1` | `Reserved`            |

#### **Size of header** `nnnn`

This contains the size of the header.

!!! info 

    Unsure, wether this is a definition error or not, the header size seems to only
    catch the size of the actual header (in this case 4, in case of reply 5 bytes).
    At least that is what the MXULF sends.