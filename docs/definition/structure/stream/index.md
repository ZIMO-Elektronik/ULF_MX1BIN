# Stream

The `MX1Bin` protocol sends any message as an encoded stream of data. The specification of transfer parameters, as well as the encode / decode process are documented here.

## Communication basics

The basic communication format is N,8,1 (no parity, 8 data bits, 1 stop bit). Communication speed
ranges from 1200 to 38400 bit/s and can be selected via CV12. The default value is 9600 bit/s.

## Flow control

Flow control is done via hardware (RTS/CTS). It is necessary to use a serial cable with at least 5
wires which also connects the RTS and CTS lines between PC and command station. Hardware flow
control can be disabled by setting CV13 to 0. Default value is 1 (hardware flow control enabled).

## The new binary communication

The first byte of each header is the unique sequence-ID of the frame and must not be identical in two different consecutive frames. If a frame has to be repeated the sequence-ID remains unchanged. The second byte specifies the message type and the meaning of additional header bytes. Most messages are identified by the contents if the 2nd and 3rd header byte. The 2nd byte provides some kind of routing information the 3rd byte identifies a specific message. Together these two bytes might be considered as unique 16 bit message identifier.

Each data frame is immediately acknowledged by the receiver (level 1 reply). This reply may contain the appropriate data if it's immediately available in the command station. Otherwise the request is passed on via the CAN bus (to another station for example) and the subsequent incoming data is returned (level 2 reply).

!!! info "Future expansions of single messages may involve additional bytes added to the frame."

## Frame structure

The first byte of each header is the unique sequence-ID of the frame and must not be identical in two different consecutive frames. If a frame has to be repeated the sequence-ID remains unchanged. The second byte specifies the message type and the meaning of additional header bytes. Most messages are identified by the contents if the 2nd and 3rd header byte. The 2nd byte provides some kind of routing information the 3rd byte identifies a specific message. Together these two bytes might be considered as unique 16 bit message identifier.

To establish a data transmission independent of content, all data (including headerinfo and check-
sum) characters identical to control characters have to be protected with the additional escape char-
acter prefix `DLE` and the character itself is XOR'ed with `0x20`.

!!! warning "Worst case, this may result in an encoded frame almost double the size of the actual frame."

| Control character | Value  | Replacement within data | Description           |
| :---------------: | ------ | ----------------------- | --------------------- |
|       `SOH`       | `0x01` | `DLE`(`SOH` \^ `0x20`)  | Start of a data frame |
|       `EOT`       | `0x17` | `DLE`(`EOT` \^ `0x20`)  | End of a data frame   |
|       `DLE`       | `0x10` | `DLE`(`DLE` \^ `0x20`)  | Escape character      |

Each frame has one of the following layouts:

- Short frames: \[`SOH`-`SOH`-[`Header`](../header/index.md)-[`Message`](../message/index.md)-[`CRC8`](../crc/index.md)-`EOT`]

- Long frames: \[`SOH`-`SOH`-[`Header`](../header/index.md)-[`Message`](../message/index.md)-[`CRC16`](../crc/index.md)-`EOT`]
