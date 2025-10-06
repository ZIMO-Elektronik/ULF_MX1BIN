## 1. Communication basics:
The basic communication format is N,8,1 (no parity, 8 data bits, 1 stop bit). Communication speed
ranges from 1200 to 38400 bit/s and can be selected via CV12. The default value is 9600 bit/s.

## 2. Flow control
Flow control is done via hardware (RTS/CTS). It is necessary to use a serial cable with at least 5
wires which also connects the RTS and CTS lines between PC and command station. Hardware flow
control can be disabled by setting CV13 to 0. Default value is 1 (hardware flow control enabled).

## 3. The new binary communication
The new binary communication consists of data frames with the following structure: 

- Long frames: 

    \<SOH\>\<SOH\>headerinfo_and_data[checksum16]\<EOT\>

- Short frames: 

    \<SOH\>\<SOH\>headerinfo_and_data[checksum8]\<EOT\>

To establish a data transmission independent of content, all data (including headerinfo and check-
sum) characters identical to control characters have to be protected with the additional escape char-
acter prefix <DLE> and the character itself is XOR'ed with 0x20.

| Control character | Value | Replacement within data | Description           |
| :---------------: | ----- | ----------------------- | --------------------- |
| SOH               | 0x01  | \<DLE\>(\<SOH\>\^0x20)  | Start of a data frame | 
| EOT               | 0x17  | \<DLE\>(\<EOT\>\^0x20)  | End of a data frame   | 
| DLE               | 0x10  | \<DLE\>(\<DLE\>\^0x20)  | Escape character      | 

Each data frame is immediately acknowledged by the receiver (level 1 reply). This reply may contain
the appropriate data if it's immediately available in the command station. Otherwise the request is
passed on via the CAN bus (to another station for example) and the subsequent incoming data is re-
turned (level 2 reply).

Please note: Future expansions of single messages may involve additional bytes added to the frame.

### 3.1 Header info
The header info describes the meaning of the data content of a frame. The length of the header info
is between 2 and 15 bytes. The first byte is the unique sequence-ID of the frame and must not be
identical in two different consecutive frames. If a frame has to be repeated the sequence-ID remains
unchanged. The second byte specifies the message type and the meaning of additional header
bytes. Most messages are identified by the contents if the 2nd and 3rd header byte. The 2nd byte pro-
vides some kind of routing information the 3rd byte identifies a specific message. Together these two
bytes might be considered as unique 16 bit message identifier.

##### Common information in header byte 2: 

| Bit(s) | Value                      | Description |
| :----: | -------------------------- | ----------- |
| [7]    | 0 <br> 1                   | Short frame <br> Long frame |
| [6..5] | 00 <br> 10 <br> 01 <br> 11 | Primary message <br> Ack / Reply level 1 <br> Reply level 2 <br> Ack (for Reply level 2) |
| [4]    | 0 <br> 1                   | Sent by command station <br> Sent by PC |
| [3..0] | 0 <br> 1 <br> 2            | Target is command station (MX1) <br> Target is accessory module (MX8) <br> Target is track section module (MX9) |

Bits 4-0 may alternatively considered to be a 5 bit part of the unique message identifier. It should not
be taken for granted that “useless” bit combinations involving bit 4 strictly interpreted as origin identi-
fier that are not used at this stage might not be assigned some other meaning some day.

#### 3.1.1 Frame structure
Generally, each frame is split into a 3 byte header and N byte message. The header is structured as follows: 
| Header byte(s)  | Value   | Description           |
| :-------------- | ------- | --------------------- |
| [0]             | 0..255  | Unique sequence-ID    |
| [1]             | I       | Header Info byte (I)  |
| [2]             | C       | Message code (C)      |

The Unique sequence-ID (uSID) is upward counting and can is counted seperatly for PC and Module. 

The message code identifies the implied instruction, as well as the structure / length of the message itself. 

#### 3.1.2 Short frame protocol primary messages
Short frames can hold up to 15 bytes of data (including the header). They are secured with a CRC8. Each of the described messages only counts the actual message not including the header. 

##### Dummy {#dummy}
| Message byte(s) | Name | Value | Description |
| :-------------- | ---- | ----- | ----------- |

##### Reset : Code 0 - Primary - Any {#reset_pri}
Reset message. Upon receiving this message, the internal state to be reset. This is a header-only message.

Reply: [Ack Level 1](#generic_ack)

##### Tack Control : Code 2 - Primary - Command station {#trackcontrol_pri} 
Message to control the state of the track. Instructions should be handled immediatly. 

| Message byte(s) | Name    | Value                  | Description |
| :-------------- | ------- | ---------------------- | ----------- |
| [0]             | cAction | 0 <br> 1 <br> 2 <br> 3 | Broadcast stop (Stop all locos) <br> Switch track voltage OFF <br> Switch track voltage ON <br> Query track status |

Reply: [Dummy](#dummy)

##### Loco Control : Code 3 - Primary - Command station {#lococontrol_pri}
Message to control a single loco on the track. 

| Message byte(s) | Name   | Value             | Description                            |
| :-------------- | ------ | ----------------- | -------------------------------------- |
| [0..1]          | cAdr   | ffaaaaaa aaaaaaaa | ff - [Dummy](#dummy) <br> a..a Address |
| [2]             | cSpeed | esssssss          | e - Emergency stop <br> Speed          |
| [3] optional    | cData1 | -                 | [7] Manual (ignore limits) <br> [6] N/A <br> [5] Direction (0 = fw, 1 = bw) <br> [4] Headlights (=DCC F0) <br> [3..2] Speed step system <br> [1] Decel. time enabled <br> [0] accel. time enabled | 
| [4] optional    | cData2 | -                 | [7..0] F1..F8 (DCC only)               |
| [5] optional    | cData3 | -                 | [3..0] F9..F12 (DCC only)              |
| [6] optional    | cData4 | -                 | [7..0] F13..F20 (DCC only)             | 
| [7] optional    | cData5 | -                 | [7..0] F21..F28 (DCC only)             |

Reply: [Dummy](#dummy)

##### Invert function bits : Code 4 - Primary - Command station {#invertbits_pri}
Message to invert function bits instead of setting

| Message byte(s) | Name   | Value             | Description                            |
| :-------------- | ------ | ----------------- | -------------------------------------- |
| [0..1]          | cAdr   | ffaaaaaa aaaaaaaa | ff - [Dummy](#dummy) <br> a..a Address |
| [2]             | cSpeed | esssssss          | e - Emergency stop <br> Speed          |
| [3]             | cData1 | -                 | [7] Manual (ignore limits) <br> [6] N/A <br> [5] Direction (0 = fw, 1 = bw) <br> [4] Headlights (=DCC F0) <br> [3..2] Speed step system <br> [1] Decel. time enabled <br> [0] accel. time enabled | 
| [4]             | cData2 | -                 | [7..0] F1..F8 (DCC only)               |
| [5]             | cData3 | -                 | [3..0] F9..F12 (DCC only)              |
| [6]             | cData4 | -                 | [7..0] F13..F20 (DCC only)             | 
| [7]             | cData5 | -                 | [7..0] F21..F28 (DCC only)             |

Reply: [Dummy](#dummy)

##### Acceleration / Deceleration : Code 5 - Primary - Command station {#acceldeccel_pri}
Message to set acceleration and decelartion times. 

| Message byte(s) | Name   | Value             | Description                              |
| :-------------- | ------ | ----------------- | ---------------------------------------- |
| [0..1]          | cAdr   | ffaaaaaa aaaaaaaa | ff - [Dummy](#dummy) <br> a..a Address   |
| [2]             | cAzBz  | -                 | [0..3] BZ (0..15) <br> [4..7] AZ (0..15) |

Reply: [Dummy](#dummy)

##### Shuttle train : Code 6 - Primary - Command station {#shuttletrain_pri}

| Message byte(s) | Name   | Value             | Description                              |
| :-------------- | ------ | ----------------- | ---------------------------------------- |
| [0..1]          | cAdr   | ffaaaaaa aaaaaaaa | ff - [Dummy](#dummy) <br> a..a Address   |
| [2]             | cData  | -                 | [0..3] Contact rails 1..4 forward <br> [4..7] Contact rails 1..4 reverse |

Reply: [Dummy](#dummy)

##### Accessory decoder : Code 7 - Primary - Command station {#accessorydecoder_pri}

| Message byte(s) | Name   | Value             | Description                              |
| :-------------- | ------ | ----------------- | ---------------------------------------- |
| [0..1]          | cAdr   | ffaaaaaa aaaaaaaa | ff - [Dummy](#dummy) <br> a..a Address   |
| [2]             | cData  | -                 | [3] 1=on, 0=off <br> [2..0] output number |

Reply: [Dummy](#dummy)

##### Query command station's loco memory : Code 8 - Primary - Command station {#locomemoryquery_pri}

| Message byte(s) | Name   | Value             | Description                              |
| :-------------- | ------ | ----------------- | ---------------------------------------- |
| [0..1]          | cAdr   | ffaaaaaa aaaaaaaa | ff - [Dummy](#dummy) <br> a..a Address   |

##### Query command station's accessory decoder memory : Code 8 - Primary - Command station {#accessorymemoryquery_pri}
 
| Message byte(s) | Name   | Value             | Description                              |
| :-------------- | ------ | ----------------- | ---------------------------------------- |
| [0..1]          | cAdr   | ffaaaaaa aaaaaaaa | ff - [Dummy](#dummy) <br> a..a Address   |

##### Address control : Code 9 - Primary - Command station {#addresscontrol_pri}


#### 3.1.3 Short frame protocol reply / acknowlegement messages

##### Ack : Reply level 1 - Code C {#generic_ack}
A generic Ack that confirms the message reception. This message inherits the message code of the matching primary message 

| Message byte(s) | Value      | Description                         |
| :-------------- | ---------- | ----------------------------------- |
| [0]             | Primary-ID | uSID of the replied primary message |

##### Nak : Reply level 1 - Code 1 {#generic_nak}
A generic Nak that implies problems during message reception. This is a header-only message.