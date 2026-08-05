# Nak **Code 1**

A Nak is sent by the command station in case a message could not be decoded.

## Available Messages

### [Level 1 Ack](../../definition/message_type/level_1_ack.md)

| Offset | Section |   Size | Field                | Value | Description |
| -----: | ------: | -----: | -------------------- | ----- | ----------- |
|      0 |    Head | 1 Byte | [Unique Sequence-ID] | -     | -           |
|      1 |    Head | 1 Byte | [Header Info]        | -     | -           |
|      2 |    Head | 1 Byte | [Command Code]       | 1     | -           |

[Command Code]: ../../definition/structure/header/index.md#command-code
[Header Info]: ../../definition/structure/header/index.md#header-info
[Unique Sequence-ID]: ../../definition/structure/header/index.md#unique-sequence-id
